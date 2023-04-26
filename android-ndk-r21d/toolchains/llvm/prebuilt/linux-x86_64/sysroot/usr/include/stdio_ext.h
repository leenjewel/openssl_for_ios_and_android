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
 * @file stdio_ext.h
 * @brief Extra standard I/O functionality. See also `<stdio.h>`.
 */

#include <sys/cdefs.h>
#include <stdio.h>

__BEGIN_DECLS

/**
 * [__fbufsize(3)](http://man7.org/linux/man-pages/man3/__fbufsize.3.html) returns the size of
 * the stream's buffer.
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
size_t __fbufsize(FILE* __fp) __INTRODUCED_IN(23);

/**
 * [__freadable(3)](http://man7.org/linux/man-pages/man3/__freadable.3.html) returns non-zero if
 * the stream allows reading, 0 otherwise.
 *
 * Available since API level 23.
 */
int __freadable(FILE* __fp) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


/**
 * [__freading(3)](http://man7.org/linux/man-pages/man3/__freading.3.html) returns non-zero if
 * the stream's last operation was a read, 0 otherwise.
 *
 * Available since API level 28.
 */

#if __ANDROID_API__ >= 28
int __freading(FILE* __fp) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


/**
 * [__fwritable(3)](http://man7.org/linux/man-pages/man3/__fwritable.3.html) returns non-zero if
 * the stream allows writing, 0 otherwise.
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
int __fwritable(FILE* __fp) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


/**
 * [__fwriting(3)](http://man7.org/linux/man-pages/man3/__fwriting.3.html) returns non-zero if
 * the stream's last operation was a write, 0 otherwise.
 *
 * Available since API level 28.
 */

#if __ANDROID_API__ >= 28
int __fwriting(FILE* __fp) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


/**
 * [__flbf(3)](http://man7.org/linux/man-pages/man3/__flbf.3.html) returns non-zero if
 * the stream is line-buffered, 0 otherwise.
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
int __flbf(FILE* __fp) __INTRODUCED_IN(23);

/**
 * [__fpurge(3)](http://man7.org/linux/man-pages/man3/__fpurge.3.html) discards the contents of
 * the stream's buffer.
 *
 * Available since API level 23.
 */
void __fpurge(FILE* __fp) __INTRODUCED_IN(23);

/**
 * [__fpending(3)](http://man7.org/linux/man-pages/man3/__fpending.3.html) returns the number of
 * bytes in the output buffer.
 *
 * Available since API level 23.
 */
size_t __fpending(FILE* __fp) __INTRODUCED_IN(23);

/**
 * [_flushlbf(3)](http://man7.org/linux/man-pages/man3/_flushlbf.3.html) flushes all
 * line-buffered streams.
 *
 * Available since API level 23.
 */
void _flushlbf(void) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


/**
 * `__fseterr` sets the
 * stream's error flag (as tested by ferror() and cleared by fclearerr()).
 *
 * Available since API level 28.
 */

#if __ANDROID_API__ >= 28
void __fseterr(FILE* __fp) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


/** __fsetlocking() constant to query locking type. */
#define FSETLOCKING_QUERY 0
/** __fsetlocking() constant to set locking to be maintained by stdio. */
#define FSETLOCKING_INTERNAL 1
/** __fsetlocking() constant to set locking to be maintained by the caller. */
#define FSETLOCKING_BYCALLER 2

/**
 * [__fsetlocking(3)](http://man7.org/linux/man-pages/man3/__fsetlocking.3.html) sets the
 * stream's locking mode to one of the `FSETLOCKING_` types.
 *
 * Returns the current locking style, `FSETLOCKING_INTERNAL` or `FSETLOCKING_BYCALLER`.
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
int __fsetlocking(FILE* __fp, int __type) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


__END_DECLS
