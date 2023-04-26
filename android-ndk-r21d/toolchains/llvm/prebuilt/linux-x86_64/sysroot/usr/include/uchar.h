/*
 * Copyright (C) 2014 The Android Open Source Project
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
 * @file uchar.h
 * @brief Unicode functions.
 */

#include <stddef.h>
#include <sys/cdefs.h>
#include <bits/mbstate_t.h>

__BEGIN_DECLS

#if !defined(__cplusplus)
/** The UTF-16 character type. */
typedef __CHAR16_TYPE__ char16_t;
/** The UTF-32 character type. */
typedef __CHAR32_TYPE__ char32_t;
#endif

/** On Android, char16_t is UTF-16. */
#define __STD_UTF_16__ 1

/** On Android, char32_t is UTF-32. */
#define __STD_UTF_32__ 1

/**
 * [c16rtomb(3)](http://man7.org/linux/man-pages/man3/c16rtomb.3.html) converts a single UTF-16
 * character to UTF-8.
 *
 * Returns the number of bytes written to `__buf` on success, and returns -1 and sets `errno`
 * on failure.
 *
 * Available since API level 21.
 */

#if __ANDROID_API__ >= 21
size_t c16rtomb(char* _Nullable __buf, char16_t __ch16, mbstate_t* _Nullable __ps) __INTRODUCED_IN(21);

/**
 * [c32rtomb(3)](http://man7.org/linux/man-pages/man3/c32rtomb.3.html) converts a single UTF-32
 * character to UTF-8.
 *
 * Returns the number of bytes written to `__buf` on success, and returns -1 and sets `errno`
 * on failure.
 *
 * Available since API level 21.
 */
size_t c32rtomb(char* _Nullable __buf, char32_t __ch32, mbstate_t* _Nullable __ps) __INTRODUCED_IN(21);

/**
 * [mbrtoc16(3)](http://man7.org/linux/man-pages/man3/mbrtoc16.3.html) converts the next UTF-8
 * sequence to a UTF-16 code point.
 *
 * Available since API level 21.
 */
size_t mbrtoc16(char16_t* _Nullable __ch16, const char* _Nullable __s, size_t __n, mbstate_t* _Nullable __ps) __INTRODUCED_IN(21);

/**
 * [mbrtoc32(3)](http://man7.org/linux/man-pages/man3/mbrtoc32.3.html) converts the next UTF-8
 * sequence to a UTF-32 code point.
 *
 * Available since API level 21.
 */
size_t mbrtoc32(char32_t* _Nullable __ch32, const char* _Nullable __s, size_t __n, mbstate_t* _Nullable __ps) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


__END_DECLS
