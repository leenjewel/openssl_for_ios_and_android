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

source ./_shared.sh

# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("android" "android-armv7" "android64-arm64" "android-x86" "android-x86_64" "mips" "mips64")
OUTNAME=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64" "mips" "mips64")
TOOLS_ROOT=`pwd`
LIB_NAME="openssl-1.1.0c"
LIB_DEST_DIR=${TOOLS_ROOT}/libs
NDK=$ANDROID_NDK_ROOT
ANDROID_API="21"
[ -d ${LIB_DEST_DIR} ] && rm -rf ${LIB_DEST_DIR}
[ -f "${LIB_NAME}.tar.gz" ] || wget https://www.openssl.org/source/${LIB_NAME}.tar.gz;
# Unarchive library, then configure and make for specified architectures
configure_make() {
  ARCH=$1; OUT=$2;
  rm -rf "${LIB_NAME}"
  tar xfz "${LIB_NAME}.tar.gz"
  pushd "${LIB_NAME}";

  configure $*
  ./Configure $ARCH \
              --prefix=${LIB_DEST_DIR}/${OUT} \
              --with-zlib-include=$SYSROOT/usr/include \
              --with-zlib-lib=$SYSROOT/usr/lib \
              zlib \
              no-asm \
              no-shared \
              no-unit-test
  PATH=$TOOLCHAIN_PATH:$PATH

  if make -j4
  then
    make install
    [ -d ${TOOLS_ROOT}/../include/$OUT ] || mkdir -p ${TOOLS_ROOT}/../include/$OUT
    cp -r include/openssl ${TOOLS_ROOT}/../include/$OUT

    [ -d ${TOOLS_ROOT}/../lib/$OUT ] || mkdir -p ${TOOLS_ROOT}/../lib/$OUT
    find . -type f -iname \( 'libssl.a' -or -iname 'libcrypto.a' \) -exec cp {} ${TOOLS_ROOT}/../lib/$OUT \;
  fi;
}

for ((i=0; i < ${#ARCHS[@]}; i++))
do
  if [[ $# -eq 0 ]] || [[ "$1" == "${ARCHS[i]}" ]]; then
    configure_make "${ARCHS[i]}" "${OUTNAME[i]}"
  fi
done
