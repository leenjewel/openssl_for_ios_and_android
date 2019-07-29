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

# Setting
IOS_MIN_TARGET="8.0"
LIB_NAME="openssl-1.1.0f"
LIB_DEST_DIR="${pwd_path}/../output/ios/openssl-universal"
HEADER_DEST_DIR="include"

# Setup architectures, library name and other vars + cleanup from previous runs
ARCHS=("arm64" "armv7s" "armv7" "i386" "x86_64")
SDKS=("iphoneos" "iphoneos" "iphoneos" "iphonesimulator" "iphonesimulator")
PLATFORMS=("iPhoneOS" "iPhoneOS" "iPhoneOS" "iPhoneSimulator" "iPhoneSimulator")
DEVELOPER=`xcode-select -print-path`
SDK_VERSION=`xcrun -sdk iphoneos --show-sdk-version`
rm -rf "${HEADER_DEST_DIR}" "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || wget https://www.openssl.org/source/${LIB_NAME}.tar.gz;
 
# Unarchive library, then configure and make for specified architectures
configure_make()
{
   ARCH=$1; SDK=$2; PLATFORM=$3;
   if [ -d "${LIB_NAME}" ]; then
       rm -fr "${LIB_NAME}"
   fi
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

   PREFIX_DIR="${pwd_path}/../output/ios/openssl-${ARCH}"
   if [ -d "${PREFIX_DIR}" ]; then
       rm -fr "${PREFIX_DIR}"
   fi
   mkdir -p "${PREFIX_DIR}"

   if [[ "${ARCH}" == "x86_64" ]]; then
        unset IPHONEOS_DEPLOYMENT_TARGET
       ./Configure darwin64-x86_64-cc --prefix="${PREFIX_DIR}"
   elif [[ "${ARCH}" == "i386" ]]; then
        unset IPHONEOS_DEPLOYMENT_TARGET
       ./Configure darwin-i386-cc --prefix="${PREFIX_DIR}"
   else
       export IPHONEOS_DEPLOYMENT_TARGET=${IOS_MIN_TARGET}
       ./Configure iphoneos-cross --prefix="${PREFIX_DIR}"
   fi
   export CFLAGS="-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK}"
   
   if [ ! -d ${CROSS_TOP}/SDKs/${CROSS_SDK} ]; then
       echo "ERROR: iOS SDK version:'${SDK_VERSION}' incorrect, SDK in your system is:"
       xcodebuild -showsdks | grep iOS
       exit -1
   fi
   
   make clean
   if make -j8
   then
       # make install;
       make install_sw;
       make install_ssldirs;
       popd;
       rm -fr "${LIB_NAME}"
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
   LIB_PATHS=( "${ARCHS[@]/#/${pwd_path}/../output/ios/openssl-}" )
   LIB_PATHS=( "${LIB_PATHS[@]/%//lib/${LIB_SRC}}" )
   lipo ${LIB_PATHS[@]} -create -output "${LIB_DST}"
}
mkdir "${LIB_DEST_DIR}";
create_lib "libcrypto.a" "${LIB_DEST_DIR}/libcrypto.a"
create_lib "libssl.a" "${LIB_DEST_DIR}/libssl.a"
 
