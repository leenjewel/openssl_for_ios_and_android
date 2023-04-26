#
# Copyright (C) 2018 The Android Open Source Project
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

NDK_APP_ASAN := $(NDK_APP_DST_DIR)/$(TARGET_ASAN_BASENAME)
NDK_APP_UBSAN := $(NDK_APP_DST_DIR)/$(TARGET_UBSAN_BASENAME)

NDK_MODULES_LDFLAGS :=
$(foreach __module,$(__ndk_modules),\
    $(eval NDK_MODULES_LDFLAGS += --module $(__ndk_modules.$(__module).LDFLAGS)))
NDK_SANITIZERS := $(strip \
    $(shell $(HOST_PYTHON) $(BUILD_PY)/ldflags_to_sanitizers.py \
    $(NDK_APP_LDFLAGS) $(NDK_MODULES_LDFLAGS)))

NDK_SANITIZER_NAME := UBSAN
NDK_SANITIZER_FSANITIZE_ARGS := fuzzer undefined
NDK_SANITIZER_EXCLUDE_FSANITIZE_ARGS := address
include $(BUILD_SYSTEM)/install_sanitizer.mk

NDK_SANITIZER_NAME := ASAN
NDK_SANITIZER_FSANITIZE_ARGS := address
NDK_SANITIZER_EXCLUDE_FSANITIZE_ARGS :=
include $(BUILD_SYSTEM)/install_sanitizer.mk

# If the user has not specified their own wrap.sh and is using ASAN, install a
# default ASAN wrap.sh for them.
ifneq (,$(filter address,$(NDK_SANITIZERS)))
    ifeq ($(NDK_NO_USER_WRAP_SH),true)
        NDK_APP_WRAP_SH_$(TARGET_ARCH_ABI) := $(NDK_ROOT)/wrap.sh/asan.sh
    endif
endif
