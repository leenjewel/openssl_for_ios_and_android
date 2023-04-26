#
# Copyright (C) 2016 The Android Open Source Project
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

# Note that the platform modules are defined in the Android.bp. This file is
# used for the NDK.

# If we're being invoked from ndk-build, we'll have NDK_ROOT defined.
ifdef NDK_ROOT

LOCAL_PATH := $(call my-dir)

# Defines a test module.
#
# The upstream gtest configuration builds each of these as separate executables.
# It's a pain for how we run tests in the platform, but we can handle that with
# a test running script.
#
# $(1): Test name. test/$(1).cc will automatically be added to sources.
# $(2): Additional source files.
# $(3): "libgtest_main" or empty.
#
# Use -Wno-unnamed-type-template-args because gtest_unittest.cc wants anonymous enum type.
define gtest-unit-test
    $(eval include $(CLEAR_VARS)) \
    $(eval LOCAL_MODULE := $(1)) \
    $(eval LOCAL_CPP_EXTENSION := .cc) \
    $(eval LOCAL_SRC_FILES := test/$(strip $(1)).cc $(2)) \
    $(eval LOCAL_C_INCLUDES := $(LOCAL_PATH)/include) \
    $(eval LOCAL_CPP_FEATURES := rtti) \
    $(eval LOCAL_CFLAGS := -Wall -Werror -Wno-sign-compare -Wno-unnamed-type-template-args) \
    $(eval LOCAL_CFLAGS += -Wno-unused-private-field) \
    $(eval LOCAL_STATIC_LIBRARIES := $(3) libgtest) \
    $(eval include $(BUILD_EXECUTABLE))
endef

# Create modules for each test in the suite.
#
# The NDK variant of gtest-death-test_test is disabled because we don't have
# pthread_atfork on android-9.
define gtest-test-suite
    $(eval $(call gtest-unit-test,googletest-death-test-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-filepath-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-listener-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-message-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-options-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-param-test-test, \
        test/googletest-param-test2-test.cc,)) \
    $(eval $(call gtest-unit-test,googletest-port-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-printers-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,googletest-test-part-test,,libgtest_main)) \
    $(eval $(call gtest-unit-test, \
        gtest-typed-test_test,test/gtest-typed-test2_test.cc, \
            libgtest_main)) \
    $(eval $(call gtest-unit-test,gtest-unittest-api_test,,)) \
    $(eval $(call gtest-unit-test,gtest_environment_test,,)) \
    $(eval $(call gtest-unit-test,gtest_main_unittest,,libgtest_main)) \
    $(eval $(call gtest-unit-test,gtest_no_test_unittest,,)) \
    $(eval $(call gtest-unit-test,gtest_pred_impl_unittest,,libgtest_main)) \
    $(eval $(call gtest-unit-test,gtest_premature_exit_test,,)) \
    $(eval $(call gtest-unit-test,gtest_prod_test,test/production.cc, \
            libgtest_main)) \
    $(eval $(call gtest-unit-test,gtest_repeat_test,,)) \
    $(eval $(call gtest-unit-test,gtest_skip_test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,gtest_sole_header_test,,libgtest_main)) \
    $(eval $(call gtest-unit-test,gtest_stress_test,,)) \
    $(eval $(call gtest-unit-test,gtest_unittest,,libgtest_main))
endef

# Test is disabled because Android doesn't build gtest with exceptions.
# $(eval $(call gtest-unit-test,gtest_throw_on_failure_ex_test,,))
# $(eval $(call gtest-unit-test,gtest_assert_by_exception_test,,))

include $(CLEAR_VARS)
LOCAL_MODULE := libgtest
LOCAL_SRC_FILES := src/gtest-all.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_CPP_FEATURES := rtti
include $(BUILD_STATIC_LIBRARY)

# Note: Unlike the platform, libgtest_main carries a dependency on libgtest.
# Users don't need to manually depend on both.
include $(CLEAR_VARS)
LOCAL_MODULE := libgtest_main
LOCAL_SRC_FILES := src/gtest_main.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CPP_FEATURES := rtti
LOCAL_STATIC_LIBRARIES := libgtest
include $(BUILD_STATIC_LIBRARY)

# These are the old names of these libraries. They don't match the platform or
# the upstream build, but we've been requiring that people put them in their NDK
# makefiles for years.

include $(CLEAR_VARS)
LOCAL_MODULE := googletest_static
LOCAL_SRC_FILES := src/gtest-all.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CPP_FEATURES := rtti
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libgoogletest_main
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := src/gtest_main.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CPP_FEATURES := rtti
LOCAL_STATIC_LIBRARIES := libgtest
include $(BUILD_STATIC_LIBRARY)

# The NDK used to include shared versions of these libraries, for some reason.

include $(CLEAR_VARS)
LOCAL_MODULE := googletest_shared
LOCAL_SRC_FILES := src/gtest-all.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include
LOCAL_CFLAGS := -DGTEST_CREATE_SHARED_LIBRARY
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CPP_FEATURES := rtti
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := googletest_main_shared
LOCAL_SRC_FILES := src/gtest_main.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CPP_FEATURES := rtti
LOCAL_SHARED_LIBRARIES := googletest_shared
include $(BUILD_STATIC_LIBRARY)

# Tests for use in the NDK itself.
$(call gtest-test-suite)

endif
