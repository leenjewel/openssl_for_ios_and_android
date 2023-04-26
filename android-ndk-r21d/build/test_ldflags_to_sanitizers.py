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
"""Does test_ldflags_to_sanitizers stuff."""
from __future__ import absolute_import
import unittest

try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO

from build.ldflags_to_sanitizers import argv_to_module_arg_lists
from build.ldflags_to_sanitizers import main as ldflags_main
from build.ldflags_to_sanitizers import sanitizers_from_args


class LdflagsToSanitizersTest(unittest.TestCase):
    def test_sanitizers_from_args_no_sanitize_args(self):
        """Tests that we don't identify sanitizers when there are none."""
        self.assertListEqual([], sanitizers_from_args([]))
        self.assertListEqual([], sanitizers_from_args(['foo', 'bar']))

    def test_sanitizers_from_args_enabled_sanitizers(self):
        """Tests that we find enabled sanitizers."""
        self.assertListEqual(
            ['address'], sanitizers_from_args(['-fsanitize=address']))
        self.assertListEqual(
            ['address'], sanitizers_from_args(['-fsanitize=address', 'foo']))
        self.assertListEqual(
            ['address', 'undefined'],
            sanitizers_from_args(
                ['-fsanitize=address', '-fsanitize=undefined']))
        self.assertListEqual(
            ['address', 'undefined'],
            sanitizers_from_args(['-fsanitize=address,undefined']))
        self.assertListEqual(
            ['address', 'undefined'],
            sanitizers_from_args(['-fsanitize=address,undefined', 'foo']))

    def test_sanitizers_from_args_disabled_sanitizers(self):
        """Tests that we don't find disabled sanitizers."""
        self.assertListEqual([], sanitizers_from_args(
            ['-fno-sanitize=address']))
        self.assertListEqual([], sanitizers_from_args(
            ['-fno-sanitize=address', 'foo']))
        self.assertListEqual([], sanitizers_from_args(
            ['-fno-sanitize=address', '-fno-sanitize=undefined']))
        self.assertListEqual([], sanitizers_from_args(
            ['-fno-sanitize=address,undefined']))
        self.assertListEqual([], sanitizers_from_args(
            ['-fno-sanitize=address,undefined', 'foo']))

    def test_sanitizers_from_args_enabled_disabled_sanitizers(self):
        """Tests that we correctly identify only enabled sanitizers."""
        self.assertListEqual([], sanitizers_from_args(
            ['-fsanitize=address', '-fno-sanitize=address']))
        self.assertListEqual(['address'], sanitizers_from_args(
            ['-fsanitize=address', '-fno-sanitize=address',
             '-fsanitize=address']))
        self.assertListEqual([], sanitizers_from_args(
            ['-fsanitize=address', '-fno-sanitize=address',
             '-fsanitize=address', '-fno-sanitize=address']))
        self.assertListEqual(['undefined'], sanitizers_from_args(
            ['-fsanitize=address,undefined', '-fno-sanitize=address']))
        self.assertListEqual(['undefined'], sanitizers_from_args(
            ['-fsanitize=address', '-fsanitize=undefined',
             '-fno-sanitize=address']))

    def test_argv_to_module_arg_lists(self):
        """Tests that modules' arguments are properly identified."""
        self.assertTupleEqual(([], []), argv_to_module_arg_lists([]))
        self.assertTupleEqual((['foo'], []), argv_to_module_arg_lists(['foo']))

        self.assertTupleEqual(
            ([], [['foo', 'bar'], ['baz']]),
            argv_to_module_arg_lists(
                ['--module', 'foo', 'bar', '--module', 'baz']))

        self.assertTupleEqual(
            (['foo', 'bar'], [['baz']]),
            argv_to_module_arg_lists(['foo', 'bar', '--module', 'baz']))

    def test_main(self):
        """Test that the program itself works."""
        sio = StringIO()
        ldflags_main(
            ['ldflags_to_sanitizers.py', '-fsanitize=undefined', '--module',
             '-fsanitize=address,thread', '-fno-sanitize=thread',
             '--module', '-fsanitize=undefined'], sio)
        self.assertEqual('address undefined', sio.getvalue().strip())
