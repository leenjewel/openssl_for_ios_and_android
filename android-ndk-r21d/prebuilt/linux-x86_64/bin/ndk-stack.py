#!/usr/bin/env python
#
# Copyright (C) 2018 The Android Open Source Project
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
"""Symbolizes stack traces from logcat.
See https://developer.android.com/ndk/guides/ndk-stack for more information.
"""

from __future__ import print_function

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile

EXE_SUFFIX = '.exe' if os.name == 'nt' else ''


class TmpDir(object):
    """Manage temporary directory creation."""

    def __init__(self):
        self._tmp_dir = None

    def delete(self):
        if self._tmp_dir:
            shutil.rmtree(self._tmp_dir)

    def get_directory(self):
        if not self._tmp_dir:
            self._tmp_dir = tempfile.mkdtemp()
        return self._tmp_dir


def get_ndk_paths():
    """Parse and find all of the paths of the ndk

    Returns: Three values:
             Full path to the root of the ndk install.
             Full path to the ndk bin directory where this executable lives.
             The platform name (eg linux-x86_64).
    """

    # ndk-stack is installed to $NDK/prebuilt/<platform>/bin, so from
    # `android-ndk-r18/prebuilt/linux-x86_64/bin/ndk-stack`...
    # ...get `android-ndk-r18/`:
    ndk_bin = os.path.dirname(os.path.realpath(__file__))
    ndk_root = os.path.abspath(os.path.join(ndk_bin, '../../..'))
    # ...get `linux-x86_64`:
    ndk_host_tag = os.path.basename(
        os.path.abspath(os.path.join(ndk_bin, '../')))
    return (ndk_root, ndk_bin, ndk_host_tag)


def find_llvm_symbolizer(ndk_root, ndk_bin, ndk_host_tag):
    """Finds the NDK llvm-symbolizer(1) binary.

    Returns: An absolute path to llvm-symbolizer(1).
    """

    llvm_symbolizer = 'llvm-symbolizer' + EXE_SUFFIX
    path = os.path.join(ndk_root, 'toolchains', 'llvm', 'prebuilt',
                        ndk_host_tag, 'bin', llvm_symbolizer)
    if os.path.exists(path):
        return path

    # Okay, maybe we're a standalone toolchain? (https://github.com/android-ndk/ndk/issues/931)
    # In that case, llvm-symbolizer and ndk-stack are conveniently in
    # the same directory...
    path = os.path.abspath(os.path.join(ndk_bin, llvm_symbolizer))
    if os.path.exists(path):
        return path
    raise OSError('Unable to find llvm-symbolizer')


def find_readelf(ndk_root, ndk_bin, ndk_host_tag):
    """Finds the NDK readelf(1) binary.

    Returns: An absolute path to readelf(1).
    """

    readelf = 'readelf' + EXE_SUFFIX
    m = re.match('^[^-]+-(.*)', ndk_host_tag)
    if m:
        # Try as if this is not a standalone install.
        arch = m.group(1)
        if arch == 'arm':
            platform_dir = arch + '-linux-androideabi'
        else:
            platform_dir = arch + '-linux-android'
        path = os.path.join(ndk_root, 'toolchains', 'llvm', 'prebuilt',
                            ndk_host_tag, platform_dir, 'bin', readelf)
        if os.path.exists(path):
            return path

    # Might be a standalone toolchain, find the first readelf available,
    # any should work.
    arches = [
        'aarch64-linux-android', 'arm-linux-androideabi',
        'x86_64-linux-android', 'i686-linux-android'
    ]
    for arch in arches:
        path = os.path.normpath(
            os.path.join(ndk_bin, '..', arch, 'bin', readelf))
        if os.path.exists(path):
            return path
    return None


def get_build_id(readelf_path, elf_file):
    """Get the GNU build id note from an elf file.

    Returns: The build id found or None if there is no build id or the
             readelf path does not exist.
    """

    try:
        output = subprocess.check_output([readelf_path, '-n', elf_file])
        m = re.search(r'Build ID:\s+([0-9a-f]+)', output.decode())
        if not m:
            return None
        return m.group(1)
    except subprocess.CalledProcessError:
        return None


def get_zip_info_from_offset(zip_file, offset):
    """Get the ZipInfo object from a zip file.

    Returns: A ZipInfo object found at the 'offset' into the zip file.
             Returns None if no file can be found at the given 'offset'.
    """

    file_size = os.stat(zip_file.filename).st_size
    if offset >= file_size:
        return None

    infos = zip_file.infolist()
    if not infos or offset < infos[0].header_offset:
        return None

    for i in range(1, len(infos)):
        prev_info = infos[i - 1]
        cur_offset = infos[i].header_offset
        if offset >= prev_info.header_offset and offset < cur_offset:
            zip_info = prev_info
            return zip_info
    zip_info = infos[len(infos) - 1]
    if offset < zip_info.header_offset:
        return None
    return zip_info


class FrameInfo(object):
    """A class to represent the data in a single backtrace frame.

    Attributes:
      num: The string representing the frame number (eg #01).
      pc: The relative program counter for the frame.
      elf_file: The file or map name in which the relative pc resides.
      container_file: The name of the file that contains the elf_file.
                      For example, an entry like GoogleCamera.apk!libsome.so
                      would set container_file to GoogleCamera.apk and
                      set elf_file to libsome.so. Set to None if no ! found.
      offset: The offset into the file at which this library was mapped.
              Set to None if no offset found.
      build_id: The Gnu build id note parsed from the frame information.
                Set to None if no build id found.
      tail: The part of the line after the program counter.
    """

    # See unwindstack::FormatFrame in libunwindstack.
    # We're deliberately very loose because NDK users are likely to be
    # looking at crashes on ancient OS releases.
    # TODO: support asan stacks too?
    _line_re = re.compile(r'.* +(#[0-9]+) +pc ([0-9a-f]+) +(([^ ]+).*)')
    _lib_re = re.compile(r'([^\!]+)\!(.+)')
    _offset_re = re.compile(r'\(offset\s+(0x[0-9a-f]+)\)')
    _build_id_re = re.compile(r'\(BuildId:\s+([0-9a-f]+)\)')

    @classmethod
    def from_line(cls, line):
        m = FrameInfo._line_re.match(line)
        if not m:
            return None
        return cls(*m.group(1, 2, 3, 4))

    def __init__(self, num, pc, tail, elf_file):
        self.num = num
        self.pc = pc
        self.tail = tail
        self.elf_file = elf_file
        m = FrameInfo._lib_re.match(self.elf_file)
        if m:
            self.container_file = m.group(1)
            self.elf_file = m.group(2)
            # Sometimes an entry like this will occur:
            #   #01 pc 0000abcd  /system/lib/lib/libc.so!libc.so (offset 0x1000)
            # In this case, no container file should be set.
            if os.path.basename(self.container_file) == os.path.basename(
                    self.elf_file):
                self.elf_file = self.container_file
                self.container_file = None
        else:
            self.container_file = None
        m = FrameInfo._offset_re.search(self.tail)
        if m:
            self.offset = int(m.group(1), 16)
        else:
            self.offset = None
        m = FrameInfo._build_id_re.search(self.tail)
        if m:
            self.build_id = m.group(1)
        else:
            self.build_id = None

    def verify_elf_file(self, readelf_path, elf_file_path, display_elf_path):
        """Verify if the elf file is valid.

        Returns: True if the elf file exists and build id matches (if it exists).
        """

        if not os.path.exists(elf_file_path):
            return False
        if readelf_path and self.build_id:
            build_id = get_build_id(readelf_path, elf_file_path)
            if self.build_id != build_id:
                print(
                    'WARNING: Mismatched build id for %s' % (display_elf_path))
                print('WARNING:   Expected %s' % (self.build_id))
                print('WARNING:   Found    %s' % (build_id))
                return False
        return True

    def get_elf_file(self, symbol_dir, readelf_path, tmp_dir):
        """Get the path to the elf file represented by this frame.

        Returns: The path to the elf file if it is valid, or None if
                 no valid elf file can be found. If the file has to be
                 extracted from an apk, the elf file will be placed in
                 tmp_dir.
        """

        elf_file = os.path.basename(self.elf_file)
        if self.container_file:
            # This matches a file format such as Base.apk!libsomething.so
            # so see if we can find libsomething.so in the symbol directory.
            elf_file_path = os.path.join(symbol_dir, elf_file)
            if self.verify_elf_file(readelf_path, elf_file_path,
                                    elf_file_path):
                return elf_file_path

            apk_file_path = os.path.join(symbol_dir,
                                         os.path.basename(self.container_file))
            with zipfile.ZipFile(apk_file_path) as zip_file:
                zip_info = get_zip_info_from_offset(zip_file, self.offset)
                if not zip_info:
                    return None
                elf_file_path = zip_file.extract(zip_info,
                                                 tmp_dir.get_directory())
                display_elf_file = '%s!%s' % (apk_file_path, elf_file)
                if not self.verify_elf_file(readelf_path, elf_file_path,
                                            display_elf_file):
                    return None
                return elf_file_path
        elif elf_file[-4:] == '.apk':
            # This matches a stack line such as:
            #   #08 pc 00cbed9c  GoogleCamera.apk (offset 0x6e32000)
            apk_file_path = os.path.join(symbol_dir, elf_file)
            with zipfile.ZipFile(apk_file_path) as zip_file:
                zip_info = get_zip_info_from_offset(zip_file, self.offset)
                if not zip_info:
                    return None

                # Rewrite the output tail so that it goes from:
                #   GoogleCamera.apk ...
                # To:
                #   GoogleCamera.apk!libsomething.so ...
                index = self.tail.find(elf_file)
                if index != -1:
                    index += len(elf_file)
                    self.tail = (self.tail[0:index] + '!' + os.path.basename(
                        zip_info.filename) + self.tail[index:])
                elf_file = os.path.basename(zip_info.filename)
                elf_file_path = os.path.join(symbol_dir, elf_file)
                if self.verify_elf_file(readelf_path, elf_file_path,
                                        elf_file_path):
                    return elf_file_path

                elf_file_path = zip_file.extract(zip_info,
                                                 tmp_dir.get_directory())
                display_elf_path = '%s!%s' % (apk_file_path, elf_file)
                if not self.verify_elf_file(readelf_path, elf_file_path,
                                            display_elf_path):
                    return None
                return elf_file_path
        elf_file_path = os.path.join(symbol_dir, elf_file)
        if self.verify_elf_file(readelf_path, elf_file_path, elf_file_path):
            return elf_file_path
        return None


def main(argv):
    """"Program entry point."""
    parser = argparse.ArgumentParser(
        description='Symbolizes Android crashes.',
        epilog='See <https://developer.android.com/ndk/guides/ndk-stack>.')
    parser.add_argument(
        '-sym',
        '--sym',
        dest='symbol_dir',
        required=True,  # TODO: default to '.'?
        help='directory containing unstripped .so files')
    parser.add_argument(
        '-i',
        '-dump',
        '--dump',
        dest='input',
        default=sys.stdin,
        type=argparse.FileType('r'),
        help='input filename')
    args = parser.parse_args(argv)

    if not os.path.exists(args.symbol_dir):
        sys.exit('{} does not exist!\n'.format(args.symbol_dir))

    ndk_paths = get_ndk_paths()
    symbolize_cmd = [
        find_llvm_symbolizer(*ndk_paths), '--demangle', '--functions=linkage',
        '--inlining=true', '--use-symbol-table=true'
    ]
    readelf_path = find_readelf(*ndk_paths)

    symbolize_proc = None
    try:
        tmp_dir = TmpDir()
        symbolize_proc = subprocess.Popen(
            symbolize_cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        banner = '*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***'
        in_crash = False
        saw_frame = False
        for line in args.input:
            line = line.rstrip()

            if not in_crash:
                if banner in line:
                    in_crash = True
                    saw_frame = False
                    print('********** Crash dump: **********')
                continue

            for tag in ['Build fingerprint:', 'Abort message:']:
                if tag in line:
                    print(line[line.find(tag):])
                    continue

            frame_info = FrameInfo.from_line(line)
            if not frame_info:
                if saw_frame:
                    in_crash = False
                    print('Crash dump is completed\n')
                continue
            saw_frame = True

            try:
                elf_file = frame_info.get_elf_file(args.symbol_dir,
                                                   readelf_path, tmp_dir)
            except IOError:
                elf_file = None

            # Print a slightly different version of the stack trace line.
            # The original format:
            #      #00 pc 0007b350  /lib/bionic/libc.so (__strchr_chk+4)
            # becomes:
            #      #00 0x0007b350 /lib/bionic/libc.so (__strchr_chk+4)
            out_line = '%s 0x%s %s' % (frame_info.num, frame_info.pc,
                                       frame_info.tail)
            print(out_line)
            indent = (out_line.find('(') + 1) * ' '
            if not elf_file:
                continue
            value = '"%s" 0x%s\n' % (elf_file, frame_info.pc)
            symbolize_proc.stdin.write(value.encode())
            symbolize_proc.stdin.flush()
            while True:
                symbolizer_output = symbolize_proc.stdout.readline().rstrip()
                if not symbolizer_output:
                    break
                # TODO: rewrite file names base on a source path?
                print('%s%s' % (indent, symbolizer_output.decode()))
    finally:
        args.input.close()
        tmp_dir.delete()
        if symbolize_proc:
            symbolize_proc.stdin.close()
            symbolize_proc.stdout.close()
            symbolize_proc.kill()
            symbolize_proc.wait()


if __name__ == '__main__':
    main(sys.argv[1:])
