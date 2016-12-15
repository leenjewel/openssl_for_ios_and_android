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

# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("android" "android-armeabi" "android64-aarch64" "android-x86" "android64" "android-mips" "android-mips64")
OUTNAME=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64" "mips" "mips64")
LIB_NAME="openssl-1.1.0c"
LIB_DEST_DIR="libs"
HEADER_DEST_DIR="include"
NDK=$ANDROID_NDK_ROOT
ANDROID_PLATFORM="android-23"
rm -rf "${HEADER_DEST_DIR}" "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || wget https://www.openssl.org/source/${LIB_NAME}.tar.gz;
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
  elif [ "$ARCH" == "android-armeabi" ]; then
    export ARCH_FLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
    export ARCH_LINK="-march=armv7-a -Wl,--fix-cortex-a8"
    export TOOL="arm-linux-androideabi"
    NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=arm-linux-androideabi-4.9 --install-dir=`pwd`/android-toolchain"
  elif [ "$ARCH" == "android64-aarch64" ]; then
    export ARCH_FLAGS=""
    export ARCH_LINK=""
    export TOOL="aarch64-linux-android"
    NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=aarch64-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
  elif [ "$ARCH" == "android-x86" ]; then
    export ARCH_FLAGS="-march=i686 -msse3 -mstackrealign -mfpmath=sse"
    export ARCH_LINK=""
    export TOOL="i686-linux-android"
    NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=x86-4.9 --install-dir=`pwd`/android-toolchain"
  elif [ "$ARCH" == "android64" ]; then
    export ARCH_FLAGS=""
    export ARCH_LINK=""
    export TOOL="x86_64-linux-android"
    NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=x86_64-4.9 --install-dir=`pwd`/android-toolchain"
  elif [ "$ARCH" == "android-mips" ]; then
    export ARCH_FLAGS=""
    export ARCH_LINK=""
    export TOOL="mipsel-linux-android"
    NDK_FLAGS="--platform=$ANDROID_PLATFORM --toolchain=mipsel-linux-android-4.9 --install-dir=`pwd`/android-toolchain"
  elif [ "$ARCH" == "android-mips64" ]; then
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
  ./Configure $ARCH \
              --with-zlib-include=$SYSROOT/usr/include \
              --with-zlib-lib=$SYSROOT/usr/lib \
              zlib
  PATH=$TOOLCHAIN_PATH:$PATH
  if make -j4
  then
    mkdir -p ../$LIB_DEST_DIR/$OUT
    cp libcrypto.a ../$LIB_DEST_DIR/$OUT
    cp libssl.a ../$LIB_DEST_DIR/$OUT
  fi
  popd; mv ${LIB_NAME} openssl-${OUT};
}

for ((i=0; i < ${#ARCHS[@]}; i++))
do
  if [[ $# -eq 0 ]] || [[ "$1" == "${ARCHS[i]}" ]]; then
    configure_make "${ARCHS[i]}" "${OUTNAME[i]}"
  fi
done
