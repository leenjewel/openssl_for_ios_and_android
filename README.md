## More iOS and Android build scripts
(simple-build-ios-and-android-script)[https://github.com/AsteriskZuo/simple-build-ios-and-android-script]

# OpenSSL & cURL Library for iOS and Android

<table>
<thead>
<tr><td>library</td><td>version</td><td>platform support</td><td>arch support</td><td>pull commit</td></tr>
</thead>
<tbody>
<tr><td>openssl</td><td>1.1.1d</td><td>ios</td><td>armv7 arm64 arm64e x86_64 </td><td>aae1672</td></tr>
<tr><td></td><td></td><td>android</td><td>armeabi-v7a arm64-v8a x86 x86_64 </td><td>aae1672</td></tr>
<tr><td>nghttp2</td><td>1.40.0</td><td>ios</td><td>armv7 arm64 arm64e x86x86_64 </td><td>aae1672</td></tr>
<tr><td></td><td></td><td>android</td><td>armeabi-v7a arm64-v8a x86 x86_64 </td><td>aae1672</td></tr>
<tr><td>curl</td><td>7.68.0</td><td>ios</td><td>armv7 arm64 arm64e x86_64 </td><td>aae1672</td></tr>
<tr><td></td><td></td><td>android</td><td>armeabi-v7a arm64-v8a x86 x86_64 </td><td>aae1672</td></tr>
</tbody>
</table>

## Downloads

If you do not want to build it by yourself, you could download our prebuilt library from [there](https://github.com/leenjewel/openssl_for_ios_and_android/releases/tag/20170105)

## OpenSSL Version

 - ~~[openssl-1.0.2c.tar.gz](https://www.openssl.org/source/openssl-1.0.2c.tar.gz)~~
 - [openssl-1.1.0f.tar.gz](https://www.openssl.org/source/openssl-1.1.0f.tar.gz)
 - [openssl-1.1.1d.tar.gz](https://www.openssl.org/source/openssl-1.1.1d.tar.gz)
 - [https://github.com/openssl/openssl](https://github.com/openssl/openssl)

## nghttp2 Version

 - [nghttp2-1.40.0.tar.gz](https://github.com/nghttp2/nghttp2/releases/download/v1.40.0/nghttp2-1.40.0.tar.gz)
 - [https://nghttp2.org/](https://nghttp2.org/)

## cURL Version

 - ~~[curl-7.47.1.tar.gz](https://curl.haxx.se/download/curl-7.47.1.tar.gz)~~
 - [curl-7.66.0.tar.gz](https://curl.haxx.se/download/curl-7.66.0.tar.gz)
 - [curl-7.68.0.tar.gz](https://curl.haxx.se/download/curl-7.68.0.tar.gz)
 - [https://github.com/curl/curl](https://github.com/curl/curl)

## Android NDK Version

 - ~~[android-ndk-r13b](https://dl.google.com/android/repository/android-ndk-r13b-darwin-x86_64.zip)~~
 - ~~[android-ndk-r14b](https://dl.google.com/android/repository/android-ndk-r14b-darwin-x86_64.zip)~~
 - ~~android-ndk-r15 (**Do not try to build use 15 It will fail**)~~
 - support api 23 above

## How to build

### ！！！Scripts are compiled and passed on the Mac, not tested on the Linux system

- MacOS info: 10.15.2

### For iOS

- Xcode info: Version 11.3.1 (11C504) (for reference only)
- Build dependencies: todo
- Build order: 1.build openssl, 2.build nghttp2, 3.build curl (curl depend openssl and nghttp2)
- only build static library(.a)
- build sh cmd: for example:
```
$ cd tools
$ sh build-ios-openssl.sh
$ sh build-ios-nghttp2.sh
$ sh build-ios-curl.sh
```

### For Android

- Android Studio info: 3.5.3 (for reference only)
- Build dependencies: todo
- Build order: 1.build openssl, 2.build nghttp2, 3.build curl (curl depend openssl and nghttp2)
- build static library(.a) and dynamic library (exclude curl arm64-v8a)
- env macro: for example:
```
export ANDROID_HOME=/Users/userid/Library/Android/sdk
export ANDROID_NDK_ROOT=$ANDROID_HOME/ndk-bundle

```
- build sh cmd: for example:
```
$ cd tools
$ sh build-android-openssl.sh
$ sh build-android-nghttp2.sh
$ sh build-android-curl.sh
```



> **You must build openssl and nghttp2(support http2) first**

### Others

OpenSSL for Android is build with `libz` support using dynamic
link. `libz` is publically provided by Android system.

## Demo

### iOS

 - Xcode version 11.3.1
 - The latest available example is in the demo2 folder (/openssl_for_ios_and_android/example/ios/demo2).
 - Copy libcrypto-universal.a, libcurl-universal.a, libnghttp2-universal.a, libssl-universal.a four files to folder (/openssl_for_ios_and_android/example/ios/demo2/test_curl_with_ssl_and_http2_ios/test/lib)
 - Use Xcode open project test.

### Android

- Android Studio 3.5.3
- The latest available example is in the demo2 folder (/openssl_for_ios_and_android/example/android/demo2).
- Copy libcrypto.a, libcurl.a, libnghttp2.a, libssl.a four files to folder (/openssl_for_ios_and_android/example/android/demo2/test_curl_with_ssl_and_http2_android/app/src/main/cpp/test/lib) (armeabi-v7a, arm64-v8a, x86_64)
- Use Android Studio open project test.

## How to use

### For iOS

Copy `lib/libcrypto.a` and `lib/libssl.a` and `lib/libcurl.a` to your project.

Copy `include/openssl` folder and `include/curl` folder to your project.

Add `libcrypto.a` and `libssl.a` and `libcurl.a` to `Frameworks` group and add them to `[Build Phases]  ====> [Link Binary With Libraries]`.

Add openssl include path and curl include path to your `[Build Settings] ====> [User Header Search Paths]`

### About "\_\_curl\_rule\_01\_\_ declared as an array with a negative size" problem

When you build cURL for arm64 you will get this error.

You need to change `curlbuild.h` from :

```c
#define CURL_SIZEOF_LONG 4
```

to :

```c
#ifdef __LP64__
#define CURL_SIZEOF_LONG 8
#else
#define CURL_SIZEOF_LONG 4
#endif
```

### For Android

Copy `lib/armeabi` folder and `lib/armeabi-v7a` folder and `lib/x86` to your android project `libs` folder.

Copy `include/openssl` folder and `include/curl` to your android project.

#### Android Makefile
Add openssl include path to `jni/Android.mk`. 

```
#Android.mk

include $(CLEAR_VARS)

LOCAL_MODULE := curl
LOCAL_SRC_FILES := Your cURL Library Path/$(TARGET_ARCH_ABI)/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/Your Openssl Include Path/openssl \
	$(LOCAL_PATH)/Your cURL Include Path/curl

LOCAL_STATIC_LIBRARIES := libcurl

LOCAL_LDLIBS := -lz
	
```

### CMake
Define `ssl`, `crypto`, `curl` as *STATIC IMPORTED* libraries.


```
add_library(crypto STATIC IMPORTED)
set_target_properties(crypto
  PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl
  PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libssl.a)

add_library(curl STATIC IMPORTED)
set_target_properties(curl
  PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libcurl.a)
```

Then link these libraries with your target, e.g.


```
target_link_libraries( # Specifies the target library.
                       native-lib

                       curl
                       ssl
                       crypto
                       )
```

### About "libcrypto.a(ui_openssl.o):ui_openssl.c:function read_string_inner: error: undefined reference to 'signal' " problem

when you get these error 

```
libcrypto.a(ui_openssl.o):ui_openssl.c:function read_string_inner: error: undefined reference to 'signal' 

libcrypto.a(ui_openssl.o):ui_openssl.c:function read_string_inner: error: undefined reference to 'tcsetattr' 
```

You need to rebuild OpenSSL static library with NDK API level 16 or earlier

If you build OpenSSL with API level 16 or earlier you may not build it for arch 64 bit only support 32 bit

more information :

[https://github.com/openssl/openssl/issues/988](https://github.com/openssl/openssl/issues/988)

[http://stackoverflow.com/questions/37122126/whats-the-exact-significance-of-android-ndk-platform-version-compared-to-api-le](http://stackoverflow.com/questions/37122126/whats-the-exact-significance-of-android-ndk-platform-version-compared-to-api-le)


