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
 * @file bits/lockf.h
 * @brief The lockf() function.
 */

#include <sys/cdefs.h>
#include <sys/types.h>

/** lockf() command to unlock a section of a file. */
#define F_ULOCK 0
/** lockf() command to block until it locks a section of a file. */
#define F_LOCK 1
/** lockf() command to try to lock a section of a file. */
#define F_TLOCK 2
/** lockf() command to test whether a section of a file is unlocked (or locked by the caller). */
#define F_TEST 3

__BEGIN_DECLS

/**
 * [lockf(3)](http://man7.org/linux/man-pages/man3/lockf.3.html) manipulates POSIX file locks.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 24.
 *
 * See also flock().
 */

#if __ANDROID_API__ >= 24
int lockf(int __fd, int __cmd, off_t __length) __RENAME_IF_FILE_OFFSET64(lockf64) __INTRODUCED_IN(24);

/**
 * Like lockf() but allows using a 64-bit length
 * even from a 32-bit process without `__FILE_OFFSET_BITS=64`.
 */
int lockf64(int __fd, int __cmd, off64_t __length) __INTRODUCED_IN(24);
#endif /* __ANDROID_API__ >= 24 */


__END_DECLS
