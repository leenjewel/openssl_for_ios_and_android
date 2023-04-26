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
"""Determines which sanitizers should be linked based on ldflags."""
from __future__ import print_function

import sys


def sanitizers_from_args(args):
    """Returns the sanitizers enabled by a given set of ldflags."""
    sanitizers = set()
    for arg in args:
        if arg.startswith('-fsanitize='):
            sanitizer_list = arg.partition('=')[2]
            sanitizers |= set(sanitizer_list.split(','))
        elif arg.startswith('-fno-sanitize='):
            sanitizer_list = arg.partition('=')[2]
            sanitizers -= set(sanitizer_list.split(','))
    return sorted(list(sanitizers))


def argv_to_module_arg_lists(args):
    """Converts module ldflags from argv format to per-module lists.

    Flags are passed to us in the following format:
        ['global flag', '--module', 'flag1', 'flag2', '--module', 'flag 3']

    These should be returned as a list for the global flags and a list of
    per-module lists, i.e.:
        ['global flag'], [['flag1', 'flag2'], ['flag1', 'flag3']]
    """
    modules = [[]]
    for arg in args:
        if arg == '--module':
            modules.append([])
        else:
            modules[-1].append(arg)
    return modules[0], modules[1:]


def main(argv, stream=sys.stdout):
    """Program entry point."""
    # The only args we're guaranteed to see are the program name and at least
    # one --module. GLOBAL_FLAGS might be empty, as might any of the
    # MODULE_FLAGS sections.
    if len(argv) < 2:
        sys.exit(
            'usage: ldflags_to_sanitizers.py [GLOBAL_FLAGS] '
            '--module [MODULE_FLAGS] [--module [MODULE_FLAGS]...]')

    global_flags, modules_flags = argv_to_module_arg_lists(argv[1:])
    all_sanitizers = list(sanitizers_from_args(global_flags))
    for module_flags in modules_flags:
        all_sanitizers.extend(sanitizers_from_args(module_flags))
    print(' '.join(sorted(set(all_sanitizers))), file=stream)


if __name__ == '__main__':
    main(sys.argv)
