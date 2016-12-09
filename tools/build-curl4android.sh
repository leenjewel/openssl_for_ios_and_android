#!/bin/bash
#
#  Copyright (c) 2013 leenjewel <leenjewel@gmail.com>
#  MIT License (see LICENSE.md file)
#
#  Based on work by Felix Schulze:
#
#  Automatic build script for libssl and libcrypto 
#  for iPhoneOS and iPhoneSimulator
#
#  Created by Felix Schulze on 16.12.10.
#  Copyright 2010 Felix Schulze. All rights reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

set -u

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
 
# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("android" "android-armv7" "android64-arm64" "android-x86" "android-x86_64" "mips" "mips64")
OUTNAME=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64" "mips" "mips64")
LIB_NAME="curl-7.51.0"
LIB_DEST_DIR="libs"
HEADER_DEST_DIR="include"
rm -rf "${HEADER_DEST_DIR}" "${LIB_DEST_DIR}" "${LIB_NAME}"
NDK=$NDK_ROOT
ANDROID_PLATFORM="android-23"
# Unarchive library, then configure and make for specified architectures
configure_make()
{
   ARCH=$1; OUT=$2;
   tar xfz "${LIB_NAME}.tar.gz"
   pushd .; cd "${LIB_NAME}";

   if [ "$ARCH" == "android" ]; then
       export ARCH_FLAGS="-mthumb"
       export ARCH_LINK=""
       export TOOL="arm-linux-androideabi"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=arm-linux-androideabi-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-armv7" ]; then
       export ARCH_FLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
       export ARCH_LINK="-march=armv7-a -Wl,--fix-cortex-a8"
       export TOOL="arm-linux-androideabi"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=arm-linux-androideabi-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android64-arm64" ]; then
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="aarch64-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=aarch64-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-x86" ]; then
       export ARCH_FLAGS="-march=i686 -msse3 -mstackrealign -mfpmath=sse"
       export ARCH_LINK=""
       export TOOL="i686-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=x86-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-x86_64" ]; then
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="x86_64-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=x86_64-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "mips" ]; then
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="mipsel-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=mipsel-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "mips64" ]; then
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="mips64el-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=mips64el-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
   fi
   $NDK/build/tools/make-standalone-toolchain.sh $NDK_FLAGS
   export TOOLCHAIN_PATH=`pwd`/android-toolchain/bin
   export SYSROOT=`pwd`/android-toolchain/sysroot
   export CROSS_SYSROOT=$SYSROOT
   export NDK_TOOLCHAIN_BASENAME=${TOOLCHAIN_PATH}/${TOOL}
   export CC=$NDK_TOOLCHAIN_BASENAME-gcc
   export CXX=$NDK_TOOLCHAIN_BASENAME-g++
   export LINK=${CXX}
   export LD=$NDK_TOOLCHAIN_BASENAME-ld
   export AR=$NDK_TOOLCHAIN_BASENAME-ar
   export RANLIB=$NDK_TOOLCHAIN_BASENAME-ranlib
   export STRIP=$NDK_TOOLCHAIN_BASENAME-strip
   export CPPFLAGS="${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64 "
   export CXXFLAGS="${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64 -frtti -fexceptions "
   export CFLAGS="${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64 "
   export LDFLAGS="${ARCH_LINK}"
   mkdir -p ../$LIB_DEST_DIR/$OUT
   cp ${pwd_path}/openssl-${OUT}/libssl.a ${SYSROOT}/usr/lib/
   cp ${pwd_path}/openssl-${OUT}/libcrypto.a ${SYSROOT}/usr/lib/
   cp -r ${pwd_path}/openssl-${OUT}/include/openssl ${SYSROOT}/usr/include/
   ./configure --prefix=${pwd_path}/$LIB_DEST_DIR/$OUT \
       --with-sysroot=${SYSROOT} \
       --host=${TOOL} \
       --with-ssl=/usr \
       --enable-static \
       --disable-shared \
       --disable-verbose \
       --enable-threaded-resolver \
       --enable-ipv6
   PATH=$TOOLCHAIN_PATH:$PATH
   if make
   then
      make install
      make clean
      popd; rm -rf "${LIB_NAME}";
  fi
}



for ((i=0; i < ${#ARCHS[@]}; i++))
do
    if [[ $# -eq 0 ]] || [[ "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${OUTNAME[i]}"
    fi
done
