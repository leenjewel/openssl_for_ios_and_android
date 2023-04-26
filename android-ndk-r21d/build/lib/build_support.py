#
# Copyright (C) 2015 The Android Open Source Project
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
import argparse
import multiprocessing
import os
import site
import subprocess
import sys


# "build" is not a valid package name for setuptools. This package will be
# silently removed from the source distribution because setuptools thinks it's
# the build directory rather than a python package named build. Pieces of this
# package are being moved into the ndk package where they belong, but will
# continue to be exported from here until we can chase down all the other
# users.
THIS_DIR = os.path.realpath(os.path.dirname(__file__))
site.addsitedir(os.path.join(THIS_DIR, '../..'))

# pylint: disable=wrong-import-position,unused-import
from ndk.abis import (
    ALL_ABIS,
    ALL_ARCHITECTURES,
    ALL_TOOLCHAINS,
    ALL_TRIPLES,
    LP32_ABIS,
    LP64_ABIS,
    arch_to_abis,
    arch_to_toolchain,
    arch_to_triple,
    toolchain_to_arch,
)

from ndk.hosts import Host, get_default_host, host_to_tag

from ndk.paths import (
    android_path,
    get_dist_dir,
    get_out_dir,
    ndk_path,
    sysroot_path,
    toolchain_path,
)
# pylint: enable=wrong-import-position,unused-import


def minimum_platform_level(abi):
    import ndk.abis
    return ndk.abis.min_api_for_abi(abi)


def jobs_arg():
    return '-j{}'.format(multiprocessing.cpu_count() * 2)


def build(cmd, args, intermediate_package=False):
    package_dir = args.out_dir if intermediate_package else args.dist_dir
    common_args = [
        '--verbose',
        '--package-dir={}'.format(package_dir),
    ]

    build_env = dict(os.environ)
    build_env['NDK_BUILDTOOLS_PATH'] = android_path('ndk/build/tools')
    build_env['ANDROID_NDK_ROOT'] = ndk_path()
    subprocess.check_call(cmd + common_args, env=build_env)


def make_package(name, directory, out_dir):
    """Pacakges an NDK module for release.

    Makes a zipfile of the single NDK module that can be released in the SDK
    manager.

    Args:
        name: Name of the final package, excluding extension.
        directory: Directory to be packaged.
        out_dir: Directory to place package.
    """
    if not os.path.isdir(directory):
        raise ValueError('directory must be a directory: ' + directory)

    path = os.path.join(out_dir, name + '.zip')
    if os.path.exists(path):
        os.unlink(path)

    cwd = os.getcwd()
    os.chdir(os.path.dirname(directory))
    basename = os.path.basename(directory)
    try:
        subprocess.check_call(
            ['zip', '-x', '*.pyc', '-x', '*.pyo', '-x', '*.swp', '-x',
             '*.git*', '-0qr', path, basename])
    finally:
        os.chdir(cwd)


class ArgParser(argparse.ArgumentParser):
    def __init__(self):
        super(ArgParser, self).__init__()

        self.add_argument(
            '--host',
            choices=Host,
            type=Host,
            default=get_default_host(),
            help='Build binaries for given OS (e.g. linux).')

        self.add_argument(
            '--out-dir', help='Directory to place temporary build files.',
            type=os.path.realpath, default=get_out_dir())

        # The default for --dist-dir has to be handled after parsing all
        # arguments because the default is derived from --out-dir. This is
        # handled in run().
        self.add_argument(
            '--dist-dir', help='Directory to place the packaged artifact.',
            type=os.path.realpath)


def run(main_func, arg_parser=ArgParser):
    if 'ANDROID_BUILD_TOP' not in os.environ:
        top = os.path.join(os.path.dirname(__file__), '../../..')
        os.environ['ANDROID_BUILD_TOP'] = os.path.realpath(top)

    args = arg_parser().parse_args()

    if args.dist_dir is None:
        args.dist_dir = get_dist_dir(args.out_dir)

    # We want any paths to be relative to the invoked build script.
    main_filename = os.path.realpath(sys.modules['__main__'].__file__)
    os.chdir(os.path.dirname(main_filename))

    main_func(args)
