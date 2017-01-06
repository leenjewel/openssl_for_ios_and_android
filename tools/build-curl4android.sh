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
TOOLS_ROOT=`pwd`
LIB_NAME="curl-7.51.0"
LIB_DEST_DIR=${TOOLS_ROOT}/libs
[ -f ${LIB_NAME}.tar.gz ] || wget https://curl.haxx.se/download/${LIB_NAME}.tar.gz
# Unarchive library, then configure and make for specified architectures
configure_make()
{
  ARCH=$1; ABI=$2;
  [ -d "${LIB_NAME}" ] && rm -rf "${LIB_NAME}"
  tar xfz "${LIB_NAME}.tar.gz"
  pushd "${LIB_NAME}";

  configure $*
  # fix me
  cp ${TOOLS_ROOT}/../lib/${ABI}/libssl.a ${SYSROOT}/usr/lib
  cp ${TOOLS_ROOT}/../lib/${ABI}/libcrypto.a ${SYSROOT}/usr/lib
  cp -r ${TOOLS_ROOT}/../include/${ABI}/openssl ${SYSROOT}/usr/include

  mkdir -p ${LIB_DEST_DIR}/${ABI}
  ./configure --prefix=${LIB_DEST_DIR}/${ABI} \
              --with-sysroot=${SYSROOT} \
              --host=${TOOL} \
              --with-ssl=/usr \
              --enable-static \
              --enable-threaded-resolver \
              --enable-ipv6 \
              --disable-shared \
              --disable-verbose \
              --disable-ldap \
              --disable-ldaps \
              --disable-rtsp \
              --disable-dict \
              --disable-telnet \
              --disable-pop3 \
              --disable-imap \
              --disable-smb \
              --disable-smtp \
              --disable-gopher \
              --disable-manual
  PATH=$TOOLCHAIN_PATH:$PATH
  if make -j4
  then
    make install

    cp ${LIB_DEST_DIR}/${ABI}/lib/libcurl.a ${TOOLS_ROOT}/../lib/${ABI}
    cp -r ${LIB_DEST_DIR}/${ABI}/include/curl ${TOOLS_ROOT}/../include/${ABI}
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
