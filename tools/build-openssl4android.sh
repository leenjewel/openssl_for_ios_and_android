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

set -u

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
 
# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("android" "android-armeabi" "android64-aarch64" "android-x86" "android64" "android-mips" "android-mips64")
OUTNAME=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64" "mips" "mips64")
LIB_NAME="openssl-1.1.0c"
LIB_DEST_DIR="libs"
HEADER_DEST_DIR="include"
rm -rf "${HEADER_DEST_DIR}" "${LIB_DEST_DIR}" "${LIB_NAME}"
NDK=$ANDROID_NDK_ROOT
ANDROID_PLATFORM="android-23"
# Unarchive library, then configure and make for specified architectures
configure_make()
{
   ARCH=$1; OUT=$2;
   if [ -d "${LIB_NAME}" ]; then
       rm -fr "${LIB_NAME}"
   fi
   tar xfz "${LIB_NAME}.tar.gz"
   pushd .; cd "${LIB_NAME}";

   if [ "$ARCH" == "android" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-armeabi"
       export ARCH_FLAGS="-mthumb"
       export ARCH_LINK=""
       export TOOL="arm-linux-androideabi"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=arm-linux-androideabi-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-armeabi" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-armeabi-v7a"
       export ARCH_FLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
       export ARCH_LINK="-march=armv7-a -Wl,--fix-cortex-a8"
       export TOOL="arm-linux-androideabi"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=arm-linux-androideabi-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android64-aarch64" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-arm64-v8a"
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="aarch64-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=aarch64-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-x86" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-x86"
       export ARCH_FLAGS="-march=i686 -msse3 -mstackrealign -mfpmath=sse"
       export ARCH_LINK=""
       export TOOL="i686-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=x86-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android64" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-x86_64"
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="x86_64-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=x86_64-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-mips" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-mips"
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="mipsel-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=mipsel-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
   elif [ "$ARCH" == "android-mips64" ]; then
       PREFIX_DIR="${pwd_path}/openssl-android-mips64"
       ARCH="linux-generic64"
       export ARCH_FLAGS=""
       export ARCH_LINK=""
       export TOOL="mips64el-linux-android"
       NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=mips64el-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
   fi
   sh $NDK/build/tools/make-standalone-toolchain.sh $NDK_FLAGS
   export TOOLCHAIN_PATH=`pwd`/android-toolchain/bin
   export NDK_TOOLCHAIN_BASENAME=${TOOLCHAIN_PATH}/${TOOL}
   export SYSROOT=`pwd`/android-toolchain/sysroot
   export CROSS_SYSROOT=$SYSROOT
   export CC=$NDK_TOOLCHAIN_BASENAME-gcc
   export CXX=$NDK_TOOLCHAIN_BASENAME-g++
   export LINK=${CXX}
   export LD=$NDK_TOOLCHAIN_BASENAME-ld
   export AR=$NDK_TOOLCHAIN_BASENAME-ar
   export RANLIB=$NDK_TOOLCHAIN_BASENAME-ranlib
   export STRIP=$NDK_TOOLCHAIN_BASENAME-strip
   export CPPFLAGS=" ${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64 "
   export CXXFLAGS=" ${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64 -frtti -fexceptions "
   export CFLAGS=" ${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64 "
   export LDFLAGS=" ${ARCH_LINK} "
   echo "**********************************************"
   echo "export ARCH=$ARCH"
   echo "export NDK_TOOLCHAIN_BASENAME=$NDK_TOOLCHAIN_BASENAME"
   echo "export SYSROOT=$SYSROOT"
   echo "export CC=$CC"
   echo "export CXX=$CXX"
   echo "export LINK=$LINK"
   echo "export LD=$LD"
   echo "export AR=$AR"
   echo "export RANLIB=$RANLIB"
   echo "export STRIP=$STRIP"
   echo "export CPPFLAGS=$CPPFLAGS"
   echo "export CXXFLAGS=$CXXFLAGS"
   echo "export CFLAGS=$CFLAGS"
   echo "export LDFLAGS=$LDFLAGS"
   echo "**********************************************"

   if [ -d "${PREFIX_DIR}" ]; then
       rm -fr "${PREFIX_DIR}"
   fi
   mkdir -p "${PREFIX_DIR}"

   ./Configure $ARCH --prefix="${PREFIX_DIR}"
   PATH=$TOOLCHAIN_PATH:$PATH
   if make -j8
   then
       make install; popd;
       rm -fr "${LIB_NAME}"
   fi
}



for ((i=0; i < ${#ARCHS[@]}; i++))
do
    if [[ $# -eq 0 ]] || [[ "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${OUTNAME[i]}"
    fi
done

