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

# Generates rules to install sanitizer runtime libraries to the out directory if
# the sanitizer is requested in the app's ldflags.
#
# Args:
#   NDK_SANTIZER_NAME:
#       Sanitizer name used as the variable name component to find the library.
#       i.e. $(TARGET_$(2)_BASENAME) is the name of the library to install. This
#       is also used as the name printed to the terminal for the build step.
#   NDK_SANITIZER_FSANITIZE_ARGS:
#       -fsanitize= arguments that require this runtime library.
#   NDK_SANITIZER_EXCLUDE_FSANITIZE_ARGS:
#       -fsanitize= arguments that exclude this runtime library. For example,
#       the UBSan runtime is included in the ASan runtime, so if we have both
#       the address and undefined sanitizers enabled, we only need to install
#       the ASan runtime.
#
# Example usage:
#   NDK_SANITIZER_NAME := UBSAN
#   NDK_SANITIZER_FSANITIZE_ARGS := undefined
#   include $(BUILD_SYSTEM)/install_sanitizer.mk

ifneq (,$(filter $(NDK_SANITIZER_FSANITIZE_ARGS),$(NDK_SANITIZERS)))
ifeq (,$(filter $(NDK_SANITIZER_EXCLUDE_FSANITIZE_ARGS),$(NDK_SANITIZERS)))
installed_modules: $(NDK_APP_$(NDK_SANITIZER_NAME))

NDK_SANITIZER_TARGET := $(NDK_APP_$(NDK_SANITIZER_NAME))
NDK_SANITIZER_LIB_PATH := $(NDK_TOOLCHAIN_LIB_DIR)/$(TARGET_$(NDK_SANITIZER_NAME)_BASENAME)

$(NDK_SANITIZER_TARGET): PRIVATE_ABI := $(TARGET_ARCH_ABI)
$(NDK_SANITIZER_TARGET): PRIVATE_NAME := $(NDK_SANITIZER_NAME)
$(NDK_SANITIZER_TARGET): PRIVATE_SRC := $(NDK_SANITIZER_LIB_PATH)
$(NDK_SANITIZER_TARGET): PRIVATE_DST := $(NDK_APP_$(NDK_SANITIZER_NAME))

$(call generate-file-dir,$(NDK_APP_$(NDK_SANITIZER_NAME)))

$(NDK_SANITIZER_TARGET): clean-installed-binaries
	$(call host-echo-build-step,$(PRIVATE_ABI),$(PRIVATE_NAME) "$(call pretty-dir,$(PRIVATE_DST))")
	$(hide) $(call host-install,$(PRIVATE_SRC),$(PRIVATE_DST))
endif
endif
