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
"""Packages the platform's RenderScript for the NDK."""
import os
import site
import sys

site.addsitedir(os.path.join(os.path.dirname(__file__), '../lib'))
site.addsitedir(os.path.join(os.path.dirname(__file__), '../..'))

# pylint: disable=import-error,wrong-import-position
import build_support
from ndk.hosts import Host, host_to_tag
# pylint: enable=import-error,wrong-import-position


def get_rs_prebuilt_path(host_tag: str) -> str:
    rel_prebuilt_path = f'prebuilts/renderscript/host/{host_tag}'
    prebuilt_path = os.path.join(build_support.android_path(),
                                 rel_prebuilt_path)
    if not os.path.isdir(prebuilt_path):
        sys.exit(f'Could not find prebuilt RenderScript at {prebuilt_path}')
    return prebuilt_path


def main(args) -> None:
    RS_VERSION = 'current'

    host: Host = args.host
    package_dir = args.dist_dir

    os_name = args.host.value
    if os_name == 'windows64':
        os_name = 'windows'

    prebuilt_path = get_rs_prebuilt_path(f'{os_name}-x86')
    print(f'prebuilt path: {prebuilt_path}')

    package_name = f'renderscript-toolchain-{host_to_tag(host)}'
    built_path = os.path.join(prebuilt_path, RS_VERSION)
    build_support.make_package(package_name, built_path, package_dir)


if __name__ == '__main__':
    build_support.run(main)
