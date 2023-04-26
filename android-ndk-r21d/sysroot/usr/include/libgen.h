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
 * @file libgen.h
 * @brief POSIX basename() and dirname().
 *
 * This file contains the POSIX basename() and dirname(). See `<string.h>` for the GNU basename().
 */

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * [basename(3)](http://man7.org/linux/man-pages/man3/basename.3.html)
 * returns the final component of the given path.
 *
 * See `<string.h>` for the GNU basename(). Including `<libgen.h>`,
 * either before or after including <string.h>, will override the GNU variant.
 *
 * Note that Android's cv-qualifiers differ from POSIX; Android's implementation doesn't
 * modify its input and uses thread-local storage for the result if necessary.
 */
char* __posix_basename(const char* __path) __RENAME(basename);

/**
 * This macro ensures that callers get the POSIX basename() if they include this header,
 * no matter what order `<libgen.h>` and `<string.h>` are included in.
 */
#define basename __posix_basename

/**
 * [dirname(3)](http://man7.org/linux/man-pages/man3/dirname.3.html)
 * returns all but the final component of the given path.
 *
 * Note that Android's cv-qualifiers differ from POSIX; Android's implementation doesn't
 * modify its input and uses thread-local storage for the result if necessary.
 */
char* dirname(const char* __path);

#if !defined(__LP64__)
/** Deprecated. Use dirname() instead. */
int dirname_r(const char* __path, char* __buf, size_t __n);
/** Deprecated. Use basename() instead. */
int basename_r(const char* __path, char* __buf, size_t __n);
#endif

__END_DECLS
