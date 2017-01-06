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
LIB_NAME="protobuf"
LIB_VERSION="3.1.0"
LIB_FILENAME=${LIB_NAME}-${LIB_VERSION}
LIB_DEST_DIR=${TOOLS_ROOT}/libs
# rm -rf ${LIB_DEST_DIR}
[ -f ${LIB_FILENAME}.tar.gz ] || wget https://github.com/google/${LIB_NAME}/archive/v${LIB_VERSION}.tar.gz -O ${LIB_FILENAME}.tar.gz
# Unarchive library, then configure and make for specified architectures
configure_make() {
  ARCH=$1; ABI=$2;
  
  [ -d ${LIB_FILENAME} ] && rm -rf "${LIB_FILENAME}"
  tar xfz "${LIB_FILENAME}.tar.gz"
  pushd "${LIB_FILENAME}";

  configure $* "clang"
  export LDFLAGS="${LDFLAGS} -static-libstdc++"
  export LIBS="${LIBS:-""} -lc++_static -latomic"
  # fix CXXFLAGS
  export CXXFLAGS=${CXXFLAGS/"-finline-limit=64"/""}
  ./autogen.sh
  ./configure --prefix=${LIB_DEST_DIR}/${ABI} \
              --with-sysroot=${SYSROOT} \
              --with-protoc=`which protoc` \
              --with-zlib \
              --host=${TOOL} \
              --enable-static \
              --disable-shared \
              --enable-cross-compile
  PATH=$TOOLCHAIN_PATH:$PATH
  if make -j4
  then
    make install

    OUTPUT_ROOT=${TOOLS_ROOT}/../output/android/protobuf-${ABI}
    [ -d ${OUTPUT_ROOT}/include ] || mkdir -p ${OUTPUT_ROOT}/include
    cp -r ${LIB_DEST_DIR}/$ABI/include/ ${OUTPUT_ROOT}/include

    [ -d ${OUTPUT_ROOT}/lib ] || mkdir -p ${OUTPUT_ROOT}/lib
    find ${LIB_DEST_DIR}/${ABI}/lib -type f -iname '*.a' -exec cp {} ${OUTPUT_ROOT}/lib \;
  fi;
  popd;
}



for ((i=0; i < ${#ARCHS[@]}; i++))
do
  if [[ $# -eq 0 ]] || [[ "$1" == "${ARCHS[i]}" ]]; then
    [[ ${ANDROID_API} < 21 ]] && ( echo "${ABIS[i]}" | grep 64 > /dev/null ) && continue;
    configure_make "${ARCHS[i]}" "${ABIS[i]}"
  fi
done
