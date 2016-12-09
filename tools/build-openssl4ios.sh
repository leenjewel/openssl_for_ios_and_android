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
pwd_path="$( cd -P "$( dirname "$SOURCE" )" && pwd )/"
 
# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("arm64" "armv7s" "armv7" "i386" "x86_64")
SDKS=("iphoneos" "iphoneos" "iphoneos" "iphonesimulator" "iphonesimulator")
PLATFORMS=("iPhoneOS" "iPhoneOS" "iPhoneOS" "iPhoneSimulator" "iPhoneSimulator")
DEVELOPER=`xcode-select -print-path`
SDK_VERSION=""10.1""
LIB_NAME="openssl-1.1.0c"
LIB_DEST_DIR="lib"
HEADER_DEST_DIR="include"
rm -rf "${HEADER_DEST_DIR}" "${LIB_DEST_DIR}" "${LIB_NAME}"
 
# Unarchive library, then configure and make for specified architectures
configure_make()
{
   ARCH=$1; SDK=$2; PLATFORM=$3;
   tar xfz "${LIB_NAME}.tar.gz"
   pushd .; cd "${LIB_NAME}";

   if [[ "${ARCH}" == "i386" || "${ARCH}" == "x86_64" ]]; then
       echo ""
   else
       sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
   fi

   export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
   export CROSS_SDK="${PLATFORM}${SDK_VERSION}.sdk"
   export TOOLS="${DEVELOPER}"
   export CC="${TOOLS}/usr/bin/gcc -arch ${ARCH}"

   if [[ "${ARCH}" == "x86_64" ]]; then
       ./Configure darwin64-x86_64-cc
   elif [[ "${ARCH}" == "i386" ]]; then
       ./Configure darwin-i386-cc
   else
       ./Configure iphoneos-cross
   fi
   export CFLAGS="-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK}"
   
   if make -j8
   then
       if [ -d "openssl-ios-${ARCH}" ]; then
           rm -fr "openssl-ios-${ARCH}"
       fi
       popd; mv "${LIB_NAME}" "openssl-ios-${ARCH}";
   fi
}
for ((i=0; i < ${#ARCHS[@]}; i++))
do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${SDKS[i]}" "${PLATFORMS[i]}"
    fi
done

# Combine libraries for different architectures into one
# Use .a files from the temp directory by providing relative paths
create_lib()
{
   LIB_SRC=$1; LIB_DST=$2;
   LIB_PATHS=( "${ARCHS[@]/#/${pwd_path}/openssl-ios-}" )
   LIB_PATHS=( "${LIB_PATHS[@]/%//${LIB_SRC}}" )
   lipo ${LIB_PATHS[@]} -create -output "${LIB_DST}"
}
mkdir "${LIB_DEST_DIR}";
create_lib "libcrypto.a" "${LIB_DEST_DIR}/libcrypto.a"
create_lib "libssl.a" "${LIB_DEST_DIR}/libssl.a"
 
