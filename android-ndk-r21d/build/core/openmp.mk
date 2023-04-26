#
# Copyright (C) 2019 The Android Open Source Project
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

# Generates rules to install the dynamic OpenMP runtime libraries to the out
# directory if the runtime is requested by the app's ldflags.

NDK_APP_NEEDS_DYNAMIC_OMP := false
$(foreach __module,$(__ndk_modules),\
    $(eval __module_ldflags := \
        $(NDK_APP_LDFLAGS) $(__ndk_modules.$(__module).LDFLAGS))\
    $(if $(filter -fopenmp,$(__module_ldflags)),\
        $(if $(filter -static-openmp,$(__module_ldflags)),,\
            $(eval NDK_APP_NEEDS_DYNAMIC_OMP := true)\
        )\
    )\
)

ifeq ($(NDK_APP_NEEDS_DYNAMIC_OMP),true)
NDK_APP_OMP := $(NDK_APP_DST_DIR)/libomp.so
installed_modules: $(NDK_APP_OMP)

NDK_OMP_LIB_PATH := \
    $(NDK_TOOLCHAIN_LIB_DIR)/$(TARGET_TOOLCHAIN_ARCH_LIB_DIR)/libomp.so

$(NDK_APP_OMP): PRIVATE_ABI := $(TARGET_ARCH_ABI)
$(NDK_APP_OMP): PRIVATE_SRC := $(NDK_OMP_LIB_PATH)
$(NDK_APP_OMP): PRIVATE_DST := $(NDK_APP_OMP)

$(call generate-file-dir,$(NDK_APP_$(NDK_SANITIZER_NAME)))

$(NDK_APP_OMP): clean-installed-binaries
	$(call host-echo-build-step,$(PRIVATE_ABI),OpenMP "$(call pretty-dir,$(PRIVATE_DST))")
	$(hide) $(call host-install,$(PRIVATE_SRC),$(PRIVATE_DST))
endif
