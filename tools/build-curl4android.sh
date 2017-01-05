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

source ./_shared.sh

# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("android" "android-armeabi" "android64-aarch64" "android-x86" "android64" "android-mips" "android-mips64")
OUTNAME=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64" "mips" "mips64")
TOOLS_ROOT="${pwd_path}"
LIB_NAME="curl-7.51.0"
LIB_DEST_DIR=${TOOLS_ROOT}/../output/android
NDK=$ANDROID_NDK_ROOT
ANDROID_PLATFORM="android-23"
ANDROID_API="23"
[ -f ${LIB_NAME}.tar.gz ] || wget https://curl.haxx.se/download/${LIB_NAME}.tar.gz
# Unarchive library, then configure and make for specified architectures
configure_make()
{
  ARCH=$1; OUT=$2;
  [ -d "${LIB_NAME}" ] && rm -rf "${LIB_NAME}"
  tar xfz "${LIB_NAME}.tar.gz"
  pushd "${LIB_NAME}";

  configure $*
  # fix me
  cp ${TOOLS_ROOT}/../output/android/openssl-android-${OUT}/lib/libssl.a ${SYSROOT}/usr/lib
  cp ${TOOLS_ROOT}/../output/android/openssl-android-${OUT}/lib/libcrypto.a ${SYSROOT}/usr/lib
  cp -r ${TOOLS_ROOT}/../output/android/openssl-android-${OUT}/include/openssl ${SYSROOT}/usr/include

  PREFIX_DIR=${LIB_DEST_DIR}/curl-android-${OUT}
  if [ -d "${PREFIX_DIR}" ]; then
      rm -fr ${PREFIX_DIR}
  fi
  ./configure --prefix=${PREFIX_DIR} \
              --with-sysroot=${SYSROOT} \
              --host=${TOOL} \
              --with-ssl=/usr \
              --enable-static \
              --disable-shared \
              --disable-verbose \
              --enable-threaded-resolver \
              --enable-ipv6
  PATH=$TOOLCHAIN_PATH:$PATH
  if make -j4
  then
    make install
  fi;
  popd;
}

for ((i=0; i < ${#ARCHS[@]}; i++))
do
  if [[ $# -eq 0 ]] || [[ "$1" == "${ARCHS[i]}" ]]; then
    configure_make "${ARCHS[i]}" "${OUTNAME[i]}"
  fi
done
