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
 * @file sys/signalfd.h
 * @brief File-descriptor based signal interface.
 */

#include <sys/cdefs.h>

#include <linux/signalfd.h>
#include <signal.h>

__BEGIN_DECLS

/**
 * [signalfd(2)](http://man7.org/linux/man-pages/man2/signalfd.2.html) creates/manipulates a
 * file descriptor for reading signal events.
 *
 * Returns the file descriptor on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 18.
 */

#if __ANDROID_API__ >= 18
int signalfd(int __fd, const sigset_t* __mask, int __flags) __INTRODUCED_IN(18);
#endif /* __ANDROID_API__ >= 18 */


/**
 * Like signalfd() but allows setting a signal mask with RT signals even from a 32-bit process.
 */

#if __ANDROID_API__ >= 28
int signalfd64(int __fd, const sigset64_t* __mask, int __flags) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


__END_DECLS
