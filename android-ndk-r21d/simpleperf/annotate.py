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

"""annotate.py: annotate source files based on perf.data.
"""


import argparse
import os
import os.path
import shutil

from simpleperf_report_lib import ReportLib
from utils import log_info, log_warning, log_exit
from utils import Addr2Nearestline, extant_dir, flatten_arg_list, is_windows, SourceFileSearcher

class SourceLine(object):
    def __init__(self, file_id, function, line):
        self.file = file_id
        self.function = function
        self.line = line

    @property
    def file_key(self):
        return self.file

    @property
    def function_key(self):
        return (self.file, self.function)

    @property
    def line_key(self):
        return (self.file, self.line)


class Addr2Line(object):
    """collect information of how to map [dso_name, vaddr] to [source_file:line].
    """
    def __init__(self, ndk_path, binary_cache_path, source_dirs):
        self.addr2line = Addr2Nearestline(ndk_path, binary_cache_path, True)
        self.source_searcher = SourceFileSearcher(source_dirs)

    def add_addr(self, dso_path, func_addr, addr):
        self.addr2line.add_addr(dso_path, func_addr, addr)

    def convert_addrs_to_lines(self):
        self.addr2line.convert_addrs_to_lines()

    def get_sources(self, dso_path, addr):
        dso = self.addr2line.get_dso(dso_path)
        if not dso:
            return []
        source = self.addr2line.get_addr_source(dso, addr)
        if not source:
            return []
        result = []
        for (source_file, source_line, function_name) in source:
            source_file_path = self.source_searcher.get_real_path(source_file)
            if not source_file_path:
                source_file_path = source_file
            result.append(SourceLine(source_file_path, function_name, source_line))
        return result


class Period(object):
    """event count information. It can be used to represent event count
       of a line, a function, a source file, or a binary. It contains two
       parts: period and acc_period.
       When used for a line, period is the event count occurred when running
       that line, acc_period is the accumulated event count occurred when
       running that line and functions called by that line. Same thing applies
       when it is used for a function, a source file, or a binary.
    """
    def __init__(self, period=0, acc_period=0):
        self.period = period
        self.acc_period = acc_period


    def __iadd__(self, other):
        self.period += other.period
        self.acc_period += other.acc_period
        return self


class DsoPeriod(object):
    """Period for each shared library"""
    def __init__(self, dso_name):
        self.dso_name = dso_name
        self.period = Period()


    def add_period(self, period):
        self.period += period


class FilePeriod(object):
    """Period for each source file"""
    def __init__(self, file_id):
        self.file = file_id
        self.period = Period()
        # Period for each line in the file.
        self.line_dict = {}
        # Period for each function in the source file.
        self.function_dict = {}


    def add_period(self, period):
        self.period += period


    def add_line_period(self, line, period):
        a = self.line_dict.get(line)
        if a is None:
            self.line_dict[line] = a = Period()
        a += period


    def add_function_period(self, function_name, function_start_line, period):
        a = self.function_dict.get(function_name)
        if not a:
            if function_start_line is None:
                function_start_line = -1
            self.function_dict[function_name] = a = [function_start_line, Period()]
        a[1] += period


class SourceFileAnnotator(object):
    """group code for annotating source files"""
    def __init__(self, config):
        # check config variables
        config_names = ['perf_data_list', 'source_dirs', 'comm_filters',
                        'pid_filters', 'tid_filters', 'dso_filters', 'ndk_path']
        for name in config_names:
            if name not in config:
                log_exit('config [%s] is missing' % name)
        symfs_dir = 'binary_cache'
        if not os.path.isdir(symfs_dir):
            symfs_dir = None
        kallsyms = 'binary_cache/kallsyms'
        if not os.path.isfile(kallsyms):
            kallsyms = None

        # init member variables
        self.config = config
        self.symfs_dir = symfs_dir
        self.kallsyms = kallsyms
        self.comm_filter = set(config['comm_filters']) if config.get('comm_filters') else None
        if config.get('pid_filters'):
            self.pid_filter = {int(x) for x in config['pid_filters']}
        else:
            self.pid_filter = None
        if config.get('tid_filters'):
            self.tid_filter = {int(x) for x in config['tid_filters']}
        else:
            self.tid_filter = None
        self.dso_filter = set(config['dso_filters']) if config.get('dso_filters') else None

        config['annotate_dest_dir'] = 'annotated_files'
        output_dir = config['annotate_dest_dir']
        if os.path.isdir(output_dir):
            shutil.rmtree(output_dir)
        os.makedirs(output_dir)


        self.addr2line = Addr2Line(self.config['ndk_path'], symfs_dir, config.get('source_dirs'))
        self.period = 0
        self.dso_periods = {}
        self.file_periods = {}


    def annotate(self):
        self._collect_addrs()
        self._convert_addrs_to_lines()
        self._generate_periods()
        self._write_summary()
        self._annotate_files()


    def _collect_addrs(self):
        """Read perf.data, collect all addresses we need to convert to
           source file:line.
        """
        for perf_data in self.config['perf_data_list']:
            lib = ReportLib()
            lib.SetRecordFile(perf_data)
            if self.symfs_dir:
                lib.SetSymfs(self.symfs_dir)
            if self.kallsyms:
                lib.SetKallsymsFile(self.kallsyms)
            while True:
                sample = lib.GetNextSample()
                if sample is None:
                    lib.Close()
                    break
                if not self._filter_sample(sample):
                    continue
                symbols = []
                symbols.append(lib.GetSymbolOfCurrentSample())
                callchain = lib.GetCallChainOfCurrentSample()
                for i in range(callchain.nr):
                    symbols.append(callchain.entries[i].symbol)
                for symbol in symbols:
                    if self._filter_symbol(symbol):
                        self.addr2line.add_addr(symbol.dso_name, symbol.symbol_addr,
                                                symbol.vaddr_in_file)
                        self.addr2line.add_addr(symbol.dso_name, symbol.symbol_addr,
                                                symbol.symbol_addr)


    def _filter_sample(self, sample):
        """Return true if the sample can be used."""
        if self.comm_filter:
            if sample.thread_comm not in self.comm_filter:
                return False
        if self.pid_filter:
            if sample.pid not in self.pid_filter:
                return False
        if self.tid_filter:
            if sample.tid not in self.tid_filter:
                return False
        return True


    def _filter_symbol(self, symbol):
        if not self.dso_filter or symbol.dso_name in self.dso_filter:
            return True
        return False


    def _convert_addrs_to_lines(self):
        self.addr2line.convert_addrs_to_lines()


    def _generate_periods(self):
        """read perf.data, collect Period for all types:
            binaries, source files, functions, lines.
        """
        for perf_data in self.config['perf_data_list']:
            lib = ReportLib()
            lib.SetRecordFile(perf_data)
            if self.symfs_dir:
                lib.SetSymfs(self.symfs_dir)
            if self.kallsyms:
                lib.SetKallsymsFile(self.kallsyms)
            while True:
                sample = lib.GetNextSample()
                if sample is None:
                    lib.Close()
                    break
                if not self._filter_sample(sample):
                    continue
                self._generate_periods_for_sample(lib, sample)


    def _generate_periods_for_sample(self, lib, sample):
        symbols = []
        symbols.append(lib.GetSymbolOfCurrentSample())
        callchain = lib.GetCallChainOfCurrentSample()
        for i in range(callchain.nr):
            symbols.append(callchain.entries[i].symbol)
        # Each sample has a callchain, but its period is only used once
        # to add period for each function/source_line/source_file/binary.
        # For example, if more than one entry in the callchain hits a
        # function, the event count of that function is only increased once.
        # Otherwise, we may get periods > 100%.
        is_sample_used = False
        used_dso_dict = {}
        used_file_dict = {}
        used_function_dict = {}
        used_line_dict = {}
        period = Period(sample.period, sample.period)
        for j, symbol in enumerate(symbols):
            if j == 1:
                period = Period(0, sample.period)
            if not self._filter_symbol(symbol):
                continue
            is_sample_used = True
            # Add period to dso.
            self._add_dso_period(symbol.dso_name, period, used_dso_dict)
            # Add period to source file.
            sources = self.addr2line.get_sources(symbol.dso_name, symbol.vaddr_in_file)
            for source in sources:
                if source.file:
                    self._add_file_period(source, period, used_file_dict)
                    # Add period to line.
                    if source.line:
                        self._add_line_period(source, period, used_line_dict)
            # Add period to function.
            sources = self.addr2line.get_sources(symbol.dso_name, symbol.symbol_addr)
            for source in sources:
                if source.file:
                    self._add_file_period(source, period, used_file_dict)
                    if source.function:
                        self._add_function_period(source, period, used_function_dict)

        if is_sample_used:
            self.period += sample.period


    def _add_dso_period(self, dso_name, period, used_dso_dict):
        if dso_name not in used_dso_dict:
            used_dso_dict[dso_name] = True
            dso_period = self.dso_periods.get(dso_name)
            if dso_period is None:
                dso_period = self.dso_periods[dso_name] = DsoPeriod(dso_name)
            dso_period.add_period(period)


    def _add_file_period(self, source, period, used_file_dict):
        if source.file_key not in used_file_dict:
            used_file_dict[source.file_key] = True
            file_period = self.file_periods.get(source.file)
            if file_period is None:
                file_period = self.file_periods[source.file] = FilePeriod(source.file)
            file_period.add_period(period)


    def _add_line_period(self, source, period, used_line_dict):
        if source.line_key not in used_line_dict:
            used_line_dict[source.line_key] = True
            file_period = self.file_periods[source.file]
            file_period.add_line_period(source.line, period)


    def _add_function_period(self, source, period, used_function_dict):
        if source.function_key not in used_function_dict:
            used_function_dict[source.function_key] = True
            file_period = self.file_periods[source.file]
            file_period.add_function_period(source.function, source.line, period)


    def _write_summary(self):
        summary = os.path.join(self.config['annotate_dest_dir'], 'summary')
        with open(summary, 'w') as f:
            f.write('total period: %d\n\n' % self.period)
            dso_periods = sorted(self.dso_periods.values(),
                                 key=lambda x: x.period.acc_period, reverse=True)
            for dso_period in dso_periods:
                f.write('dso %s: %s\n' % (dso_period.dso_name,
                                          self._get_percentage_str(dso_period.period)))
            f.write('\n')

            file_periods = sorted(self.file_periods.values(),
                                  key=lambda x: x.period.acc_period, reverse=True)
            for file_period in file_periods:
                f.write('file %s: %s\n' % (file_period.file,
                                           self._get_percentage_str(file_period.period)))
            for file_period in file_periods:
                f.write('\n\n%s: %s\n' % (file_period.file,
                                          self._get_percentage_str(file_period.period)))
                values = []
                for func_name in file_period.function_dict.keys():
                    func_start_line, period = file_period.function_dict[func_name]
                    values.append((func_name, func_start_line, period))
                values = sorted(values, key=lambda x: x[2].acc_period, reverse=True)
                for value in values:
                    f.write('\tfunction (%s): line %d, %s\n' % (
                        value[0], value[1], self._get_percentage_str(value[2])))
                f.write('\n')
                for line in sorted(file_period.line_dict.keys()):
                    f.write('\tline %d: %s\n' % (
                        line, self._get_percentage_str(file_period.line_dict[line])))


    def _get_percentage_str(self, period, short=False):
        s = 'acc_p: %f%%, p: %f%%' if short else 'accumulated_period: %f%%, period: %f%%'
        return s % self._get_percentage(period)


    def _get_percentage(self, period):
        if self.period == 0:
            return (0, 0)
        acc_p = 100.0 * period.acc_period / self.period
        p = 100.0 * period.period / self.period
        return (acc_p, p)


    def _annotate_files(self):
        """Annotate Source files: add acc_period/period for each source file.
           1. Annotate java source files, which have $JAVA_SRC_ROOT prefix.
           2. Annotate c++ source files.
        """
        dest_dir = self.config['annotate_dest_dir']
        for key in self.file_periods:
            from_path = key
            if not os.path.isfile(from_path):
                log_warning("can't find source file for path %s" % from_path)
                continue
            if from_path.startswith('/'):
                to_path = os.path.join(dest_dir, from_path[1:])
            elif is_windows() and ':\\' in from_path:
                to_path = os.path.join(dest_dir, from_path.replace(':\\', os.sep))
            else:
                to_path = os.path.join(dest_dir, from_path)
            is_java = from_path.endswith('.java')
            self._annotate_file(from_path, to_path, self.file_periods[key], is_java)


    def _annotate_file(self, from_path, to_path, file_period, is_java):
        """Annotate a source file.

        Annotate a source file in three steps:
          1. In the first line, show periods of this file.
          2. For each function, show periods of this function.
          3. For each line not hitting the same line as functions, show
             line periods.
        """
        log_info('annotate file %s' % from_path)
        with open(from_path, 'r') as rf:
            lines = rf.readlines()

        annotates = {}
        for line in file_period.line_dict.keys():
            annotates[line] = self._get_percentage_str(file_period.line_dict[line], True)
        for func_name in file_period.function_dict.keys():
            func_start_line, period = file_period.function_dict[func_name]
            if func_start_line == -1:
                continue
            line = func_start_line - 1 if is_java else func_start_line
            annotates[line] = '[func] ' + self._get_percentage_str(period, True)
        annotates[1] = '[file] ' + self._get_percentage_str(file_period.period, True)

        max_annotate_cols = 0
        for key in annotates:
            max_annotate_cols = max(max_annotate_cols, len(annotates[key]))

        empty_annotate = ' ' * (max_annotate_cols + 6)

        dirname = os.path.dirname(to_path)
        if not os.path.isdir(dirname):
            os.makedirs(dirname)
        with open(to_path, 'w') as wf:
            for line in range(1, len(lines) + 1):
                annotate = annotates.get(line)
                if annotate is None:
                    if not lines[line-1].strip():
                        annotate = ''
                    else:
                        annotate = empty_annotate
                else:
                    annotate = '/* ' + annotate + (
                        ' ' * (max_annotate_cols - len(annotate))) + ' */'
                wf.write(annotate)
                wf.write(lines[line-1])

def main():
    parser = argparse.ArgumentParser(description="""
        Annotate source files based on profiling data. It reads line information from binary_cache
        generated by app_profiler.py or binary_cache_builder.py, and generate annotated source
        files in annotated_files directory.""")
    parser.add_argument('-i', '--perf_data_list', nargs='+', action='append', help="""
        The paths of profiling data. Default is perf.data.""")
    parser.add_argument('-s', '--source_dirs', type=extant_dir, nargs='+', action='append', help="""
        Directories to find source files.""")
    parser.add_argument('--comm', nargs='+', action='append', help="""
        Use samples only in threads with selected names.""")
    parser.add_argument('--pid', nargs='+', action='append', help="""
        Use samples only in processes with selected process ids.""")
    parser.add_argument('--tid', nargs='+', action='append', help="""
        Use samples only in threads with selected thread ids.""")
    parser.add_argument('--dso', nargs='+', action='append', help="""
        Use samples only in selected binaries.""")
    parser.add_argument('--ndk_path', type=extant_dir, help='Set the path of a ndk release.')

    args = parser.parse_args()
    config = {}
    config['perf_data_list'] = flatten_arg_list(args.perf_data_list)
    if not config['perf_data_list']:
        config['perf_data_list'].append('perf.data')
    config['source_dirs'] = flatten_arg_list(args.source_dirs)
    config['comm_filters'] = flatten_arg_list(args.comm)
    config['pid_filters'] = flatten_arg_list(args.pid)
    config['tid_filters'] = flatten_arg_list(args.tid)
    config['dso_filters'] = flatten_arg_list(args.dso)
    config['ndk_path'] = args.ndk_path

    annotator = SourceFileAnnotator(config)
    annotator.annotate()
    log_info('annotate finish successfully, please check result in annotated_files/.')

if __name__ == '__main__':
    main()
