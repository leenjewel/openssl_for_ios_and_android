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

# read -n1 -p "Press any key to continue..."

set -u

TOOLS_ROOT=$(pwd)

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$(cd -P "$(dirname "$SOURCE")" && pwd)"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$(cd -P "$(dirname "$SOURCE")" && pwd)"

echo pwd_path=${pwd_path}
echo TOOLS_ROOT=${TOOLS_ROOT}

# openssl-1.1.0f has a configure bug
# openssl-1.1.1d has fix configure bug
LIB_VERSION="OpenSSL_1_1_1d"
LIB_NAME="openssl-1.1.1d"
LIB_DEST_DIR="${pwd_path}/../output/android"

ARCHS=("arm" "arm64" "x86_64")
ABIS=("armeabi-v7a" "arm64-v8a" "x86_64")
ABI_TRIPLES=("arm-linux-androideabi" "aarch64-linux-android" "x86_64-linux-android")

export ANDROID_API=23
export ANDROID_MIN_API=7
export ANDROID_MAX_API=29

# ARCHS=("x86_64")
# SDKS=("iphonesimulator")
# PLATFORMS=("iPhoneSimulator")

echo "https://www.openssl.org/source/${LIB_NAME}.tar.gz"

# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1d.tar.gz
# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1f.tar.gz
DEVELOPER=$(xcode-select -print-path)
SDK_VERSION=$(xcrun -sdk iphoneos --show-sdk-version)
rm -rf "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl https://www.openssl.org/source/${LIB_NAME}.tar.gz >${LIB_NAME}.tar.gz

source ./build-android-common.sh

configure_make() {

    ARCH=$1
    ABI=$2
    ABI_TRIPLE=$3

    echo "configure $ARCH start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    PREFIX_DIR="${pwd_path}/../output/android/openssl-${ARCH}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    set_android_toolchain "openssl" ${ARCH}

    if [[ "${ARCH}" == "x86_64" ]]; then

        ./Configure darwin64-x86_64-cc no-shared --prefix="${PREFIX_DIR}"

    elif [[ "${ARCH}" == "armv7" ]]; then

        ./Configure iphoneos-cross no-shared --prefix="${PREFIX_DIR}"

    elif [[ "${ARCH}" == "arm64" ]]; then

        ./Configure iphoneos-cross no-shared --prefix="${PREFIX_DIR}"

    else
        echo "not support" && exit 1
    fi

    OUTPUT_ROOT=${TOOLS_ROOT}/../output/android/openssl-${ARCH}
    mkdir -p ${OUTPUT_ROOT}/log

    echo "make $ARCH start..."

    make clean &>"${OUTPUT_ROOT}/log/${ARCH}.log"
    if make -j8 >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1; then
        make install_sw >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
        make install_ssldirs >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
    fi

    popd
}

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${ABIS[i]}" "${ABI_TRIPLES[i]}"
    fi
done

echo "buil android openssl end..."
