/*
 * Copyright (C) 2016 The Android Open Source Project
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
 * @file bits/strcasecmp.h
 * @brief Case-insensitive string comparison.
 */

#include <sys/cdefs.h>
#include <sys/types.h>
#include <xlocale.h>

__BEGIN_DECLS

/**
 * [strcasecmp(3)](http://man7.org/linux/man-pages/man3/strcasecmp.3.html) compares two strings
 * ignoring case.
 *
 * Returns an integer less than, equal to, or greater than zero if the first string is less than,
 * equal to, or greater than the second string (ignoring case).
 */
int strcasecmp(const char* __s1, const char* __s2) __attribute_pure__;

/**
 * Like strcasecmp() but taking a `locale_t`.
 */

#if __ANDROID_API__ >= 23
int strcasecmp_l(const char* __s1, const char* __s2, locale_t __l) __attribute_pure__ __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


/**
 * [strncasecmp(3)](http://man7.org/linux/man-pages/man3/strncasecmp.3.html) compares the first
 * `n` bytes of two strings ignoring case.
 *
 * Returns an integer less than, equal to, or greater than zero if the first `n` bytes of the
 * first string is less than, equal to, or greater than the first `n` bytes of the second
 * string (ignoring case).
 */
int strncasecmp(const char* __s1, const char* __s2, size_t __n) __attribute_pure__;

/**
 * Like strncasecmp() but taking a `locale_t`.
 */

#if __ANDROID_API__ >= 23
int strncasecmp_l(const char* __s1, const char* __s2, size_t __n, locale_t __l) __attribute_pure__ __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


__END_DECLS
