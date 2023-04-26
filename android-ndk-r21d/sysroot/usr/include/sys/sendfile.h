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
 * @file sys/sendfile.h
 * @brief The sendfile() function.
 */

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/* See https://android.googlesource.com/platform/bionic/+/master/docs/32-bit-abi.md */
#if defined(__USE_FILE_OFFSET64)

#if __ANDROID_API__ >= 21
ssize_t sendfile(int __out_fd, int __in_fd, off_t* __offset, size_t __count) __RENAME(sendfile64) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */

#else
/**
 * [sendfile(2)](http://man7.org/linux/man-pages/man2/sendfile.2.html) copies data directly
 * between two file descriptors.
 *
 * Returns the number of bytes copied on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 21.
 */
ssize_t sendfile(int __out_fd, int __in_fd, off_t* __offset, size_t __count);
#endif

/**
 * Like sendfile() but allows using a 64-bit offset
 * even from a 32-bit process without `__FILE_OFFSET_BITS=64`.
 */

#if __ANDROID_API__ >= 21
ssize_t sendfile64(int __out_fd, int __in_fd, off64_t* __offset, size_t __count) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


__END_DECLS
