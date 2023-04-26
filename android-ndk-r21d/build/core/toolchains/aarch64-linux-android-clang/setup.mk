# Copyright (C) 2014 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

TOOLCHAIN_NAME := aarch64-linux-android
LLVM_TRIPLE := aarch64-none-linux-android

TARGET_TOOLCHAIN_ARCH_LIB_DIR := aarch64
TARGET_ASAN_BASENAME := libclang_rt.asan-aarch64-android.so
TARGET_UBSAN_BASENAME := libclang_rt.ubsan_standalone-aarch64-android.so

TARGET_CFLAGS := -fpic

TARGET_arm64_release_CFLAGS := \
    -O2 \
    -DNDEBUG \

TARGET_arm64_debug_CFLAGS := \
    -O0 \
    -UNDEBUG \
    -fno-limit-debug-info \

# This function will be called to determine the target CFLAGS used to build
# a C or Assembler source file, based on its tags.
#
TARGET-process-src-files-tags = \
$(eval __debug_sources := $(call get-src-files-with-tag,debug)) \
$(eval __release_sources := $(call get-src-files-without-tag,debug)) \
$(call set-src-files-target-cflags, $(__debug_sources), $(TARGET_arm64_debug_CFLAGS)) \
$(call set-src-files-target-cflags, $(__release_sources),$(TARGET_arm64_release_CFLAGS)) \
