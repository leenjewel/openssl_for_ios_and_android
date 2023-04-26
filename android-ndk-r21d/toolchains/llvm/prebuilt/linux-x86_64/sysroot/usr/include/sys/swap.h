/*
 * Copyright (C) 2013 The Android Open Source Project
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
 * @file sys/swap.h
 * @brief Swap control.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/** swapon() flag to discard pages. */
#define SWAP_FLAG_DISCARD 0x10000

/**
 * swapon() flag to give this swap area a non-default priority.
 * The priority is also encoded in the flags:
 * `(priority << SWAP_FLAG_PRIO_SHIFT) & SWAP_FLAG_PRIO_MASK`.
 */
#define SWAP_FLAG_PREFER 0x8000
/** See SWAP_FLAG_PREFER. */
#define SWAP_FLAG_PRIO_MASK 0x7fff
/** See SWAP_FLAG_PREFER. */
#define SWAP_FLAG_PRIO_SHIFT 0

/**
 * [swapon(2)](http://man7.org/linux/man-pages/man2/swapon.2.html) enables swapping.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 19.
 */

#if __ANDROID_API__ >= 19
int swapon(const char* __path,  int __flags) __INTRODUCED_IN(19);

/**
 * [swapoff(2)](http://man7.org/linux/man-pages/man2/swapoff.2.html) disables swapping.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 19.
 */
int swapoff(const char* __path) __INTRODUCED_IN(19);
#endif /* __ANDROID_API__ >= 19 */


__END_DECLS
