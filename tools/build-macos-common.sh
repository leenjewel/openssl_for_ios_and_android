#!/bin/bash
#
# Copyright 2016 leenjewel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

source ./build-common.sh

if [ -z ${api+x} ]; then 
  api="10.12"
fi
if [ -z ${arch+x} ]; then 
  arch=("x86_64")
fi
if [ -z ${sdk+x} ]; then 
  sdk=("macosx")
fi
if [ -z ${platform+x} ]; then 
  platform=("MacOSX")
fi

export PLATFORM_TYPE="macOS"
export MACOS_MIN_TARGET="$api"
export ARCHS=(${arch[@]})
export SDKS=(${sdk[@]})
export PLATFORMS=(${platform[@]})

function get_macos_arch() {
    local common_arch=$1
    case ${common_arch} in
    x86_64)
        echo "x86-64"
        ;;
    esac
}

function macos_get_build_host() {
    local arch=$(get_macos_arch $1)
    case ${arch} in
    x86-64)
        echo "x86_64-apple-darwin"
        ;;
    esac
}

function set_macos_cpu_feature() {
    local name=$1
    local arch=$(get_macos_arch $2)
    local macos_min_target=$3
    local sysroot=$4
    case ${arch} in
    x86-64)
        export CC="xcrun -sdk macosx clang -arch x86_64"
        export CXX="xcrun -sdk macosx clang++ -arch x86_64"
        export CFLAGS="-arch x86_64 -target x86_64-apple-darwin -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel -Wno-unused-function -fstrict-aliasing -O2 -Wno-ignored-optimization-argument -isysroot ${sysroot} -mmacosx-version-min=${macos_min_target} -I${sysroot}/usr/include"
        export LDFLAGS="-arch x86_64 -target x86_64-apple-darwin -march=x86-64 -isysroot ${sysroot} -L${sysroot}/usr/lib "
        export CXXFLAGS="-std=c++14 -arch x86_64 -target x86_64-apple-darwin -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel -fstrict-aliasing -mmacosx-version-min=${macos_min_target} -I${sysroot}/usr/include"
        ;;
    *)
        log_error "not support" && exit 1
        ;;
    esac
}

function macos_printf_global_params() {
    local arch=$1
    local type=$2
    local platform=$3
    local in_dir=$4
    local out_dir=$5
    echo -e "arch =             $arch"
    echo -e "type =             $type"
    echo -e "platform =         $platform"
    echo -e "PLATFORM_TYPE =    $PLATFORM_TYPE"
    echo -e "MACOS_MIN_TARGET = $MACOS_MIN_TARGET"
    echo -e "in_dir =           $in_dir"
    echo -e "out_dir =          $out_dir"
    echo -e "CC =               $CC"
    echo -e "CXX =              $CXX"
    echo -e "CFLAGS =           $CFLAGS"
    echo -e "CXXFLAGS =         $CXXFLAGS"
    echo -e "LDFLAGS =          $LDFLAGS"
}
