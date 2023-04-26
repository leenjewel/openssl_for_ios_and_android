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

TOOLCHAIN_NAME := arm-linux-androideabi
LLVM_TRIPLE := armv7-none-linux-androideabi

TARGET_TOOLCHAIN_ARCH_LIB_DIR := arm
TARGET_ASAN_BASENAME := libclang_rt.asan-arm-android.so
TARGET_UBSAN_BASENAME := libclang_rt.ubsan_standalone-arm-android.so

TARGET_CFLAGS := -fpic

# Clang does not set this up properly when using -fno-integrated-as.
# https://github.com/android-ndk/ndk/issues/906
TARGET_CFLAGS += -march=armv7-a

TARGET_CFLAGS.no_neon := -mfpu=vfpv3-d16

TARGET_arm_release_CFLAGS := \
    -O2 \
    -DNDEBUG \

TARGET_thumb_release_CFLAGS := \
    -mthumb \
    -Oz \
    -DNDEBUG \

TARGET_arm_debug_CFLAGS := \
    -O0 \
    -UNDEBUG \
    -fno-limit-debug-info \

TARGET_thumb_debug_CFLAGS := \
    -mthumb \
    -O0 \
    -UNDEBUG \
    -fno-limit-debug-info \

# This function will be called to determine the target CFLAGS used to build
# a C or Assembler source file, based on its tags.
#
TARGET-process-src-files-tags = \
$(eval __arm_sources := $(call get-src-files-with-tag,arm)) \
$(eval __thumb_sources := $(call get-src-files-without-tag,arm)) \
$(eval __debug_sources := $(call get-src-files-with-tag,debug)) \
$(eval __release_sources := $(call get-src-files-without-tag,debug)) \
$(call set-src-files-target-cflags, \
    $(call set_intersection,$(__arm_sources),$(__debug_sources)), \
    $(TARGET_arm_debug_CFLAGS)) \
$(call set-src-files-target-cflags,\
    $(call set_intersection,$(__arm_sources),$(__release_sources)),\
    $(TARGET_arm_release_CFLAGS)) \
$(call set-src-files-target-cflags,\
    $(call set_intersection,$(__thumb_sources),$(__debug_sources)),\
    $(TARGET_thumb_debug_CFLAGS)) \
$(call set-src-files-target-cflags,\
    $(call set_intersection,$(__thumb_sources),$(__release_sources)),\
    $(TARGET_thumb_release_CFLAGS)) \
$(call add-src-files-target-cflags,\
    $(call get-src-files-with-tag,no_neon),\
    $(TARGET_CFLAGS.no_neon)) \
$(call set-src-files-text,$(__arm_sources),arm) \
$(call set-src-files-text,$(__thumb_sources),thumb)
