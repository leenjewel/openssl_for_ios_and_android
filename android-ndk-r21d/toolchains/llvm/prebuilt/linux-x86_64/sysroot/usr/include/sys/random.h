/*
 * Copyright (C) 2017 The Android Open Source Project
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
 * @file sys/random.h
 * @brief The getentropy() and getrandom() functions.
 */

#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/random.h>

__BEGIN_DECLS

/**
 * [getentropy(3)](http://man7.org/linux/man-pages/man3/getentropy.3.html) fills the given buffer
 * with random bytes.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 28.
 *
 * See also arc4random_buf() which is available in all API levels.
 */

#if __ANDROID_API__ >= 28
int getentropy(void* __buffer, size_t __buffer_size) __wur __INTRODUCED_IN(28);

/**
 * [getrandom(2)](http://man7.org/linux/man-pages/man2/getrandom.2.html) fills the given buffer
 * with random bytes.
 *
 * Returns the number of bytes copied on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 28.
 *
 * See also arc4random_buf() which is available in all API levels.
 */
ssize_t getrandom(void* __buffer, size_t __buffer_size, unsigned int __flags) __wur __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


__END_DECLS
