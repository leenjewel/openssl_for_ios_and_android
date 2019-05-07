#!/bin/sh
#
# Copyright 2019 leenjewel
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

# Thanks https://stackoverflow.com/questions/52717228/how-to-compile-openssl-1-1-1-for-android

set -u

OPENSSL_VERSION=1.1.1a

API_LEVEL=16

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
PWDPATH="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    CYGWIN*)    MACHINE=Cygwin;;
    MINGW*)     MACHINE=MinGw;;
    *)          MACHINE="UNKNOWN:${unameOut}"
esac

BUILD_DIR=$PWDPATH/../output/openssl_111_android_build
OUT_DIR=$PWDPATH/../output/openssl_111_android

BUILD_TARGETS="armeabi armeabi-v7a arm64-v8a x86 x86_64"

if [ ! -d openssl-${OPENSSL_VERSION} ]
then
    if [ ! -f openssl-${OPENSSL_VERSION}.tar.gz ]
    then
        wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz || exit 128
    fi
    tar xzf openssl-${OPENSSL_VERSION}.tar.gz || exit 128
fi

cd openssl-${OPENSSL_VERSION} || exit 128

##### remove output-directory #####
rm -rf $OUT_DIR

##### export ndk directory. Required by openssl-build-scripts #####
export ANDROID_NDK

##### build-function #####
build_the_thing() {
    if [[ "$MACHINE" == "Mac" ]]; then
        TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/darwin-x86_64
    elif [[ "$MACHINE" == "Linux" ]]; then
        TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64
    fi
    export PATH=$TOOLCHAIN/$TRIBLE/bin:$TOOLCHAIN/bin:"$PATH"
echo $PATH
    make clean
    #./Configure $SSL_TARGET $OPTIONS -fuse-ld="$TOOLCHAIN/$TRIBLE/bin/ld" "-gcc-toolchain $TOOLCHAIN" && \
    ./Configure $SSL_TARGET $OPTIONS -fuse-ld="$TOOLCHAIN/$TRIBLE/bin/ld" zlib \
              no-asm \
              no-shared \
              no-unit-test && \
    make && \
    make install DESTDIR=$DESTDIR || exit 128
}

##### set variables according to build-tagret #####
for build_target in $BUILD_TARGETS
do
    case $build_target in
    armeabi)
        TRIBLE="arm-linux-androideabi"
        TC_NAME="arm-linux-androideabi-4.9"
        #OPTIONS="--target=armv5te-linux-androideabi -mthumb -fPIC -latomic -D__ANDROID_API__=$API_LEVEL"
        OPTIONS="--target=armv5te-linux-androideabi -mthumb -fPIC -latomic -D__ANDROID_API__=$API_LEVEL"
        DESTDIR="$BUILD_DIR/armeabi"
        SSL_TARGET="android-arm"
    ;;
    armeabi-v7a)
        TRIBLE="arm-linux-androideabi"
        TC_NAME="arm-linux-androideabi-4.9"
        OPTIONS="--target=armv7a-linux-androideabi -Wl,--fix-cortex-a8 -fPIC -D__ANDROID_API__=$API_LEVEL"
        DESTDIR="$BUILD_DIR/armeabi-v7a"
        SSL_TARGET="android-arm"
    ;;
    x86)
        TRIBLE="i686-linux-android"
        TC_NAME="x86-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${API_LEVEL}"
        DESTDIR="$BUILD_DIR/x86"
        SSL_TARGET="android-x86"
    ;;
    x86_64)
        TRIBLE="x86_64-linux-android"
        TC_NAME="x86_64-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${API_LEVEL}"
        DESTDIR="$BUILD_DIR/x86_64"
        SSL_TARGET="android-x86_64"
    ;;
    arm64-v8a)
        TRIBLE="aarch64-linux-android"
        TC_NAME="aarch64-linux-android-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${API_LEVEL}"
        DESTDIR="$BUILD_DIR/arm64-v8a"
        SSL_TARGET="android-arm64"
    ;;
    esac

    rm -rf $DESTDIR
    build_the_thing
#### copy libraries and includes to output-directory #####
    mkdir -p $OUT_DIR/inc/$build_target
    cp -R $DESTDIR/usr/local/include/* $OUT_DIR/inc/$build_target
    mkdir -p $OUT_DIR/lib/$build_target
    cp -R $DESTDIR/usr/local/lib/*.so $OUT_DIR/lib/$build_target
    cp -R $DESTDIR/usr/local/lib/*.a $OUT_DIR/lib/$build_target
done

echo Success
