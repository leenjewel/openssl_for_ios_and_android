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

# # read -n1 -p "Press any key to continue..."

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

LIB_VERSION="v1.40.0"
LIB_NAME="nghttp2-1.40.0"
LIB_DEST_DIR="${pwd_path}/../output/android/nghttp2-universal"

ARCHS=("arm" "arm64" "x86_64")
ABIS=("armeabi-v7a" "arm64-v8a" "x86_64")
ABI_TRIPLES=("arm-linux-androideabi" "aarch64-linux-android" "x86_64-linux-android")
ANDROID_API=23

# ARCHS=("arm64")

source ./build-android-common.sh

echo "https://github.com/nghttp2/nghttp2/releases/download/${LIB_VERSION}/${LIB_NAME}.tar.gz"

DEVELOPER=$(xcode-select -print-path)
SDK_VERSION=$(xcrun -sdk iphoneos --show-sdk-version)
rm -rf "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl -LO https://github.com/nghttp2/nghttp2/releases/download/${LIB_VERSION}/${LIB_NAME}.tar.gz >${LIB_NAME}.tar.gz

set_android_toolchain_bin

function configure_make() {

    ARCH=$1
    ABI=$2
    ABI_TRIPLE=$3

    # read -n1 -p "Press any key to continue..."
    log_info "configure $ARCH start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    PREFIX_DIR="${pwd_path}/../output/android/nghttp2-${ARCH}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    OUTPUT_ROOT=${TOOLS_ROOT}/../output/android/nghttp2-${ARCH}
    mkdir -p ${OUTPUT_ROOT}/log

    set_android_toolchain "nghttp2" "${ARCH}" "${ANDROID_API}"
    set_android_cpu_feature "nghttp2" "${ARCH}" "${ANDROID_API}"

    export ANDROID_NDK_HOME=${ANDROID_NDK_ROOT}
    echo ANDROID_NDK_HOME=${ANDROID_NDK_HOME}

    # read -n1 -p "Press any key to continue..."

    if [[ "${ARCH}" == "x86_64" ]]; then

        ./configure --host=x86_64-linux-android --prefix="${PREFIX_DIR}" --disable-app --disable-threads --enable-lib-only >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1

    elif [[ "${ARCH}" == "arm" ]]; then

        ./configure --host=arm-linux-androideabi --prefix="${PREFIX_DIR}" --disable-app --disable-threads --enable-lib-only >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1

    elif [[ "${ARCH}" == "arm64" ]]; then

        # --disable-lib-only need xml2 supc++ stdc++14
        ./configure --host=aarch64-linux-android --prefix="${PREFIX_DIR}" --disable-app --disable-threads --enable-lib-only >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1

    else
        log_error "not support" && exit 1
    fi

    # read -n1 -p "Press any key to continue..."
    log_info "make $ARCH start..."

    make clean >>"${OUTPUT_ROOT}/log/${ARCH}.log"
    if make -j$(get_cpu_count) >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1; then
        make install >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
    fi

    popd
}

# read -n1 -p "Press any key to continue..."

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${ABIS[i]}" "${ABI_TRIPLES[i]}"
    fi
done

log_info "build android nghttp2 end..."
