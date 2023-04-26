#!/usr/bin/env python
#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""simpleperf_report_lib.py: a python wrapper of libsimpleperf_report.so.
   Used to access samples in perf.data.

"""

import collections
import ctypes as ct
import struct
from utils import bytes_to_str, get_host_binary_path, is_windows, str_to_bytes


def _get_native_lib():
    return get_host_binary_path('libsimpleperf_report.so')


def _is_null(p):
    if p:
        return False
    return ct.cast(p, ct.c_void_p).value is None


def _char_pt(s):
    return str_to_bytes(s)


def _char_pt_to_str(char_pt):
    return bytes_to_str(char_pt)

def _check(cond, failmsg):
    if not cond:
        raise RuntimeError(failmsg)


class SampleStruct(ct.Structure):
    """ Instance of a sample in perf.data.
        ip: the program counter of the thread generating the sample.
        pid: process id (or thread group id) of the thread generating the sample.
        tid: thread id.
        thread_comm: thread name.
        time: time at which the sample was generated. The value is in nanoseconds.
              The clock is decided by the --clockid option in `simpleperf record`.
        in_kernel: whether the instruction is in kernel space or user space.
        cpu: the cpu generating the sample.
        period: count of events have happened since last sample. For example, if we use
             -e cpu-cycles, it means how many cpu-cycles have happened.
             If we use -e cpu-clock, it means how many nanoseconds have passed.
    """
    _fields_ = [('ip', ct.c_uint64),
                ('pid', ct.c_uint32),
                ('tid', ct.c_uint32),
                ('_thread_comm', ct.c_char_p),
                ('time', ct.c_uint64),
                ('in_kernel', ct.c_uint32),
                ('cpu', ct.c_uint32),
                ('period', ct.c_uint64)]

    @property
    def thread_comm(self):
        return _char_pt_to_str(self._thread_comm)


class TracingFieldFormatStruct(ct.Structure):
    """Format of a tracing field.
       name: name of the field.
       offset: offset of the field in tracing data.
       elem_size: size of the element type.
       elem_count: the number of elements in this field, more than one if the field is an array.
       is_signed: whether the element type is signed or unsigned.
    """
    _fields_ = [('_name', ct.c_char_p),
                ('offset', ct.c_uint32),
                ('elem_size', ct.c_uint32),
                ('elem_count', ct.c_uint32),
                ('is_signed', ct.c_uint32)]

    _unpack_key_dict = {1: 'b', 2: 'h', 4: 'i', 8: 'q'}

    @property
    def name(self):
        return _char_pt_to_str(self._name)

    def parse_value(self, data):
        """ Parse value of a field in a tracepoint event.
            The return value depends on the type of the field, and can be an int value, a string,
            an array of int values, etc. If the type can't be parsed, return a byte array or an
            array of byte arrays.
        """
        if self.elem_count > 1 and self.elem_size == 1 and self.is_signed == 0:
            # The field is a string.
            length = 0
            while length < self.elem_count and bytes_to_str(data[self.offset + length]) != '\x00':
                length += 1
            return bytes_to_str(data[self.offset : self.offset + length])
        unpack_key = self._unpack_key_dict.get(self.elem_size)
        if unpack_key:
            if not self.is_signed:
                unpack_key = unpack_key.upper()
            value = struct.unpack('%d%s' % (self.elem_count, unpack_key),
                                  data[self.offset:self.offset + self.elem_count * self.elem_size])
        else:
            # Since we don't know the element type, just return the bytes.
            value = []
            offset = self.offset
            for _ in range(self.elem_count):
                value.append(data[offset : offset + self.elem_size])
                offset += self.elem_size
        if self.elem_count == 1:
            value = value[0]
        return value


class TracingDataFormatStruct(ct.Structure):
    """Format of tracing data of a tracepoint event, like
       https://www.kernel.org/doc/html/latest/trace/events.html#event-formats.
       size: total size of all fields in the tracing data.
       field_count: the number of fields.
       fields: an array of fields.
    """
    _fields_ = [('size', ct.c_uint32),
                ('field_count', ct.c_uint32),
                ('fields', ct.POINTER(TracingFieldFormatStruct))]


class EventStruct(ct.Structure):
    """Event type of a sample.
       name: name of the event type.
       tracing_data_format: only available when it is a tracepoint event.
    """
    _fields_ = [('_name', ct.c_char_p),
                ('tracing_data_format', TracingDataFormatStruct)]

    @property
    def name(self):
        return _char_pt_to_str(self._name)


class MappingStruct(ct.Structure):
    """ A mapping area in the monitored threads, like the content in /proc/<pid>/maps.
        start: start addr in memory.
        end: end addr in memory.
        pgoff: offset in the mapped shared library.
    """
    _fields_ = [('start', ct.c_uint64),
                ('end', ct.c_uint64),
                ('pgoff', ct.c_uint64)]


class SymbolStruct(ct.Structure):
    """ Symbol info of the instruction hit by a sample or a callchain entry of a sample.
        dso_name: path of the shared library containing the instruction.
        vaddr_in_file: virtual address of the instruction in the shared library.
        symbol_name: name of the function containing the instruction.
        symbol_addr: start addr of the function containing the instruction.
        symbol_len: length of the function in the shared library.
        mapping: the mapping area hit by the instruction.
    """
    _fields_ = [('_dso_name', ct.c_char_p),
                ('vaddr_in_file', ct.c_uint64),
                ('_symbol_name', ct.c_char_p),
                ('symbol_addr', ct.c_uint64),
                ('symbol_len', ct.c_uint64),
                ('mapping', ct.POINTER(MappingStruct))]

    @property
    def dso_name(self):
        return _char_pt_to_str(self._dso_name)

    @property
    def symbol_name(self):
        return _char_pt_to_str(self._symbol_name)


class CallChainEntryStructure(ct.Structure):
    """ A callchain entry of a sample.
        ip: the address of the instruction of the callchain entry.
        symbol: symbol info of the callchain entry.
    """
    _fields_ = [('ip', ct.c_uint64),
                ('symbol', SymbolStruct)]


class CallChainStructure(ct.Structure):
    """ Callchain info of a sample.
        nr: number of entries in the callchain.
        entries: a pointer to an array of CallChainEntryStructure.

        For example, if a sample is generated when a thread is running function C
        with callchain function A -> function B -> function C.
        Then nr = 2, and entries = [function B, function A].
    """
    _fields_ = [('nr', ct.c_uint32),
                ('entries', ct.POINTER(CallChainEntryStructure))]


class FeatureSectionStructure(ct.Structure):
    """ A feature section in perf.data to store information like record cmd, device arch, etc.
        data: a pointer to a buffer storing the section data.
        data_size: data size in bytes.
    """
    _fields_ = [('data', ct.POINTER(ct.c_char)),
                ('data_size', ct.c_uint32)]


class ReportLibStructure(ct.Structure):
    _fields_ = []


# pylint: disable=invalid-name
class ReportLib(object):

    def __init__(self, native_lib_path=None):
        if native_lib_path is None:
            native_lib_path = _get_native_lib()

        self._load_dependent_lib()
        self._lib = ct.CDLL(native_lib_path)
        self._CreateReportLibFunc = self._lib.CreateReportLib
        self._CreateReportLibFunc.restype = ct.POINTER(ReportLibStructure)
        self._DestroyReportLibFunc = self._lib.DestroyReportLib
        self._SetLogSeverityFunc = self._lib.SetLogSeverity
        self._SetSymfsFunc = self._lib.SetSymfs
        self._SetRecordFileFunc = self._lib.SetRecordFile
        self._SetKallsymsFileFunc = self._lib.SetKallsymsFile
        self._ShowIpForUnknownSymbolFunc = self._lib.ShowIpForUnknownSymbol
        self._ShowArtFramesFunc = self._lib.ShowArtFrames
        self._MergeJavaMethodsFunc = self._lib.MergeJavaMethods
        self._GetNextSampleFunc = self._lib.GetNextSample
        self._GetNextSampleFunc.restype = ct.POINTER(SampleStruct)
        self._GetEventOfCurrentSampleFunc = self._lib.GetEventOfCurrentSample
        self._GetEventOfCurrentSampleFunc.restype = ct.POINTER(EventStruct)
        self._GetSymbolOfCurrentSampleFunc = self._lib.GetSymbolOfCurrentSample
        self._GetSymbolOfCurrentSampleFunc.restype = ct.POINTER(SymbolStruct)
        self._GetCallChainOfCurrentSampleFunc = self._lib.GetCallChainOfCurrentSample
        self._GetCallChainOfCurrentSampleFunc.restype = ct.POINTER(CallChainStructure)
        self._GetTracingDataOfCurrentSampleFunc = self._lib.GetTracingDataOfCurrentSample
        self._GetTracingDataOfCurrentSampleFunc.restype = ct.POINTER(ct.c_char)
        self._GetBuildIdForPathFunc = self._lib.GetBuildIdForPath
        self._GetBuildIdForPathFunc.restype = ct.c_char_p
        self._GetFeatureSection = self._lib.GetFeatureSection
        self._GetFeatureSection.restype = ct.POINTER(FeatureSectionStructure)
        self._instance = self._CreateReportLibFunc()
        assert not _is_null(self._instance)

        self.meta_info = None
        self.current_sample = None
        self.record_cmd = None

    def _load_dependent_lib(self):
        # As the windows dll is built with mingw we need to load 'libwinpthread-1.dll'.
        if is_windows():
            self._libwinpthread = ct.CDLL(get_host_binary_path('libwinpthread-1.dll'))

    def Close(self):
        if self._instance is None:
            return
        self._DestroyReportLibFunc(self._instance)
        self._instance = None

    def SetLogSeverity(self, log_level='info'):
        """ Set log severity of native lib, can be verbose,debug,info,error,fatal."""
        cond = self._SetLogSeverityFunc(self.getInstance(), _char_pt(log_level))
        _check(cond, 'Failed to set log level')

    def SetSymfs(self, symfs_dir):
        """ Set directory used to find symbols."""
        cond = self._SetSymfsFunc(self.getInstance(), _char_pt(symfs_dir))
        _check(cond, 'Failed to set symbols directory')

    def SetRecordFile(self, record_file):
        """ Set the path of record file, like perf.data."""
        cond = self._SetRecordFileFunc(self.getInstance(), _char_pt(record_file))
        _check(cond, 'Failed to set record file')

    def ShowIpForUnknownSymbol(self):
        self._ShowIpForUnknownSymbolFunc(self.getInstance())

    def ShowArtFrames(self, show=True):
        """ Show frames of internal methods of the Java interpreter. """
        self._ShowArtFramesFunc(self.getInstance(), show)

    def MergeJavaMethods(self, merge=True):
        """ This option merges jitted java methods with the same name but in different jit
            symfiles. If possible, it also merges jitted methods with interpreted methods,
            by mapping jitted methods to their corresponding dex files.
            Side effects:
              It only works at method level, not instruction level.
              It makes symbol.vaddr_in_file and symbol.mapping not accurate for jitted methods.
            Java methods are merged by default.
        """
        self._MergeJavaMethodsFunc(self.getInstance(), merge)

    def SetKallsymsFile(self, kallsym_file):
        """ Set the file path to a copy of the /proc/kallsyms file (for off device decoding) """
        cond = self._SetKallsymsFileFunc(self.getInstance(), _char_pt(kallsym_file))
        _check(cond, 'Failed to set kallsyms file')

    def GetNextSample(self):
        psample = self._GetNextSampleFunc(self.getInstance())
        if _is_null(psample):
            self.current_sample = None
        else:
            self.current_sample = psample[0]
        return self.current_sample

    def GetCurrentSample(self):
        return self.current_sample

    def GetEventOfCurrentSample(self):
        event = self._GetEventOfCurrentSampleFunc(self.getInstance())
        assert not _is_null(event)
        return event[0]

    def GetSymbolOfCurrentSample(self):
        symbol = self._GetSymbolOfCurrentSampleFunc(self.getInstance())
        assert not _is_null(symbol)
        return symbol[0]

    def GetCallChainOfCurrentSample(self):
        callchain = self._GetCallChainOfCurrentSampleFunc(self.getInstance())
        assert not _is_null(callchain)
        return callchain[0]

    def GetTracingDataOfCurrentSample(self):
        data = self._GetTracingDataOfCurrentSampleFunc(self.getInstance())
        if _is_null(data):
            return None
        event = self.GetEventOfCurrentSample()
        result = collections.OrderedDict()
        for i in range(event.tracing_data_format.field_count):
            field = event.tracing_data_format.fields[i]
            result[field.name] = field.parse_value(data)
        return result

    def GetBuildIdForPath(self, path):
        build_id = self._GetBuildIdForPathFunc(self.getInstance(), _char_pt(path))
        assert not _is_null(build_id)
        return _char_pt_to_str(build_id)

    def GetRecordCmd(self):
        if self.record_cmd is not None:
            return self.record_cmd
        self.record_cmd = ''
        feature_data = self._GetFeatureSection(self.getInstance(), _char_pt('cmdline'))
        if not _is_null(feature_data):
            void_p = ct.cast(feature_data[0].data, ct.c_void_p)
            arg_count = ct.cast(void_p, ct.POINTER(ct.c_uint32)).contents.value
            void_p.value += 4
            args = []
            for _ in range(arg_count):
                str_len = ct.cast(void_p, ct.POINTER(ct.c_uint32)).contents.value
                void_p.value += 4
                char_p = ct.cast(void_p, ct.POINTER(ct.c_char))
                current_str = ''
                for j in range(str_len):
                    c = bytes_to_str(char_p[j])
                    if c != '\0':
                        current_str += c
                if ' ' in current_str:
                    current_str = '"' + current_str + '"'
                args.append(current_str)
                void_p.value += str_len
            self.record_cmd = ' '.join(args)
        return self.record_cmd

    def _GetFeatureString(self, feature_name):
        feature_data = self._GetFeatureSection(self.getInstance(), _char_pt(feature_name))
        result = ''
        if not _is_null(feature_data):
            void_p = ct.cast(feature_data[0].data, ct.c_void_p)
            str_len = ct.cast(void_p, ct.POINTER(ct.c_uint32)).contents.value
            void_p.value += 4
            char_p = ct.cast(void_p, ct.POINTER(ct.c_char))
            for i in range(str_len):
                c = bytes_to_str(char_p[i])
                if c == '\0':
                    break
                result += c
        return result

    def GetArch(self):
        return self._GetFeatureString('arch')

    def MetaInfo(self):
        """ Return a string to string map stored in meta_info section in perf.data.
            It is used to pass some short meta information.
        """
        if self.meta_info is None:
            self.meta_info = {}
            feature_data = self._GetFeatureSection(self.getInstance(), _char_pt('meta_info'))
            if not _is_null(feature_data):
                str_list = []
                data = feature_data[0].data
                data_size = feature_data[0].data_size
                current_str = ''
                for i in range(data_size):
                    c = bytes_to_str(data[i])
                    if c != '\0':
                        current_str += c
                    else:
                        str_list.append(current_str)
                        current_str = ''
                for i in range(0, len(str_list), 2):
                    self.meta_info[str_list[i]] = str_list[i + 1]
        return self.meta_info

    def getInstance(self):
        if self._instance is None:
            raise Exception('Instance is Closed')
        return self._instance
