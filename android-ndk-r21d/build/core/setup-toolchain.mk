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

# this file is included repeatedly from build/core/setup-abi.mk and is used
# to setup the target toolchain for a given platform/abi combination.
#

$(call assert-defined,TARGET_PLATFORM_LEVEL TARGET_ARCH TARGET_ARCH_ABI)
$(call assert-defined,NDK_APPS NDK_APP_STL)

# Check that we have a toolchain that supports the current ABI.
# NOTE: If NDK_TOOLCHAIN is defined, we're going to use it.
ifndef NDK_TOOLCHAIN
    # TODO: Remove all the multiple-toolchain configuration stuff. We only have
    # Clang.

    # This is a sorted list of toolchains that support the given ABI. For older
    # NDKs this was a bit more complicated, but now we just have the GCC and the
    # Clang toolchains with GCC being first (named "*-4.9", whereas clang is
    # "*-clang").
    TARGET_TOOLCHAIN_LIST := \
        $(strip $(sort $(NDK_ABI.$(TARGET_ARCH_ABI).toolchains)))

    ifneq ($(words $(TARGET_TOOLCHAIN_LIST)),1)
        $(call __ndk_error,Expected two items in TARGET_TOOLCHAIN_LIST, \
            found "$(TARGET_TOOLCHAIN_LIST)")
    endif

    ifndef TARGET_TOOLCHAIN_LIST
        $(call __ndk_info,There is no toolchain that supports the $(TARGET_ARCH_ABI) ABI.)
        $(call __ndk_info,Please modify the APP_ABI definition in $(NDK_APP_APPLICATION_MK) to use)
        $(call __ndk_info,a set of the following values: $(NDK_ALL_ABIS))
        $(call __ndk_error,Aborting)
    endif

    # We default to using Clang, which is the last item in the list.
    TARGET_TOOLCHAIN := $(lastword $(TARGET_TOOLCHAIN_LIST))

    $(call ndk_log,Using target toolchain '$(TARGET_TOOLCHAIN)' for '$(TARGET_ARCH_ABI)' ABI)
else # NDK_TOOLCHAIN is not empty
    TARGET_TOOLCHAIN_LIST := $(strip $(filter $(NDK_TOOLCHAIN),$(NDK_ABI.$(TARGET_ARCH_ABI).toolchains)))
    ifndef TARGET_TOOLCHAIN_LIST
        $(call __ndk_info,The selected toolchain ($(NDK_TOOLCHAIN)) does not support the $(TARGET_ARCH_ABI) ABI.)
        $(call __ndk_info,Please modify the APP_ABI definition in $(NDK_APP_APPLICATION_MK) to use)
        $(call __ndk_info,a set of the following values: $(NDK_TOOLCHAIN.$(NDK_TOOLCHAIN).abis))
        $(call __ndk_info,Or change your NDK_TOOLCHAIN definition.)
        $(call __ndk_error,Aborting)
    endif
    TARGET_TOOLCHAIN := $(NDK_TOOLCHAIN)
endif # NDK_TOOLCHAIN is not empty

TARGET_PREBUILT_SHARED_LIBRARIES :=

# Define default values for TOOLCHAIN_NAME, this can be overriden in
# the setup file.
TOOLCHAIN_NAME   := $(TARGET_TOOLCHAIN)
TOOLCHAIN_VERSION := $(call last,$(subst -,$(space),$(TARGET_TOOLCHAIN)))

# We expect the gdbserver binary for this toolchain to be located at its root.
TARGET_GDBSERVER := $(NDK_ROOT)/prebuilt/android-$(TARGET_ARCH)/gdbserver/gdbserver

# compute NDK_APP_DST_DIR as the destination directory for the generated files
NDK_APP_DST_DIR := $(NDK_APP_LIBS_OUT)/$(TARGET_ARCH_ABI)

# Default build commands, can be overriden by the toolchain's setup script
include $(BUILD_SYSTEM)/default-build-commands.mk

# now call the toolchain-specific setup script
include $(NDK_TOOLCHAIN.$(TARGET_TOOLCHAIN).setup)

# Setup sysroot variables.
#
# Note that these are not needed for the typical case of invoking Clang, as
# Clang already knows where the sysroot is relative to itself. We still need to
# manually refer to these in some places because other tools such as yasm and
# the renderscript compiler don't have this knowledge.

# SYSROOT_INC points to a directory that contains all public header files for a
# given platform.
ifndef NDK_UNIFIED_SYSROOT_PATH
    NDK_UNIFIED_SYSROOT_PATH := $(TOOLCHAIN_ROOT)/sysroot
endif

# TODO: Have the driver add the library path to -rpath-link.
SYSROOT_INC := $(NDK_UNIFIED_SYSROOT_PATH)

SYSROOT_LIB_DIR := $(NDK_UNIFIED_SYSROOT_PATH)/usr/lib/$(TOOLCHAIN_NAME)
SYSROOT_API_LIB_DIR := $(SYSROOT_LIB_DIR)/$(TARGET_PLATFORM_LEVEL)

# API-specific library directory comes first to make the linker prefer shared
# libs over static libs.
SYSROOT_LINK_ARG := -L $(SYSROOT_API_LIB_DIR) -L $(SYSROOT_LIB_DIR)

# Architecture specific headers like asm/ and machine/ are installed to an
# arch-$ARCH subdirectory of the sysroot.
SYSROOT_ARCH_INC_ARG := \
    -isystem $(SYSROOT_INC)/usr/include/$(TOOLCHAIN_NAME)

NDK_TOOLCHAIN_RESOURCE_DIR := $(shell $(TARGET_CXX) -print-resource-dir)
NDK_TOOLCHAIN_LIB_DIR := $(strip $(NDK_TOOLCHAIN_RESOURCE_DIR))/lib/linux

clean-installed-binaries::

include $(BUILD_SYSTEM)/gdb.mk

# free the dictionary of LOCAL_MODULE definitions
$(call modules-clear)

$(call ndk-stl-select,$(NDK_APP_STL))

# now parse the Android.mk for the application, this records all
# module declarations, but does not populate the dependency graph yet.
include $(NDK_APP_BUILD_SCRIPT)

# Avoid computing sanitizer/wrap.sh things in the DUMP_VAR case because both of
# these will create build rules and we want to avoid that. The DUMP_VAR case
# also doesn't parse the module definitions, so we're missing a lot of the
# information we need.
ifeq (,$(DUMP_VAR))
    # Comes after NDK_APP_BUILD_SCRIPT because we need to know if *any* module
    # has -fsanitize in its ldflags.
    include $(BUILD_SYSTEM)/sanitizers.mk
    include $(BUILD_SYSTEM)/openmp.mk

    ifneq ($(NDK_APP_WRAP_SH_$(TARGET_ARCH_ABI)),)
        include $(BUILD_SYSTEM)/install_wrap_sh.mk
    endif
endif

$(call ndk-stl-add-dependencies,$(NDK_APP_STL))

# recompute all dependencies between modules
$(call modules-compute-dependencies)

# for debugging purpose
ifdef NDK_DEBUG_MODULES
$(call modules-dump-database)
endif

# now, really build the modules, the second pass allows one to deal
# with exported values
$(foreach __pass2_module,$(__ndk_modules),\
    $(eval LOCAL_MODULE := $(__pass2_module))\
    $(eval include $(BUILD_SYSTEM)/build-binary.mk)\
)

# Now compute the closure of all module dependencies.
#
# If APP_MODULES is not defined in the Application.mk, we
# will build all modules that were listed from the top-level Android.mk
# and the installable imported ones they depend on
#
ifeq ($(strip $(NDK_APP_MODULES)),)
    WANTED_MODULES := $(call modules-get-all-installable,$(modules-get-top-list))
    ifeq (,$(strip $(WANTED_MODULES)))
        WANTED_MODULES := $(modules-get-top-list)
        $(call ndk_log,[$(TARGET_ARCH_ABI)] No installable modules in project - forcing static library build)
    endif
else
    WANTED_MODULES := $(call module-get-all-dependencies,$(NDK_APP_MODULES))
endif

$(call ndk_log,[$(TARGET_ARCH_ABI)] Modules to build: $(WANTED_MODULES))

WANTED_INSTALLED_MODULES += $(call map,module-get-installed,$(WANTED_MODULES))
