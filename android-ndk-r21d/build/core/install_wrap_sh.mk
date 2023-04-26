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

# Generates rules to install wrap.sh files to the app's out directory.

NDK_WRAP_SH := $(NDK_APP_DST_DIR)/wrap.sh
$(call generate-file-dir,$(NDK_WRAP_SH))

installed_modules: $(NDK_WRAP_SH)

WRAP_SH_SRC := $(call local-source-file-path,$(NDK_APP_WRAP_SH_$(TARGET_ARCH_ABI)))

$(NDK_WRAP_SH): PRIVATE_ABI := $(TARGET_ARCH_ABI)
$(NDK_WRAP_SH): $(WRAP_SH_SRC) clean-installed-binaries
	$(call host-echo-build-step,$(PRIVATE_ABI),wrap.sh "$(call pretty-dir,$@)")
	$(hide) $(call host-install,$<,$@)
