# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)



add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/http)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/http/openssl/${ANDROID_ABI}/include)

add_library(local_crypto STATIC IMPORTED)
set_target_properties(local_crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/http/openssl/${ANDROID_ABI}/lib/libcrypto.a)

add_library(local_ssl STATIC IMPORTED)
set_target_properties(local_ssl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/http/openssl/${ANDROID_ABI}/lib/libssl.a)

#target_link_libraries(local_ssl local_crypto)


find_library(z-lib z)








# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        http/test_http.cpp
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.

        local_ssl
        local_crypto
        ${z-lib}
        ${log-lib})