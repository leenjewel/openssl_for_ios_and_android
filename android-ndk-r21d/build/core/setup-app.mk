# Copyright (C) 2009 The Android Open Source Project
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

# this file is included repeatedly from build/core/main.mk
# and is used to prepare for app-specific build rules.
#

$(call assert-defined,_app)

_map := NDK_APP.$(_app)

# ok, let's parse all Android.mk source files in order to build
# the modules for this app.
#

# Restore the APP_XXX variables just for this pass as NDK_APP_XXX
#
NDK_APP_NAME           := $(_app)
NDK_APP_APPLICATION_MK := $(call get,$(_map),Application.mk)

$(foreach __name,$(NDK_APP_VARS),\
  $(eval NDK_$(__name) := $(call get,$(_map),$(__name)))\
)

# make the application depend on the modules it requires
.PHONY: ndk-app-$(_app)
ndk-app-$(_app): $(NDK_APP_MODULES)
all: ndk-app-$(_app)

# The ABI(s) to use
NDK_APP_ABI := $(subst $(comma),$(space),$(strip $(NDK_APP_ABI)))
ifndef NDK_APP_ABI
    NDK_APP_ABI := $(NDK_DEFAULT_ABIS)
endif

NDK_ABI_FILTER := $(strip $(NDK_ABI_FILTER))
ifdef NDK_ABI_FILTER
    $(eval $(NDK_ABI_FILTER))
endif

# If APP_ABI is 'all', then set it to all supported ABIs
# Otherwise, check that we don't have an invalid value here.
#
ifeq ($(NDK_APP_ABI),all)
    NDK_APP_ABI := $(NDK_APP_ABI_ALL_EXPANDED)
else ifeq ($(NDK_APP_ABI),all32)
    NDK_APP_ABI := $(NDK_APP_ABI_ALL32_EXPANDED)
else ifeq ($(NDK_APP_ABI),all64)
    NDK_APP_ABI := $(NDK_APP_ABI_ALL64_EXPANDED)
else
    # check the target ABIs for this application
    _bad_abis = $(strip $(filter-out $(NDK_ALL_ABIS),$(NDK_APP_ABI)))
    ifneq ($(_bad_abis),)
        ifneq ($(filter $(_bad_abis),armeabi-v7a-hard),)
            $(call __ndk_info,armeabi-v7a-hard is no longer supported. Use armeabi-v7a.)
            $(call __ndk_info,See https://android.googlesource.com/platform/ndk/+/master/docs/HardFloatAbi.md)
        else ifneq ($(filter $(_bad_abis),armeabi),)
            $(call __ndk_info,The armeabi ABI is no longer supported. Use armeabi-v7a.)
        else ifneq ($(filter $(_bad_abis),mips mips64),)
            $(call __ndk_info,MIPS and MIPS64 are no longer supported.)
        endif
        $(call __ndk_info,NDK Application '$(_app)' targets unknown ABI(s): $(_bad_abis))
        $(call __ndk_info,Please fix the APP_ABI definition in $(NDK_APP_APPLICATION_MK))
        $(call __ndk_error,Aborting)
    endif
endif

_deprecated_abis := $(filter $(NDK_DEPRECATED_ABIS),$(NDK_APP_ABI))
ifneq ($(_deprecated_abis),)
    $(call __ndk_warning,Application targets deprecated ABI(s): $(_deprecated_abis))
    $(call __ndk_warning,Support for these ABIs will be removed in a future NDK release.)
endif

# Clear all installed binaries for this application. This ensures that if the
# build fails or if you remove a module, you're not going to mistakenly package
# an obsolete version.
#
# Historically this would clear every ABI, meaning that the following workflow
# would leave only x86_64 present in the lib dir on completion:
#
#     for abi in armeabi-v7a arm64-v8a x86 x86_64; do
#         ndk-build APP_ABI=$abi
#     done
#
# This is the workflow used by gradle. They currently override NDK_ALL_ABIS (an
# internal variable) to workaround this behavior. Changing this behavior allows
# them to remove their workaround and stop clobbering our implementation
# details.
ifeq ($(NDK_APP.$(_app).cleaned_binaries),)
    NDK_APP.$(_app).cleaned_binaries := true

clean-installed-binaries::
	$(hide) $(call host-rm,$(NDK_APP_ABI:%=$(NDK_APP_LIBS_OUT)/%/*))
	$(hide) $(call host-rm,$(NDK_APP_ABI:%=$(NDK_APP_LIBS_OUT)/%/gdbserver))
	$(hide) $(call host-rm,$(NDK_APP_ABI:%=$(NDK_APP_LIBS_OUT)/%/gdb.setup))
endif

# Renderscript

RENDERSCRIPT_TOOLCHAIN_PREBUILT_ROOT := \
    $(NDK_ROOT)/toolchains/renderscript/prebuilt/$(HOST_TAG64)
RENDERSCRIPT_TOOLCHAIN_PREFIX := $(RENDERSCRIPT_TOOLCHAIN_PREBUILT_ROOT)/bin/
RENDERSCRIPT_TOOLCHAIN_HEADER := $(RENDERSCRIPT_TOOLCHAIN_PREBUILT_ROOT)/clang-include
RENDERSCRIPT_PLATFORM_HEADER := $(RENDERSCRIPT_TOOLCHAIN_PREBUILT_ROOT)/platform/rs

COMPILE_COMMANDS_JSON := $(call host-path,compile_commands.json)
sub_commands_json :=

# Each ABI
$(foreach _abi,$(NDK_APP_ABI),\
    $(eval TARGET_ARCH_ABI := $(_abi))\
    $(eval include $(BUILD_SYSTEM)/setup-abi.mk) \
)

_sub_commands_arg := $(sub_commands_json)

ifeq ($(LOCAL_SHORT_COMMANDS),true)
compile_commands_list_file := $(NDK_APP_OUT)/compile_commands.list
_sub_commands_arg := @$(compile_commands_list_file)
$(compile_commands_list_file): $(sub_commands_json)
$(call generate-list-file,$(sub_commands_json),$(compile_commands_list_file))
endif

$(COMPILE_COMMANDS_JSON): PRIVATE_SUB_COMMANDS := $(_sub_commands_arg)
$(COMPILE_COMMANDS_JSON): $(compile_commands_list_file) $(sub_commands_json)
	$(hide) $(HOST_PYTHON) $(BUILD_PY)/gen_compile_db.py -o $@ \
        $(PRIVATE_SUB_COMMANDS)
