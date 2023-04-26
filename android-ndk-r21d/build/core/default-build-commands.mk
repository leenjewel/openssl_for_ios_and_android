# The following definitions are the defaults used by all toolchains.
# This is included in setup-toolchain.mk just before the inclusion
# of the toolchain's specific setup.mk file which can then override
# these definitions.
#

# These flags are used to ensure that a binary doesn't reference undefined
# flags.
TARGET_NO_UNDEFINED_LDFLAGS := -Wl,--no-undefined


# Return the list of object, static libraries and shared libraries as they
# must appear on the final static linker command (order is important).
#
# This can be over-ridden by a specific toolchain. Note that by default
# we always put libgcc _after_ all static libraries and _before_ shared
# libraries. This ensures that any libgcc function used by the final
# executable will be copied into it. Otherwise, it could contain
# symbol references to the same symbols as exported by shared libraries
# and this causes binary compatibility problems when they come from
# system libraries (e.g. libc.so and others).
#
# IMPORTANT: The result must use the host path convention.
#
# $1: object files
# $2: static libraries
# $3: whole static libraries
# $4: shared libraries
#
TARGET-get-linker-objects-and-libraries = \
    $(call host-path, $1) \
    $(call link-whole-archives,$3) \
    $(call host-path, $2) \
    $(PRIVATE_LIBGCC) \
    $(PRIVATE_LIBATOMIC) \
    $(call host-path, $4) \

# This flag are used to provide compiler protection against format
# string vulnerabilities.
TARGET_FORMAT_STRING_CFLAGS := -Wformat -Werror=format-security

# This flag disables the above security checks
TARGET_DISABLE_FORMAT_STRING_CFLAGS := -Wno-error=format-security

# NOTE: Ensure that TARGET_LIBGCC is placed after all private objects
#       and static libraries, but before any other library in the link
#       command line when generating shared libraries and executables.
#
#       This ensures that all libgcc.a functions required by the target
#       will be included into it, instead of relying on what's available
#       on other libraries like libc.so, which may change between system
#       releases due to toolchain or library changes.
#
define cmd-build-shared-library
$(PRIVATE_CXX) \
    -Wl,-soname,$(notdir $(LOCAL_BUILT_MODULE)) \
    -shared \
    $(PRIVATE_LINKER_OBJECTS_AND_LIBRARIES) \
    $(GLOBAL_LDFLAGS) \
    $(PRIVATE_LDFLAGS) \
    $(PRIVATE_LDLIBS) \
    -o $(call host-path,$(LOCAL_BUILT_MODULE))
endef

# The following -rpath-link= are needed for ld.bfd (default for ARM64) when
# linking executables to supress warning about missing symbol from libraries not
# directly needed. ld.gold (default for all other architectures) doesn't emulate
# this buggy behavior.
define cmd-build-executable
$(PRIVATE_CXX) \
    -Wl,--gc-sections \
    -Wl,-rpath-link=$(call host-path,$(PRIVATE_SYSROOT_API_LIB_DIR)) \
    -Wl,-rpath-link=$(call host-path,$(TARGET_OUT)) \
    $(PRIVATE_LINKER_OBJECTS_AND_LIBRARIES) \
    $(GLOBAL_LDFLAGS) \
    $(PRIVATE_LDFLAGS) \
    $(PRIVATE_LDLIBS) \
    -o $(call host-path,$(LOCAL_BUILT_MODULE))
endef

define cmd-build-static-library
$(PRIVATE_AR) $(call host-path,$(LOCAL_BUILT_MODULE)) $(PRIVATE_AR_OBJECTS)
endef

cmd-strip = $(PRIVATE_STRIP) $(PRIVATE_STRIP_MODE) $(call host-path,$1)

# arm32 currently uses a linker script in place of libgcc to ensure that
# libunwind is linked in the correct order. --exclude-libs does not propagate to
# the contents of the linker script and can't be specified within the linker
# script. Hide both regardless of architecture to future-proof us in case we
# move other architectures to a linker script (which we may want to do so we
# automatically link libclangrt on other architectures).
TARGET_LIBGCC = -lgcc -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libgcc_real.a
TARGET_LIBATOMIC = -latomic -Wl,--exclude-libs,libatomic.a
TARGET_LDLIBS := -lc -lm

TOOLCHAIN_ROOT := $(NDK_ROOT)/toolchains/llvm/prebuilt/$(HOST_TAG64)
LLVM_TOOLCHAIN_PREFIX := $(TOOLCHAIN_ROOT)/bin/

# IMPORTANT: The following definitions must use lazy assignment because
# the value of TOOLCHAIN_NAME or TARGET_CFLAGS can be changed later by
# the toolchain's setup.mk script.
TOOLCHAIN_PREFIX = $(TOOLCHAIN_ROOT)/bin/$(TOOLCHAIN_NAME)-

ifneq ($(findstring ccc-analyzer,$(CC)),)
    TARGET_CC = $(CC)
else
    TARGET_CC = $(LLVM_TOOLCHAIN_PREFIX)clang$(HOST_EXEEXT)
endif

CLANG_TIDY = $(LLVM_TOOLCHAIN_PREFIX)clang-tidy$(HOST_EXEEXT)

GLOBAL_CFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -fdata-sections \
    -ffunction-sections \
    -fstack-protector-strong \
    -funwind-tables \
    -no-canonical-prefixes \

# This is unnecessary given the new toolchain layout, but Studio will not
# recognize this as an Android build if there is no --sysroot flag.
# TODO: Teach Studio to recognize Android builds based on --target.
GLOBAL_CFLAGS += --sysroot $(call host-path,$(NDK_UNIFIED_SYSROOT_PATH))

# Always enable debug info. We strip binaries when needed.
GLOBAL_CFLAGS += -g

# TODO: Remove.
GLOBAL_CFLAGS += \
    -Wno-invalid-command-line-argument \
    -Wno-unused-command-line-argument \

GLOBAL_CFLAGS += -D_FORTIFY_SOURCE=2

GLOBAL_LDFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -no-canonical-prefixes \

GLOBAL_CXXFLAGS = $(GLOBAL_CFLAGS) -fno-exceptions -fno-rtti

TARGET_CFLAGS =
TARGET_CONLYFLAGS =
TARGET_CXXFLAGS = $(TARGET_CFLAGS)

ifneq ($(findstring c++-analyzer,$(CXX)),)
    TARGET_CXX = $(CXX)
else
    TARGET_CXX = $(LLVM_TOOLCHAIN_PREFIX)clang++$(HOST_EXEEXT)
endif

TARGET_RS_CC    = $(RENDERSCRIPT_TOOLCHAIN_PREFIX)llvm-rs-cc
TARGET_RS_BCC   = $(RENDERSCRIPT_TOOLCHAIN_PREFIX)bcc_compat
TARGET_RS_FLAGS = -Wall -Werror
ifeq (,$(findstring 64,$(TARGET_ARCH_ABI)))
TARGET_RS_FLAGS += -m32
else
TARGET_RS_FLAGS += -m64
endif

TARGET_ASM      = $(TOOLCHAIN_ROOT)/bin/yasm
TARGET_ASMFLAGS =

TARGET_LD       = $(TOOLCHAIN_PREFIX)ld
TARGET_LDFLAGS :=

TARGET_AR = $(TOOLCHAIN_PREFIX)ar
TARGET_ARFLAGS := crsD

TARGET_STRIP    = $(TOOLCHAIN_PREFIX)strip

TARGET_OBJ_EXTENSION := .o
TARGET_LIB_EXTENSION := .a
TARGET_SONAME_EXTENSION := .so
