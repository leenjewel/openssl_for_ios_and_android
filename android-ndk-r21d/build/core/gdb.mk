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

# Ensure that for debuggable applications, gdbserver will be copied to
# the proper location

NDK_APP_GDBSERVER := $(NDK_APP_DST_DIR)/gdbserver
NDK_APP_GDBSETUP := $(NDK_APP_DST_DIR)/gdb.setup

ifeq ($(NDK_APP_DEBUGGABLE),true)
ifeq ($(TARGET_SONAME_EXTENSION),.so)

installed_modules: $(NDK_APP_GDBSERVER)

$(NDK_APP_GDBSERVER): PRIVATE_ABI     := $(TARGET_ARCH_ABI)
$(NDK_APP_GDBSERVER): PRIVATE_NAME    := $(TOOLCHAIN_NAME)
$(NDK_APP_GDBSERVER): PRIVATE_SRC     := $(TARGET_GDBSERVER)
$(NDK_APP_GDBSERVER): PRIVATE_DST     := $(NDK_APP_GDBSERVER)

$(call generate-file-dir,$(NDK_APP_GDBSERVER))

$(NDK_APP_GDBSERVER): clean-installed-binaries
	$(call host-echo-build-step,$(PRIVATE_ABI),Gdbserver) "[$(PRIVATE_NAME)] $(call pretty-dir,$(PRIVATE_DST))"
	$(hide) $(call host-install,$(PRIVATE_SRC),$(PRIVATE_DST))
endif

# Install gdb.setup for both .so and .bc projects
ifneq (,$(filter $(TARGET_SONAME_EXTENSION),.so .bc))
installed_modules: $(NDK_APP_GDBSETUP)

$(NDK_APP_GDBSETUP): PRIVATE_ABI := $(TARGET_ARCH_ABI)
$(NDK_APP_GDBSETUP): PRIVATE_DST := $(NDK_APP_GDBSETUP)
$(NDK_APP_GDBSETUP): PRIVATE_SOLIB_PATH := $(TARGET_OUT)
$(NDK_APP_GDBSETUP): PRIVATE_SRC_DIRS := $(SYSROOT_INC)

$(NDK_APP_GDBSETUP):
	$(call host-echo-build-step,$(PRIVATE_ABI),Gdbsetup) "$(call pretty-dir,$(PRIVATE_DST))"
	$(hide) $(HOST_ECHO) "set solib-search-path $(call host-path,$(PRIVATE_SOLIB_PATH))" > $(PRIVATE_DST)
	$(hide) $(HOST_ECHO) "directory $(call host-path,$(call remove-duplicates,$(PRIVATE_SRC_DIRS)))" >> $(PRIVATE_DST)

$(call generate-file-dir,$(NDK_APP_GDBSETUP))

# This prevents parallel execution to clear gdb.setup after it has been written to
$(NDK_APP_GDBSETUP): clean-installed-binaries
endif
endif
