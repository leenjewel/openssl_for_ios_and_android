#!/usr/bin/env python
#
# Copyright (C) 2017 The Android Open Source Project
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
# pylint: disable=not-callable, relative-import

# Note that adding top-level imports is discouraged unless they're guaranteed to
# be used. Unnecessary imports eat a measurable number of cycles of this
# wrapper.
import os
import sys

BISECT_STAGE = os.environ.get('BISECT_STAGE')
# We do not need bisect functionality with Goma and clang.
# Goma server does not have bisect_driver, so we only import
# bisect_driver when needed. See http://b/34862041
# We should be careful when doing imports because of Goma.
if BISECT_STAGE:
    import bisect_driver

DEFAULT_BISECT_DIR = os.path.expanduser('~/ANDROID_BISECT')
BISECT_DIR = os.environ.get('BISECT_DIR') or DEFAULT_BISECT_DIR
STDERR_REDIRECT_KEY = 'ANDROID_LLVM_STDERR_REDIRECT'
PREBUILT_COMPILER_PATH_KEY = 'ANDROID_LLVM_PREBUILT_COMPILER_PATH'
DISABLED_WARNINGS_KEY = 'ANDROID_LLVM_FALLBACK_DISABLED_WARNINGS'


def ProcessArgFile(arg_file):
    import shlex

    args = []
    # Read in entire file at once and parse as if in shell
    with open(arg_file, 'rb') as f:
        args.extend(shlex.split(f.read()))
    return args


def write_log(path, command, log):
    import errno
    import fcntl
    import time

    with open(path, 'a+') as f:
        while True:
            try:
                fcntl.flock(f, fcntl.LOCK_EX | fcntl.LOCK_NB)
                break
            except IOError as e:
                if e.errno == errno.EAGAIN or e.errno == errno.EACCES:
                    time.sleep(0.5)
        f.write('==================COMMAND:====================\n')
        f.write(' '.join(command) + '\n\n')
        f.write(log)
        f.write('==============================================\n\n')


class CompilerWrapper(object):

    def __init__(self, argv):
        self.argv0_current = argv[0]
        self.args = argv[1:]
        self.execargs = []
        self.real_compiler = None
        self.argv0 = None
        self.append_flags = []
        self.prepend_flags = []
        self.custom_flags = {'--gomacc-path': None}

    def set_real_compiler(self):
        """Find the real compiler with the absolute path."""
        compiler_path = os.path.dirname(self.argv0_current)
        if os.path.islink(__file__):
            compiler = os.path.basename(os.readlink(__file__))
        else:
            compiler = os.path.basename(os.path.abspath(__file__))
        self.real_compiler = os.path.join(compiler_path, compiler + '.real')
        self.argv0 = self.real_compiler

    def process_gomacc_command(self):
        """Return the gomacc command if '--gomacc-path' is set."""
        gomacc = self.custom_flags['--gomacc-path']
        if gomacc and os.path.isfile(gomacc):
            self.argv0 = gomacc
            self.execargs += [gomacc]

    def parse_custom_flags(self):
        i = 0
        args = []
        while i < len(self.args):
            if self.args[i] in self.custom_flags:
                if i >= len(self.args) - 1:
                    sys.exit('The value of {} is not set.'.format(self.args[i]))
                self.custom_flags[self.args[i]] = self.args[i + 1]
                i = i + 2
            else:
                args.append(self.args[i])
                i = i + 1
        self.args = args

    def add_flags(self):
        self.args = self.prepend_flags + self.args + self.append_flags

    def prepare_compiler_args(self, enable_fallback=False):
        self.set_real_compiler()
        self.parse_custom_flags()
        # Goma should not be enabled for new prebuilt.
        if not enable_fallback:
            self.process_gomacc_command()
        self.add_flags()
        self.execargs += [self.real_compiler] + self.args

    def exec_clang_with_fallback(self):
        import subprocess

        extra_args_begin = len(self.execargs)

        # We only want to pass extra flags to clang and clang++.
        if os.path.basename(__file__) in ['clang', 'clang++']:
            # We may introduce some new warnings after rebasing and we need to
            # disable them before we fix those warnings.
            disabled_warnings_env = os.environ.get(DISABLED_WARNINGS_KEY, '')
            disabled_warnings = disabled_warnings_env.split(' ')
            self.execargs += ['-fno-color-diagnostics'] + disabled_warnings

        p = subprocess.Popen(self.execargs, stderr=subprocess.PIPE)
        (_, err) = p.communicate()
        sys.stderr.write(err)
        if p.returncode != 0:
            redirect_path = os.environ[STDERR_REDIRECT_KEY]
            write_log(redirect_path, self.execargs, err)
            fallback_arg0 = os.path.join(os.environ[PREBUILT_COMPILER_PATH_KEY],
                                         os.path.basename(__file__))

            # Delete PREBUILT_COMPILER_PATH_KEY so the fallback doesn't keep
            # calling itself in case of an error.
            del os.environ[PREBUILT_COMPILER_PATH_KEY]

            # Strip extra args added (from DISABLED_WARNINGS_KEY) for clang and
            # clang++ above.  They may not be recognized by the fallback clang.
            self.execargs = self.execargs[:extra_args_begin]

            os.execv(fallback_arg0, [fallback_arg0] + self.execargs[1:])

    def invoke_compiler(self):
        enable_fallback = PREBUILT_COMPILER_PATH_KEY in os.environ
        self.prepare_compiler_args(enable_fallback)
        if enable_fallback:
            self.exec_clang_with_fallback()
        else:
            os.execv(self.argv0, self.execargs)

    def bisect(self):
        self.prepare_compiler_args()
        # Handle @file argument syntax with compiler
        idx = 0
        # The length of self.execargs can be changed during the @file argument
        # expansion, so we need to use while loop instead of for loop.
        while idx < len(self.execargs):
            if self.execargs[idx][0] == '@':
                args_in_file = ProcessArgFile(self.execargs[idx][1:])
                self.execargs = self.execargs[0:idx] + args_in_file +\
                        self.execargs[idx + 1:]
                # Skip update of idx, since we want to recursively expand
                # response files.
            else:
                idx = idx + 1
        bisect_driver.bisect_driver(BISECT_STAGE, BISECT_DIR, self.execargs)


def main(argv):
    cw = CompilerWrapper(argv)
    if BISECT_STAGE and BISECT_STAGE in bisect_driver.VALID_MODES\
            and '-o' in argv:
        cw.bisect()
    else:
        cw.invoke_compiler()


if __name__ == '__main__':
    main(sys.argv)
