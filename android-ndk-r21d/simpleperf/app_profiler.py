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

"""app_profiler.py: Record cpu profiling data of an android app or native program.

    It downloads simpleperf on device, uses it to collect profiling data on the selected app,
    and pulls profiling data and related binaries on host.
"""

from __future__ import print_function
import argparse
import os
import os.path
import subprocess
import sys
import time

from utils import AdbHelper, bytes_to_str, extant_dir, get_script_dir, get_target_binary_path
from utils import log_debug, log_info, log_exit, ReadElf, remove, set_log_level, str_to_bytes

NATIVE_LIBS_DIR_ON_DEVICE = '/data/local/tmp/native_libs/'

class HostElfEntry(object):
    """Represent a native lib on host in NativeLibDownloader."""
    def __init__(self, path, name, score):
        self.path = path
        self.name = name
        self.score = score

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return '[path: %s, name %s, score %s]' % (self.path, self.name, self.score)


class NativeLibDownloader(object):
    """Download native libs on device.

    1. Collect info of all native libs in the native_lib_dir on host.
    2. Check the available native libs in /data/local/tmp/native_libs on device.
    3. Sync native libs on device.
    """
    def __init__(self, ndk_path, device_arch, adb):
        self.adb = adb
        self.readelf = ReadElf(ndk_path)
        self.device_arch = device_arch
        self.need_archs = self._get_need_archs()
        self.host_build_id_map = {}  # Map from build_id to HostElfEntry.
        self.device_build_id_map = {}  # Map from build_id to relative_path on device.
        self.name_count_map = {}  # Used to give a unique name for each library.
        self.dir_on_device = NATIVE_LIBS_DIR_ON_DEVICE
        self.build_id_list_file = 'build_id_list'

    def _get_need_archs(self):
        """Return the archs of binaries needed on device."""
        if self.device_arch == 'arm64':
            return ['arm', 'arm64']
        if self.device_arch == 'arm':
            return ['arm']
        if self.device_arch == 'x86_64':
            return ['x86', 'x86_64']
        if self.device_arch == 'x86':
            return ['x86']
        return []

    def collect_native_libs_on_host(self, native_lib_dir):
        self.host_build_id_map.clear()
        for root, _, files in os.walk(native_lib_dir):
            for name in files:
                if not name.endswith('.so'):
                    continue
                self.add_native_lib_on_host(os.path.join(root, name), name)

    def add_native_lib_on_host(self, path, name):
        build_id = self.readelf.get_build_id(path)
        if not build_id:
            return
        arch = self.readelf.get_arch(path)
        if arch not in self.need_archs:
            return
        sections = self.readelf.get_sections(path)
        score = 0
        if '.debug_info' in sections:
            score = 3
        elif '.gnu_debugdata' in sections:
            score = 2
        elif '.symtab' in sections:
            score = 1
        entry = self.host_build_id_map.get(build_id)
        if entry:
            if entry.score < score:
                entry.path = path
                entry.score = score
        else:
            repeat_count = self.name_count_map.get(name, 0)
            self.name_count_map[name] = repeat_count + 1
            unique_name = name if repeat_count == 0 else name + '_' + str(repeat_count)
            self.host_build_id_map[build_id] = HostElfEntry(path, unique_name, score)

    def collect_native_libs_on_device(self):
        self.device_build_id_map.clear()
        self.adb.check_run(['shell', 'mkdir', '-p', self.dir_on_device])
        if os.path.exists(self.build_id_list_file):
            os.remove(self.build_id_list_file)
        result, output = self.adb.run_and_return_output(['shell', 'ls', self.dir_on_device])
        if not result:
            return
        file_set = set(output.strip().split())
        if self.build_id_list_file not in file_set:
            return
        self.adb.run(['pull', self.dir_on_device + self.build_id_list_file])
        if os.path.exists(self.build_id_list_file):
            with open(self.build_id_list_file, 'rb') as fh:
                for line in fh.readlines():
                    line = bytes_to_str(line).strip()
                    items = line.split('=')
                    if len(items) == 2:
                        build_id, filename = items
                        if filename in file_set:
                            self.device_build_id_map[build_id] = filename
            remove(self.build_id_list_file)

    def sync_natives_libs_on_device(self):
        # Push missing native libs on device.
        for build_id in self.host_build_id_map:
            if build_id not in self.device_build_id_map:
                entry = self.host_build_id_map[build_id]
                self.adb.check_run(['push', entry.path, self.dir_on_device + entry.name])
        # Remove native libs not exist on host.
        for build_id in self.device_build_id_map:
            if build_id not in self.host_build_id_map:
                name = self.device_build_id_map[build_id]
                self.adb.run(['shell', 'rm', self.dir_on_device + name])
        # Push new build_id_list on device.
        with open(self.build_id_list_file, 'wb') as fh:
            for build_id in self.host_build_id_map:
                s = str_to_bytes('%s=%s\n' % (build_id, self.host_build_id_map[build_id].name))
                fh.write(s)
        self.adb.check_run(['push', self.build_id_list_file,
                            self.dir_on_device + self.build_id_list_file])
        os.remove(self.build_id_list_file)


class ProfilerBase(object):
    """Base class of all Profilers."""
    def __init__(self, args):
        self.args = args
        self.adb = AdbHelper(enable_switch_to_root=not args.disable_adb_root)
        self.is_root_device = self.adb.switch_to_root()
        self.android_version = self.adb.get_android_version()
        if self.android_version < 7:
            log_exit("""app_profiler.py isn't supported on Android < N, please switch to use
                        simpleperf binary directly.""")
        self.device_arch = self.adb.get_device_arch()
        self.record_subproc = None

    def profile(self):
        log_info('prepare profiling')
        self.prepare()
        log_info('start profiling')
        self.start()
        self.wait_profiling()
        log_info('collect profiling data')
        self.collect_profiling_data()
        log_info('profiling is finished.')

    def prepare(self):
        """Prepare recording. """
        self.download_simpleperf()
        if self.args.native_lib_dir:
            self.download_libs()

    def download_simpleperf(self):
        simpleperf_binary = get_target_binary_path(self.device_arch, 'simpleperf')
        self.adb.check_run(['push', simpleperf_binary, '/data/local/tmp'])
        self.adb.check_run(['shell', 'chmod', 'a+x', '/data/local/tmp/simpleperf'])

    def download_libs(self):
        downloader = NativeLibDownloader(self.args.ndk_path, self.device_arch, self.adb)
        downloader.collect_native_libs_on_host(self.args.native_lib_dir)
        downloader.collect_native_libs_on_device()
        downloader.sync_natives_libs_on_device()

    def start(self):
        raise NotImplementedError

    def start_profiling(self, target_args):
        """Start simpleperf reocrd process on device."""
        args = ['/data/local/tmp/simpleperf', 'record', '-o', '/data/local/tmp/perf.data',
                self.args.record_options]
        if self.adb.run(['shell', 'ls', NATIVE_LIBS_DIR_ON_DEVICE, '>/dev/null', '2>&1']):
            args += ['--symfs', NATIVE_LIBS_DIR_ON_DEVICE]
        args += ['--log', self.args.log]
        args += target_args
        adb_args = [self.adb.adb_path, 'shell'] + args
        log_info('run adb cmd: %s' % adb_args)
        self.record_subproc = subprocess.Popen(adb_args)

    def wait_profiling(self):
        """Wait until profiling finishes, or stop profiling when user presses Ctrl-C."""
        returncode = None
        try:
            returncode = self.record_subproc.wait()
        except KeyboardInterrupt:
            self.stop_profiling()
            self.record_subproc = None
            # Don't check return value of record_subproc. Because record_subproc also
            # receives Ctrl-C, and always returns non-zero.
            returncode = 0
        log_debug('profiling result [%s]' % (returncode == 0))
        if returncode != 0:
            log_exit('Failed to record profiling data.')

    def stop_profiling(self):
        """Stop profiling by sending SIGINT to simpleperf, and wait until it exits
           to make sure perf.data is completely generated."""
        has_killed = False
        while True:
            (result, _) = self.adb.run_and_return_output(['shell', 'pidof', 'simpleperf'])
            if not result:
                break
            if not has_killed:
                has_killed = True
                self.adb.run_and_return_output(['shell', 'pkill', '-l', '2', 'simpleperf'])
            time.sleep(1)

    def collect_profiling_data(self):
        self.adb.check_run_and_return_output(['pull', '/data/local/tmp/perf.data',
                                              self.args.perf_data_path])
        if not self.args.skip_collect_binaries:
            binary_cache_args = [sys.executable,
                                 os.path.join(get_script_dir(), 'binary_cache_builder.py')]
            binary_cache_args += ['-i', self.args.perf_data_path, '--log', self.args.log]
            if self.args.native_lib_dir:
                binary_cache_args += ['-lib', self.args.native_lib_dir]
            if self.args.disable_adb_root:
                binary_cache_args += ['--disable_adb_root']
            if self.args.ndk_path:
                binary_cache_args += ['--ndk_path', self.args.ndk_path]
            subprocess.check_call(binary_cache_args)


class AppProfiler(ProfilerBase):
    """Profile an Android app."""
    def prepare(self):
        super(AppProfiler, self).prepare()
        if self.args.compile_java_code:
            self.compile_java_code()

    def compile_java_code(self):
        self.kill_app_process()
        # Fully compile Java code on Android >= N.
        self.adb.set_property('debug.generate-debug-info', 'true')
        self.adb.check_run(['shell', 'cmd', 'package', 'compile', '-f', '-m', 'speed',
                            self.args.app])

    def kill_app_process(self):
        if self.find_app_process():
            self.adb.check_run(['shell', 'am', 'force-stop', self.args.app])
            count = 0
            while True:
                time.sleep(1)
                pid = self.find_app_process()
                if not pid:
                    break
                # When testing on Android N, `am force-stop` sometimes can't kill
                # com.example.simpleperf.simpleperfexampleofkotlin. So use kill when this happens.
                count += 1
                if count >= 3:
                    self.run_in_app_dir(['kill', '-9', str(pid)])

    def find_app_process(self):
        result, output = self.adb.run_and_return_output(['shell', 'pidof', self.args.app])
        return int(output) if result else None

    def run_in_app_dir(self, args):
        if self.is_root_device:
            adb_args = ['shell', 'cd /data/data/' + self.args.app + ' && ' + (' '.join(args))]
        else:
            adb_args = ['shell', 'run-as', self.args.app] + args
        return self.adb.run_and_return_output(adb_args, log_output=False)

    def start(self):
        if self.args.activity or self.args.test:
            self.kill_app_process()
        self.start_profiling(['--app', self.args.app])
        if self.args.activity:
            self.start_activity()
        elif self.args.test:
            self.start_test()
        # else: no need to start an activity or test.

    def start_activity(self):
        activity = self.args.app + '/' + self.args.activity
        result = self.adb.run(['shell', 'am', 'start', '-n', activity])
        if not result:
            self.record_subproc.terminate()
            log_exit("Can't start activity %s" % activity)

    def start_test(self):
        runner = self.args.app + '/androidx.test.runner.AndroidJUnitRunner'
        result = self.adb.run(['shell', 'am', 'instrument', '-e', 'class',
                               self.args.test, runner])
        if not result:
            self.record_subproc.terminate()
            log_exit("Can't start instrumentation test  %s" % self.args.test)


class NativeProgramProfiler(ProfilerBase):
    """Profile a native program."""
    def start(self):
        pid = int(self.adb.check_run_and_return_output(['shell', 'pidof',
                                                        self.args.native_program]))
        self.start_profiling(['-p', str(pid)])


class NativeCommandProfiler(ProfilerBase):
    """Profile running a native command."""
    def start(self):
        self.start_profiling([self.args.cmd])


class NativeProcessProfiler(ProfilerBase):
    """Profile processes given their pids."""
    def start(self):
        self.start_profiling(['-p', ','.join(self.args.pid)])


class NativeThreadProfiler(ProfilerBase):
    """Profile threads given their tids."""
    def start(self):
        self.start_profiling(['-t', ','.join(self.args.tid)])


class SystemWideProfiler(ProfilerBase):
    """Profile system wide."""
    def start(self):
        self.start_profiling(['-a'])


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)

    target_group = parser.add_argument_group(title='Select profiling target'
                                            ).add_mutually_exclusive_group(required=True)
    target_group.add_argument('-p', '--app', help="""Profile an Android app, given the package name.
                              Like `-p com.example.android.myapp`.""")

    target_group.add_argument('-np', '--native_program', help="""Profile a native program running on
                              the Android device. Like `-np surfaceflinger`.""")

    target_group.add_argument('-cmd', help="""Profile running a command on the Android device.
                              Like `-cmd "pm -l"`.""")

    target_group.add_argument('--pid', nargs='+', help="""Profile native processes running on device
                              given their process ids.""")

    target_group.add_argument('--tid', nargs='+', help="""Profile native threads running on device
                              given their thread ids.""")

    target_group.add_argument('--system_wide', action='store_true', help="""Profile system wide.""")

    app_target_group = parser.add_argument_group(title='Extra options for profiling an app')
    app_target_group.add_argument('--compile_java_code', action='store_true', help="""Used with -p.
                                  On Android N and Android O, we need to compile Java code into
                                  native instructions to profile Java code. Android O also needs
                                  wrap.sh in the apk to use the native instructions.""")

    app_start_group = app_target_group.add_mutually_exclusive_group()
    app_start_group.add_argument('-a', '--activity', help="""Used with -p. Profile the launch time
                                 of an activity in an Android app. The app will be started or
                                 restarted to run the activity. Like `-a .MainActivity`.""")

    app_start_group.add_argument('-t', '--test', help="""Used with -p. Profile the launch time of an
                                 instrumentation test in an Android app. The app will be started or
                                 restarted to run the instrumentation test. Like
                                 `-t test_class_name`.""")

    record_group = parser.add_argument_group('Select recording options')
    record_group.add_argument('-r', '--record_options',
                              default='-e task-clock:u -f 1000 -g --duration 10', help="""Set
                              recording options for `simpleperf record` command. Use
                              `run_simpleperf_on_device.py record -h` to see all accepted options.
                              Default is "-e task-clock:u -f 1000 -g --duration 10".""")

    record_group.add_argument('-lib', '--native_lib_dir', type=extant_dir,
                              help="""When profiling an Android app containing native libraries,
                                      the native libraries are usually stripped and lake of symbols
                                      and debug information to provide good profiling result. By
                                      using -lib, you tell app_profiler.py the path storing
                                      unstripped native libraries, and app_profiler.py will search
                                      all shared libraries with suffix .so in the directory. Then
                                      the native libraries will be downloaded on device and
                                      collected in build_cache.""")

    record_group.add_argument('-o', '--perf_data_path', default='perf.data',
                              help='The path to store profiling data. Default is perf.data.')

    record_group.add_argument('-nb', '--skip_collect_binaries', action='store_true',
                              help="""By default we collect binaries used in profiling data from
                                      device to binary_cache directory. It can be used to annotate
                                      source code and disassembly. This option skips it.""")

    other_group = parser.add_argument_group('Other options')
    other_group.add_argument('--ndk_path', type=extant_dir,
                             help="""Set the path of a ndk release. app_profiler.py needs some
                                     tools in ndk, like readelf.""")

    other_group.add_argument('--disable_adb_root', action='store_true',
                             help="""Force adb to run in non root mode. By default, app_profiler.py
                                     will try to switch to root mode to be able to profile released
                                     Android apps.""")
    other_group.add_argument(
        '--log', choices=['debug', 'info', 'warning'], default='info', help='set log level')

    def check_args(args):
        if (not args.app) and (args.compile_java_code or args.activity or args.test):
            log_exit('--compile_java_code, -a, -t can only be used when profiling an Android app.')

    args = parser.parse_args()
    set_log_level(args.log)
    check_args(args)
    if args.app:
        profiler = AppProfiler(args)
    elif args.native_program:
        profiler = NativeProgramProfiler(args)
    elif args.cmd:
        profiler = NativeCommandProfiler(args)
    elif args.pid:
        profiler = NativeProcessProfiler(args)
    elif args.tid:
        profiler = NativeThreadProfiler(args)
    elif args.system_wide:
        profiler = SystemWideProfiler(args)
    profiler.profile()

if __name__ == '__main__':
    main()
