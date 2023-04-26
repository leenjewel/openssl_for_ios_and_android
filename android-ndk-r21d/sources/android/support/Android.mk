LOCAL_PATH := $(call my-dir)

# libandroid_support is only needed on LP32.
ifeq ($(filter $(NDK_KNOWN_DEVICE_ABI64S),$(TARGET_ARCH_ABI)),)

ifneq ($(LIBCXX_FORCE_REBUILD),true) # Using prebuilt

LIBCXX_LIBS := ../../cxx-stl/llvm-libc++/libs/$(TARGET_ARCH_ABI)

include $(CLEAR_VARS)
LOCAL_MODULE := android_support
LOCAL_SRC_FILES := $(LIBCXX_LIBS)/lib$(LOCAL_MODULE)$(TARGET_LIB_EXTENSION)
include $(PREBUILT_STATIC_LIBRARY)

else # Building

android_support_cflags := \
    -Drestrict=__restrict__ \
    -ffunction-sections \
    -fdata-sections \
    -fvisibility=hidden \

android_support_c_includes := \
    $(BIONIC_PATH)/libc \
    $(BIONIC_PATH)/libc/upstream-openbsd/android/include \
    $(BIONIC_PATH)/libm \
    $(BIONIC_PATH)/libm/upstream-freebsd/android/include \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src \

android_support_cflags += \
    -include freebsd-compat.h \
    -include openbsd-compat.h \
    -include $(LOCAL_PATH)/src/support_preinclude.h \
    -D__BIONIC_BUILD_FOR_ANDROID_SUPPORT \
    -Werror \

android_support_sources := \
    $(BIONIC_PATH)/libc/bionic/c32rtomb.cpp \
    $(BIONIC_PATH)/libc/bionic/locale.cpp \
    $(BIONIC_PATH)/libc/bionic/mbrtoc32.cpp \
    $(BIONIC_PATH)/libc/bionic/wchar.cpp \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcscat.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcschr.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcslen.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcsncmp.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcsncpy.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcspbrk.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcsrchr.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcsspn.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcsstr.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wcstok.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wmemchr.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wmemcmp.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wmemcpy.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wmemmove.c \
    $(BIONIC_PATH)/libc/upstream-freebsd/lib/libc/string/wmemset.c \
    $(BIONIC_PATH)/libc/upstream-openbsd/lib/libc/locale/mbtowc.c \
    $(BIONIC_PATH)/libc/upstream-openbsd/lib/libc/stdlib/imaxabs.c \
    $(BIONIC_PATH)/libc/upstream-openbsd/lib/libc/stdlib/imaxdiv.c \
    $(BIONIC_PATH)/libm/digittoint.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_acos.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_acosh.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_asin.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_atan2.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_atanh.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_cosh.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_exp.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_hypot.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_log.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_log10.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_log2.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_log2f.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_logf.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_remainder.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_sinh.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/e_sqrt.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/k_cos.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/k_exp.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/k_rem_pio2.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/k_sin.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/k_tan.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_asinh.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_atan.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_cbrt.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_cos.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_erf.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_exp2.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_expm1.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_frexp.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_frexpf.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_log1p.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_logb.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_nextafter.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_remquo.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_rint.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_sin.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_tan.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_tanh.c \
    src/locale_support.cpp \
    src/posix_memalign.cpp \
    src/swprintf.cpp \
    src/wcstox.cpp \

ifeq (x86,$(TARGET_ARCH_ABI))
# Replaces broken implementations in x86 libm.so
android_support_sources += \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_scalbln.c \
    $(BIONIC_PATH)/libm/upstream-freebsd/lib/msun/src/s_scalbn.c \

# fake_long_double.c doesn't define this for x86.
# TODO: seems like we don't pass .S files to the assembler?
#android_support_c_includes += $(BIONIC_PATH)/libc/arch-x86/include
#android_support_sources += $(BIONIC_PATH)/libm/x86/lrint.S
endif

include $(CLEAR_VARS)
LOCAL_MODULE := android_support
LOCAL_SRC_FILES := $(android_support_sources)
LOCAL_C_INCLUDES := $(android_support_c_includes)
LOCAL_CFLAGS := $(android_support_cflags)
LOCAL_ARM_NEON := false

LOCAL_CPPFLAGS := \
    -fvisibility-inlines-hidden \
    -std=c++11 \

include $(BUILD_STATIC_LIBRARY)

endif # Prebuilt/building

endif # LP32
