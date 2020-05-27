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

export PLATFORM_TYPE="iOS"
export IOS_MIN_TARGET="8.0"
export ARCHS=("armv7" "arm64" "arm64e" "x86_64")
export SDKS=("iphoneos" "iphoneos" "iphoneos" "iphonesimulator")
export PLATFORMS=("iPhoneOS" "iPhoneOS" "iphoneos" "iPhoneSimulator")

function get_android_arch() {
    local common_arch=$1
    case ${common_arch} in
    armv7)
        echo "armv7"
        ;;
    arm64)
        echo "arm64"
        ;;
    arm64e)
        echo "arm64e"
        ;;
    x86)
        echo "i*86"
        ;;
    x86_64)
        echo "x86-64"
        ;;
    esac
}

function set_android_cpu_feature() {
    local name=$1
    local arch=$(get_android_arch $2)
    local ios_min_target=$3
    local sysroot=$4
    case ${arch} in
    armv7)
        export CC="xcrun -sdk iphoneos clang -arch armv7"
        export CXX="xcrun -sdk iphoneos clang++ -arch armv7"
        export CFLAGS="-arch armv7 -target armv7-ios-darwin -march=armv7 -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -Wno-unused-function -fstrict-aliasing -Oz -Wno-ignored-optimization-argument -DIOS -isysroot ${sysroot} -fembed-bitcode -miphoneos-version-min=${ios_min_target} -I${sysroot}/usr/include"
        export LDFLAGS="-arch armv7 -target armv7-ios-darwin -march=armv7 -isysroot ${sysroot} -fembed-bitcode -L${sysroot}/usr/lib "
        export CXXFLAGS="-std=c++14 -arch armv7 -target armv7-ios-darwin -march=armv7 -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -fstrict-aliasing -fembed-bitcode -DIOS -miphoneos-version-min=${ios_min_target} -I${sysroot}/usr/include"
        ;;
    arm64)
        export CC="xcrun -sdk iphoneos clang -arch arm64"
        export CXX="xcrun -sdk iphoneos clang++ -arch arm64"
        export CFLAGS="-arch arm64 -target aarch64-ios-darwin -march=armv8 -mcpu=generic -Wno-unused-function -fstrict-aliasing -Oz -Wno-ignored-optimization-argument -DIOS -isysroot ${sysroot} -fembed-bitcode -miphoneos-version-min=${ios_min_target} -I${sysroot}/usr/include"
        export LDFLAGS="-arch arm64 -target aarch64-ios-darwin -march=armv8 -isysroot ${sysroot} -fembed-bitcode -L${sysroot}/usr/lib "
        export CXXFLAGS="-std=c++14 -arch arm64 -target aarch64-ios-darwin -march=armv8 -mcpu=generic -fstrict-aliasing -fembed-bitcode -DIOS -miphoneos-version-min=${ios_min_target} -I${sysroot}/usr/include"
        ;;
    arm64e)
        # -march=armv8.3 ???
        export CC="xcrun -sdk iphoneos clang -arch arm64e"
        export CXX="xcrun -sdk iphoneos clang++ -arch arm64e"
        export CFLAGS="-arch arm64e -target aarch64-ios-darwin -Wno-unused-function -fstrict-aliasing -DIOS -isysroot ${sysroot} -fembed-bitcode -miphoneos-version-min=${ios_min_target} -I${sysroot}/usr/include"
        export LDFLAGS="-arch arm64e -target aarch64-ios-darwin -isysroot ${sysroot} -fembed-bitcode -L${sysroot}/usr/lib "
        export CXXFLAGS="-std=c++14 -arch arm64e -target aarch64-ios-darwin -fstrict-aliasing -fembed-bitcode -DIOS -miphoneos-version-min=${ios_min_target} -I${sysroot}/usr/include"
        ;;
    i*86)
        log_error "not support" && exit 1
        ;;
    x86-64)
        export CC="xcrun -sdk iphonesimulator clang -arch x86_64"
        export CXX="xcrun -sdk iphonesimulator clang++ -arch x86_64"
        export CFLAGS="-arch x86_64 -target x86_64-ios-darwin -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel -Wno-unused-function -fstrict-aliasing -O2 -Wno-ignored-optimization-argument -DIOS -isysroot ${sysroot} -mios-simulator-version-min=${ios_min_target} -I${sysroot}/usr/include"
        export LDFLAGS="-arch x86_64 -target x86_64-ios-darwin -march=x86-64 -isysroot ${sysroot} -L${sysroot}/usr/lib "
        export CXXFLAGS="-std=c++14 -arch x86_64 -target x86_64-ios-darwin -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel -fstrict-aliasing -DIOS -mios-simulator-version-min=${ios_min_target} -I${sysroot}/usr/include"
        ;;
    *) 
        log_error "not support" && exit 1
        ;;
    esac

    echo CC=$CC
    echo CXX=$CXX
    echo CFLAGS=$CFLAGS
    echo CXXFLAGS=$CXXFLAGS
    echo LDFLAGS=$LDFLAGS
}
