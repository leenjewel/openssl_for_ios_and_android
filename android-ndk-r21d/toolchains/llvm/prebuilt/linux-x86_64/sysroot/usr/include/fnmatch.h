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
 * @file fnmatch.h
 * @brief Filename matching.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/** Returned by fnmatch() if matching failed. */
#define FNM_NOMATCH 1

/** Returned by fnmatch() if the function is not supported. This is never returned on Android. */
#define FNM_NOSYS 2

/** fnmatch() flag to disable backslash escaping. */
#define FNM_NOESCAPE     0x01
/** fnmatch() flag to ensure that slashes must be matched by slashes. */
#define FNM_PATHNAME     0x02
/** fnmatch() flag to ensure that periods must be matched by periods. */
#define FNM_PERIOD       0x04
/** fnmatch() flag to ignore /... after a match. */
#define FNM_LEADING_DIR  0x08
/** fnmatch() flag for a case-insensitive search. */
#define FNM_CASEFOLD     0x10

/** Synonym for `FNM_CASEFOLD`: case-insensitive search. */
#define FNM_IGNORECASE   FNM_CASEFOLD
/** Synonym for `FNM_PATHNAME`: slashes must be matched by slashes. */
#define FNM_FILE_NAME    FNM_PATHNAME

/**
 * [fnmatch(3)](http://man7.org/linux/man-pages/man3/fnmatch.3.html) matches `__string` against
 * the shell wildcard `__pattern`.
 *
 * Returns 0 on success, and returns `FNM_NOMATCH` on failure.
 */
int fnmatch(const char* _Nonnull __pattern, const char* _Nonnull __string, int __flags);

__END_DECLS
