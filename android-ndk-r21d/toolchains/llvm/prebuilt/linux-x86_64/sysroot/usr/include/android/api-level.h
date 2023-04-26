/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

/**
 * @file android/api-level.h
 * @brief Functions and constants for dealing with multiple API levels.
 *
 * See
 * https://android.googlesource.com/platform/bionic/+/master/docs/defines.md.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * Magic version number for an Android OS build which has not yet turned
 * into an official release, for comparison against `__ANDROID_API__`. See
 * https://android.googlesource.com/platform/bionic/+/master/docs/defines.md.
 */
#define __ANDROID_API_FUTURE__ 10000

/* This #ifndef should never be true except when doxygen is generating docs. */
#ifndef __ANDROID_API__
/**
 * `__ANDROID_API__` is the API level being targeted. For the OS,
 * this is `__ANDROID_API_FUTURE__`. For the NDK, this is set by the
 * compiler system based on the API level you claimed to target.
 */
#define __ANDROID_API__ __ANDROID_API_FUTURE__
#endif

#if __ANDROID_API__ != __ANDROID_API_FUTURE__
/**
 * `__ANDROID_NDK__` is defined for code that's built by the NDK
 * rather than as part of the OS. "Built by the NDK" is an ambiguous idea,
 * so you might prefer to check `__ANDROID__`, `__BIONIC__`, `__linux__`,
 * or `__NDK_MAJOR__` instead, depending on exactly what you're trying to say.
 *
 * `__ANDROID_NDK__` is intended to capture "this code is being built for
 * Android, but targeting a specific API level". This is true for all code built
 * by the NDK proper, but also for OS code that sets `sdk_version` in its build
 * file. This distinction might matter to you if, for example, your code could
 * be built as part of an app *or* as part of the OS.
 */
#define __ANDROID_NDK__ 1
#endif

/** Names the Gingerbread API level (9), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_G__ 9

/** Names the Ice-Cream Sandwich API level (14), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_I__ 14

/** Names the Jellybean API level (16), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_J__ 16

/** Names the Jellybean MR1 API level (17), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_J_MR1__ 17

/** Names the Jellybean MR2 API level (18), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_J_MR2__ 18

/** Names the KitKat API level (19), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_K__ 19

/** Names the Lollipop API level (21), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_L__ 21

/** Names the Lollipop MR1 API level (22), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_L_MR1__ 22

/** Names the Marshmallow API level (23), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_M__ 23

/** Names the Nougat API level (24), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_N__ 24

/** Names the Nougat MR1 API level (25), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_N_MR1__ 25

/** Names the Oreo API level (26), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_O__ 26

/** Names the Oreo MR1 API level (27), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_O_MR1__ 27

/** Names the Pie API level (28), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_P__ 28

/**
 * Names the "Q" API level (29), for comparison against `__ANDROID_API__`.
 * This release was called Android 10 publicly, not to be (but sure to be)
 * confused with API level 10.
 */
#define __ANDROID_API_Q__ 29

/** Names the "R" API level (30), for comparison against `__ANDROID_API__`. */
#define __ANDROID_API_R__ 30

/**
 * Returns the `targetSdkVersion` of the caller, or `__ANDROID_API_FUTURE__`
 * if there is no known target SDK version (for code not running in the
 * context of an app).
 *
 * The returned values correspond to the named constants in `<android/api-level.h>`,
 * and is equivalent to the AndroidManifest.xml `targetSdkVersion`.
 *
 * See also android_get_device_api_level().
 *
 * Available since API level 24.
 */

#if __ANDROID_API__ >= 24
int android_get_application_target_sdk_version() __INTRODUCED_IN(24);
#endif /* __ANDROID_API__ >= 24 */


#if __ANDROID_API__ < 29

// android_get_device_api_level is a static inline before API level 29.
#define __BIONIC_GET_DEVICE_API_LEVEL_INLINE static __inline
#include <bits/get_device_api_level_inlines.h>
#undef __BIONIC_GET_DEVICE_API_LEVEL_INLINE

#else

/**
 * Returns the API level of the device we're actually running on, or -1 on failure.
 * The returned values correspond to the named constants in `<android/api-level.h>`,
 * and is equivalent to the Java `Build.VERSION.SDK_INT` API.
 *
 * See also android_get_application_target_sdk_version().
 */
int android_get_device_api_level() __INTRODUCED_IN(29);

#endif

__END_DECLS
