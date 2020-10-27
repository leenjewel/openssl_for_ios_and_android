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

source ./build-android-common.sh

if [ -z ${version+x} ]; then 
  version="7.68.0"
fi

init_log_color

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

LIB_VERSION="curl-$(echo $version | sed 's/\./_/g')"
LIB_NAME="curl-$version"
LIB_DEST_DIR="${pwd_path}/../output/android/curl-universal"

echo "https://github.com/curl/curl/releases/download/${LIB_VERSION}/${LIB_NAME}.tar.gz"

# https://curl.haxx.se/download/${LIB_NAME}.tar.gz
# https://github.com/curl/curl/releases/download/curl-7_69_0/curl-7.69.0.tar.gz
# https://github.com/curl/curl/releases/download/curl-7_68_0/curl-7.68.0.tar.gz
DEVELOPER=$(xcode-select -print-path)
SDK_VERSION=$(xcrun -sdk iphoneos --show-sdk-version)
rm -rf "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl -LO https://github.com/curl/curl/releases/download/${LIB_VERSION}/${LIB_NAME}.tar.gz >${LIB_NAME}.tar.gz

set_android_toolchain_bin

function configure_make() {

    ARCH=$1
    ABI=$2
    ABI_TRIPLE=$3

    log_info "configure $ABI start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    PREFIX_DIR="${pwd_path}/../output/android/curl-${ABI}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    OUTPUT_ROOT=${TOOLS_ROOT}/../output/android/curl-${ABI}
    mkdir -p ${OUTPUT_ROOT}/log

    set_android_toolchain "curl" "${ARCH}" "${ANDROID_API}"
    set_android_cpu_feature "curl" "${ARCH}" "${ANDROID_API}"

    export ANDROID_NDK_HOME=${ANDROID_NDK_ROOT}
    echo ANDROID_NDK_HOME=${ANDROID_NDK_HOME}

    OPENSSL_OUT_DIR="${pwd_path}/../output/android/openssl-${ABI}"
    NGHTTP2_OUT_DIR="${pwd_path}/../output/android/nghttp2-${ABI}"

    export LDFLAGS="${LDFLAGS} -L${OPENSSL_OUT_DIR}/lib -L${NGHTTP2_OUT_DIR}/lib"
    # export LDFLAGS="-Wl,-rpath-link,-L${NGHTTP2_OUT_DIR}/lib,-L${OPENSSL_OUT_DIR}/lib $LDFLAGS "

    android_printf_global_params "$ARCH" "$ABI" "$ABI_TRIPLE" "$PREFIX_DIR" "$OUTPUT_ROOT"

    if [[ "${ARCH}" == "x86_64" ]]; then

        ./configure --host=$(android_get_build_host "${ARCH}") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --with-nghttp2=${NGHTTP2_OUT_DIR} >"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1

    elif [[ "${ARCH}" == "x86" ]]; then

        ./configure --host=$(android_get_build_host "${ARCH}") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --with-nghttp2=${NGHTTP2_OUT_DIR} >"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1

    elif [[ "${ARCH}" == "arm" ]]; then

        ./configure --host=$(android_get_build_host "${ARCH}") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --with-nghttp2=${NGHTTP2_OUT_DIR} >"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1

    elif [[ "${ARCH}" == "arm64" ]]; then

        # --enable-shared need nghttp2 cpp compile
        ./configure --host=$(android_get_build_host "${ARCH}") --prefix="${PREFIX_DIR}" --disable-shared --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --with-nghttp2=${NGHTTP2_OUT_DIR} >"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1

    else
        log_error "not support" && exit 1
    fi

    log_info "make $ABI start..."

    make clean >>"${OUTPUT_ROOT}/log/${ABI}.log"
    if make -j$(get_cpu_count) >>"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1; then
        make install >>"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1
    fi

    popd
}

log_info "${PLATFORM_TYPE} ${LIB_NAME} start..."

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${ABIS[i]}" "${ARCHS[i]}-linux-android"
    fi
done

log_info "${PLATFORM_TYPE} ${LIB_NAME} end..."
