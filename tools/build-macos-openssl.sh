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

source ./build-macos-common.sh

if [ -z ${version+x} ]; then 
  version="1.1.1d"
fi
echo $version

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
LIB_VERSION="OpenSSL_$(echo $version | sed 's/\./_/g')"
LIB_NAME="openssl-$version"
LIB_DEST_DIR="${pwd_path}/../output/macos/openssl-universal"

init_log_color

echo "https://www.openssl.org/source/${LIB_NAME}.tar.gz"

# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1d.tar.gz
# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1f.tar.gz
DEVELOPER=$(xcode-select -print-path)
rm -rf "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl https://www.openssl.org/source/${LIB_NAME}.tar.gz >${LIB_NAME}.tar.gz

function configure_make() {

    ARCH=$1
    SDK=$2
    PLATFORM=$3
    SDK_PATH=$(xcrun -sdk ${SDK} --show-sdk-path)

    log_info "configure $ARCH start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    PREFIX_DIR="${pwd_path}/../output/macos/openssl-${ARCH}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    OUTPUT_ROOT=${TOOLS_ROOT}/../output/macos/openssl-${ARCH}
    mkdir -p ${OUTPUT_ROOT}/log

    set_macos_cpu_feature "openssl" "${ARCH}" "${MACOS_MIN_TARGET}" "${SDK_PATH}"
    
    macos_printf_global_params "$ARCH" "$SDK" "$PLATFORM" "$PREFIX_DIR" "$OUTPUT_ROOT"

    unset MACOSX_DEPLOYMENT_TARGET

    if [[ "${ARCH}" == "x86_64" ]]; then

        # openssl1.1.1d can be set normally, 1.1.0f does not take effect
        ./Configure darwin64-x86_64-cc no-shared --prefix="${PREFIX_DIR}"

    else
        log_error "not support" && exit 1
    fi

    log_info "make $ARCH start..."

    make clean >"${OUTPUT_ROOT}/log/${ARCH}.log"
    if make -j8 >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1; then
        make install_sw >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
        make install_ssldirs >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
    fi

    popd
}

log_info "${PLATFORM_TYPE} ${LIB_NAME} start..."

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${SDKS[i]}" "${PLATFORMS[i]}"
    fi
done

log_info "lipo start..."

function lipo_library() {
    LIB_SRC=$1
    LIB_DST=$2
    LIB_PATHS=("${ARCHS[@]/#/${pwd_path}/../output/macos/openssl-}")
    LIB_PATHS=("${LIB_PATHS[@]/%//lib/${LIB_SRC}}")
    lipo ${LIB_PATHS[@]} -create -output "${LIB_DST}"
}
mkdir -p "${LIB_DEST_DIR}"
lipo_library "libcrypto.a" "${LIB_DEST_DIR}/libcrypto-universal.a"
lipo_library "libssl.a" "${LIB_DEST_DIR}/libssl-universal.a"

log_info "${PLATFORM_TYPE} ${LIB_NAME} end..."
