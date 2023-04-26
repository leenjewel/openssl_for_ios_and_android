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

"""binary_cache_builder.py: read perf.data, collect binaries needed by
    it, and put them in binary_cache.
"""

from __future__ import print_function
import argparse
import os
import os.path
import shutil

from simpleperf_report_lib import ReportLib
from utils import AdbHelper, extant_dir, extant_file, flatten_arg_list, log_info, log_warning
from utils import ReadElf, set_log_level

def is_jit_symfile(dso_name):
    return dso_name.split('/')[-1].startswith('TemporaryFile')

class BinaryCacheBuilder(object):
    """Collect all binaries needed by perf.data in binary_cache."""
    def __init__(self, ndk_path, disable_adb_root):
        self.adb = AdbHelper(enable_switch_to_root=not disable_adb_root)
        self.readelf = ReadElf(ndk_path)
        self.binary_cache_dir = 'binary_cache'
        if not os.path.isdir(self.binary_cache_dir):
            os.makedirs(self.binary_cache_dir)
        self.binaries = {}


    def build_binary_cache(self, perf_data_path, symfs_dirs):
        self._collect_used_binaries(perf_data_path)
        self.copy_binaries_from_symfs_dirs(symfs_dirs)
        self._pull_binaries_from_device()
        self._pull_kernel_symbols()


    def _collect_used_binaries(self, perf_data_path):
        """read perf.data, collect all used binaries and their build id (if available)."""
        # A dict mapping from binary name to build_id
        binaries = {}
        lib = ReportLib()
        lib.SetRecordFile(perf_data_path)
        lib.SetLogSeverity('error')
        while True:
            sample = lib.GetNextSample()
            if sample is None:
                lib.Close()
                break
            symbols = [lib.GetSymbolOfCurrentSample()]
            callchain = lib.GetCallChainOfCurrentSample()
            for i in range(callchain.nr):
                symbols.append(callchain.entries[i].symbol)

            for symbol in symbols:
                dso_name = symbol.dso_name
                if dso_name not in binaries:
                    if is_jit_symfile(dso_name):
                        continue
                    binaries[dso_name] = lib.GetBuildIdForPath(dso_name)
        self.binaries = binaries


    def copy_binaries_from_symfs_dirs(self, symfs_dirs):
        """collect all files in symfs_dirs."""
        if not symfs_dirs:
            return

        # It is possible that the path of the binary in symfs_dirs doesn't match
        # the one recorded in perf.data. For example, a file in symfs_dirs might
        # be "debug/arm/obj/armeabi-v7a/libsudo-game-jni.so", but the path in
        # perf.data is "/data/app/xxxx/lib/arm/libsudo-game-jni.so". So we match
        # binaries if they have the same filename (like libsudo-game-jni.so)
        # and same build_id.

        # Map from filename to binary paths.
        filename_dict = {}
        for binary in self.binaries:
            index = binary.rfind('/')
            filename = binary[index+1:]
            paths = filename_dict.get(filename)
            if paths is None:
                filename_dict[filename] = paths = []
            paths.append(binary)

        # Walk through all files in symfs_dirs, and copy matching files to build_cache.
        for symfs_dir in symfs_dirs:
            for root, _, files in os.walk(symfs_dir):
                for filename in files:
                    paths = filename_dict.get(filename)
                    if not paths:
                        continue
                    build_id = self._read_build_id(os.path.join(root, filename))
                    if not build_id:
                        continue
                    for binary in paths:
                        expected_build_id = self.binaries.get(binary)
                        if expected_build_id == build_id:
                            self._copy_to_binary_cache(os.path.join(root, filename),
                                                       expected_build_id, binary)
                            break


    def _copy_to_binary_cache(self, from_path, expected_build_id, target_file):
        if target_file[0] == '/':
            target_file = target_file[1:]
        target_file = target_file.replace('/', os.sep)
        target_file = os.path.join(self.binary_cache_dir, target_file)
        if not self._need_to_copy(from_path, target_file, expected_build_id):
            # The existing file in binary_cache can provide more information, so no need to copy.
            return
        target_dir = os.path.dirname(target_file)
        if not os.path.isdir(target_dir):
            os.makedirs(target_dir)
        log_info('copy to binary_cache: %s to %s' % (from_path, target_file))
        shutil.copy(from_path, target_file)


    def _need_to_copy(self, source_file, target_file, expected_build_id):
        if not os.path.isfile(target_file):
            return True
        if self._read_build_id(target_file) != expected_build_id:
            return True
        return self._get_file_stripped_level(source_file) < self._get_file_stripped_level(
            target_file)


    def _get_file_stripped_level(self, file_path):
        """Return stripped level of an ELF file. Larger value means more stripped."""
        sections = self.readelf.get_sections(file_path)
        if '.debug_line' in sections:
            return 0
        if '.symtab' in sections:
            return 1
        return 2


    def _pull_binaries_from_device(self):
        """pull binaries needed in perf.data to binary_cache."""
        for binary in self.binaries:
            build_id = self.binaries[binary]
            if not binary.startswith('/') or binary == "//anon" or binary.startswith("/dev/"):
                # [kernel.kallsyms] or unknown, or something we can't find binary.
                continue
            binary_cache_file = binary[1:].replace('/', os.sep)
            binary_cache_file = os.path.join(self.binary_cache_dir, binary_cache_file)
            self._check_and_pull_binary(binary, build_id, binary_cache_file)


    def _check_and_pull_binary(self, binary, expected_build_id, binary_cache_file):
        """If the binary_cache_file exists and has the expected_build_id, there
           is no need to pull the binary from device. Otherwise, pull it.
        """
        need_pull = True
        if os.path.isfile(binary_cache_file):
            need_pull = False
            if expected_build_id:
                build_id = self._read_build_id(binary_cache_file)
                if expected_build_id != build_id:
                    need_pull = True
        if need_pull:
            target_dir = os.path.dirname(binary_cache_file)
            if not os.path.isdir(target_dir):
                os.makedirs(target_dir)
            if os.path.isfile(binary_cache_file):
                os.remove(binary_cache_file)
            log_info('pull file to binary_cache: %s to %s' % (binary, binary_cache_file))
            self._pull_file_from_device(binary, binary_cache_file)
        else:
            log_info('use current file in binary_cache: %s' % binary_cache_file)


    def _read_build_id(self, file_path):
        """read build id of a binary on host."""
        return self.readelf.get_build_id(file_path)


    def _pull_file_from_device(self, device_path, host_path):
        if self.adb.run(['pull', device_path, host_path]):
            return True
        # In non-root device, we can't pull /data/app/XXX/base.odex directly.
        # Instead, we can first copy the file to /data/local/tmp, then pull it.
        filename = device_path[device_path.rfind('/')+1:]
        if (self.adb.run(['shell', 'cp', device_path, '/data/local/tmp']) and
                self.adb.run(['pull', '/data/local/tmp/' + filename, host_path])):
            self.adb.run(['shell', 'rm', '/data/local/tmp/' + filename])
            return True
        log_warning('failed to pull %s from device' % device_path)
        return False


    def _pull_kernel_symbols(self):
        file_path = os.path.join(self.binary_cache_dir, 'kallsyms')
        if os.path.isfile(file_path):
            os.remove(file_path)
        if self.adb.switch_to_root():
            self.adb.run(['shell', 'echo', '0', '>/proc/sys/kernel/kptr_restrict'])
            self.adb.run(['pull', '/proc/kallsyms', file_path])


def main():
    parser = argparse.ArgumentParser(description="""
        Pull binaries needed by perf.data from device to binary_cache directory.""")
    parser.add_argument('-i', '--perf_data_path', default='perf.data', type=extant_file, help="""
        The path of profiling data.""")
    parser.add_argument('-lib', '--native_lib_dir', type=extant_dir, nargs='+', help="""
        Path to find debug version of native shared libraries used in the app.""", action='append')
    parser.add_argument('--disable_adb_root', action='store_true', help="""
        Force adb to run in non root mode.""")
    parser.add_argument('--ndk_path', nargs=1, help='Find tools in the ndk path.')
    parser.add_argument(
        '--log', choices=['debug', 'info', 'warning'], default='info', help='set log level')
    args = parser.parse_args()
    set_log_level(args.log)
    ndk_path = None if not args.ndk_path else args.ndk_path[0]
    builder = BinaryCacheBuilder(ndk_path, args.disable_adb_root)
    symfs_dirs = flatten_arg_list(args.native_lib_dir)
    builder.build_binary_cache(args.perf_data_path, symfs_dirs)


if __name__ == '__main__':
    main()
