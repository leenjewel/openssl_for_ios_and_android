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

# this script is used to record an application definition in the
# NDK build system, before performing any build whatsoever.
#
# It is included repeatedly from build/core/main.mk and expects a
# variable named '_application_mk' which points to a given Application.mk
# file that will be included here. The latter must define a few variables
# to describe the application to the build system, and the rest of the
# code here will perform book-keeping and basic checks
#

$(call assert-defined, _application_mk _app)
$(call ndk_log,Parsing $(_application_mk))

$(call clear-vars, $(NDK_APP_VARS))

# Check that NDK_DEBUG is properly defined. If it is
# the only valid states are: undefined, 0, 1, false and true
#
# We set APP_DEBUG to <undefined>, 'true' or 'false'.
#
APP_DEBUG := $(strip $(NDK_DEBUG))
ifeq ($(APP_DEBUG),0)
  APP_DEBUG:= false
endif
ifeq ($(APP_DEBUG),1)
  APP_DEBUG := true
endif
ifdef APP_DEBUG
  ifneq (,$(filter-out true false,$(APP_DEBUG)))
    $(call __ndk_warning,NDK_DEBUG is defined to the unsupported value '$(NDK_DEBUG)', will be ignored!)
  endif
endif

include $(_application_mk)

$(call check-required-vars,$(NDK_APP_VARS_REQUIRED),$(_application_mk))

_map := NDK_APP.$(_app)

# strip the 'lib' prefix in front of APP_MODULES modules
APP_MODULES := $(call strip-lib-prefix,$(APP_MODULES))

APP_PROJECT_PATH := $(strip $(APP_PROJECT_PATH))
ifndef APP_PROJECT_PATH
    APP_PROJECT_PATH := $(NDK_PROJECT_PATH)
endif

include $(BUILD_SYSTEM)/setup-app-platform.mk

# Check that the value of APP_ABI corresponds to known ABIs
# 'all' is a special case that means 'all supported ABIs'
#
# It will be handled in setup-app.mk. We can't hope to change
# the value of APP_ABI is the user enforces it on the command-line
# with a call like:  ndk-build APP_ABI=all
#
# Because GNU Make makes the APP_ABI variable read-only (any assignments
# to it will be ignored)
#
APP_ABI := $(subst $(comma),$(space),$(strip $(APP_ABI)))
ifndef APP_ABI
    APP_ABI := $(NDK_DEFAULT_ABIS)
endif

# If APP_BUILD_SCRIPT is defined, check that the file exists.
# If undefined, look in $(APP_PROJECT_PATH)/jni/Android.mk
#
APP_BUILD_SCRIPT := $(strip $(APP_BUILD_SCRIPT))
ifdef APP_BUILD_SCRIPT
    _build_script := $(strip $(wildcard $(APP_BUILD_SCRIPT)))
    ifndef _build_script
        $(call __ndk_info,Your APP_BUILD_SCRIPT points to an unknown file: $(APP_BUILD_SCRIPT))
        $(call __ndk_error,Aborting...)
    endif
    APP_BUILD_SCRIPT := $(_build_script)
    $(call ndk_log,  Using build script $(APP_BUILD_SCRIPT))
else
    ifeq (null,$(APP_PROJECT_PATH))
      $(call __ndk_info,NDK_PROJECT_PATH==null.  Please explicitly set APP_BUILD_SCRIPT.)
      $(call __ndk_error,Aborting.)
    endif

    _build_script := $(strip $(wildcard $(APP_PROJECT_PATH)/jni/Android.mk))
    ifndef _build_script
        $(call __ndk_info,There is no Android.mk under $(APP_PROJECT_PATH)/jni)
        $(call __ndk_info,If this is intentional, please define APP_BUILD_SCRIPT to point)
        $(call __ndk_info,to a valid NDK build script.)
        $(call __ndk_error,Aborting...)
    endif
    APP_BUILD_SCRIPT := $(_build_script)
    $(call ndk_log,  Defaulted to APP_BUILD_SCRIPT=$(APP_BUILD_SCRIPT))
endif

# Determine whether the application should be debuggable.
# - If APP_DEBUG is set to 'true', then it always should.
# - If APP_DEBUG is set to 'false', then it never should
# - Otherwise, extract the android:debuggable attribute from the manifest.
#
ifdef APP_DEBUG
  APP_DEBUGGABLE := $(APP_DEBUG)
  ifeq ($(NDK_LOG),1)
    ifeq ($(APP_DEBUG),true)
      $(call ndk_log,Application '$(_app)' forced debuggable through NDK_DEBUG)
    else
      $(call ndk_log,Application '$(_app)' forced *not* debuggable through NDK_DEBUG)
    endif
  endif
else
  # NOTE: To make unit-testing simpler, handle the case where there is no manifest.
  APP_DEBUGGABLE := false
  ifdef APP_MANIFEST
    APP_DEBUGGABLE := $(shell $(HOST_PYTHON) $(BUILD_PY)/extract_manifest.py debuggable $(call host-path,$(APP_MANIFEST)))
  endif
  ifeq ($(NDK_LOG),1)
    ifeq ($(APP_DEBUGGABLE),true)
      $(call ndk_log,Application '$(_app)' *is* debuggable)
    else
      $(call ndk_log,Application '$(_app)' is not debuggable)
    endif
  endif
endif

# LOCAL_BUILD_MODE will be either release or debug
#
# If APP_OPTIM is defined in the Application.mk, just use this.
#
# Otherwise, set to 'debug' if android:debuggable is set to TRUE,
# and to 'release' if not.
#
ifneq ($(APP_OPTIM),)
    # check that APP_OPTIM, if defined, is either 'release' or 'debug'
    $(if $(filter-out release debug,$(APP_OPTIM)),\
        $(call __ndk_info, The APP_OPTIM defined in $(_application_mk) must only be 'release' or 'debug')\
        $(call __ndk_error,Aborting)\
    )
    $(call ndk_log,Selecting optimization mode through Application.mk: $(APP_OPTIM))
else
    ifeq ($(APP_DEBUGGABLE),true)
        $(call ndk_log,Selecting debug optimization mode (app is debuggable))
        APP_OPTIM := debug
    else
        $(call ndk_log,Selecting release optimization mode (app is not debuggable))
        APP_OPTIM := release
    endif
endif

APP_CFLAGS := $(strip $(APP_CFLAGS))
APP_CONLYFLAGS := $(strip $(APP_CONLYFLAGS))
APP_CPPFLAGS := $(strip $(APP_CPPFLAGS))
APP_CXXFLAGS := $(strip $(APP_CXXFLAGS))
APP_RENDERSCRIPT_FLAGS := $(strip $(APP_RENDERSCRIPT_FLAGS))
APP_ASFLAGS := $(strip $(APP_ASFLAGS))
APP_ASMFLAGS := $(strip $(APP_ASMFLAGS))
APP_LDFLAGS  := $(strip $(APP_LDFLAGS))

# Check that APP_STL is defined. If not, use the default value (system)
# otherwise, check that the name is correct.
APP_STL := $(strip $(APP_STL))
ifndef APP_STL
    APP_STL := system
else
    ifneq ($(filter $(APP_STL),gnustl_static gnustl_shared stlport_static stlport_shared),)
        $(call __ndk_error,APP_STL $(APP_STL) is no longer supported. Please \
            switch to either c++_static or c++_shared. See \
            https://developer.android.com/ndk/guides/cpp-support.html for more \
            information.)
    endif
    $(call ndk-stl-check,$(APP_STL))
endif

# wrap.sh files can be specified in the user's Application.mk in either an
# ABI-generic (APP_WRAP_SH) or ABI-specific (APP_WRAP_SH_x86, etc) fashion.
# These two approaches cannot be combined; if any ABI-specific wrap.sh files are
# specified then it is an error to also specify an ABI-generic one.
#
# After this block, only the ABI-specific values should be checked; if there is
# an ABI-generic script specified the ABI-specific variables will be populated
# with the generic script.
NDK_NO_USER_WRAP_SH := true
ifneq ($(APP_WRAP_SH),)
    NDK_NO_USER_WRAP_SH := false
endif

NDK_HAVE_ABI_SPECIFIC_WRAP_SH := false
$(foreach _abi,$(NDK_ALL_ABIS),\
    $(if $(APP_WRAP_SH_$(_abi)),\
        $(eval NDK_HAVE_ABI_SPECIFIC_WRAP_SH := true)))

ifeq ($(NDK_HAVE_ABI_SPECIFIC_WRAP_SH),true)
    # It is an error to have both ABI-specific and ABI-generic wrap.sh files
    # specified.
    ifneq ($(APP_WRAP_SH),)
        $(call __ndk_error,Found both ABI-specific and ABI-generic APP_WRAP_SH \
            directives. Must use either all ABI-specific or only ABI-generic.)
    endif
    NDK_NO_USER_WRAP_SH := false
else
    # If we have no ABI-specific wrap.sh files but we *do* have an ABI-generic
    # one, install the generic one for all ABIs.
    $(foreach _abi,$(NDK_ALL_ABIS),\
        $(eval APP_WRAP_SH_$(_abi) := $(APP_WRAP_SH)))
endif

# Stripping can be configured both at the app (APP_STRIP_MODE) and module level
# (LOCAL_STRIP_MODE). The module setting always overrides the application
# setting.
#
# This value is passed as-is as the flag to the strip command except when it is
# set to the special value "none". If set to "none", the binary will not be
# stripped at all.
ifeq ($(APP_STRIP_MODE),)
    # The strip command is only used for shared libraries and executables. It is
    # thus safe to use --strip-unneeded, which is only dangerous when applied to
    # static libraries or object files.
    APP_STRIP_MODE := --strip-unneeded
endif

$(if $(call get,$(_map),defined),\
  $(call __ndk_info,Weird, the application $(_app) is already defined by $(call get,$(_map),defined))\
  $(call __ndk_error,Aborting)\
)

$(call set,$(_map),defined,$(_application_mk))

# Record all app-specific variable definitions
$(foreach __name,$(NDK_APP_VARS),\
  $(call set,$(_map),$(__name),$($(__name)))\
)

# Record the Application.mk for debugging
$(call set,$(_map),Application.mk,$(_application_mk))

NDK_ALL_APPS += $(_app)
