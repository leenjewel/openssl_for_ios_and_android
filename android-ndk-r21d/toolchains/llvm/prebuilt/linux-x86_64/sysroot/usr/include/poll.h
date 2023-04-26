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
 * @file poll.h
 * @brief Wait for events on a set of file descriptors.
 */

#include <sys/cdefs.h>
#include <linux/poll.h>
#include <signal.h> /* For sigset_t. */
#include <time.h> /* For timespec. */

__BEGIN_DECLS

/** The type of a file descriptor count, used by poll() and ppoll(). */
typedef unsigned int nfds_t;

/**
 * [poll(3)](http://man7.org/linux/man-pages/man3/poll.3.html) waits on a set of file descriptors.
 *
 * Returns the number of ready file descriptors on success, 0 for timeout,
 * and returns -1 and sets `errno` on failure.
 */
int poll(struct pollfd* _Nullable __fds, nfds_t __count, int __timeout_ms);

/**
 * [ppoll(3)](http://man7.org/linux/man-pages/man3/ppoll.3.html) waits on a set of file descriptors
 * or a signal. Set `__timeout` to null for no timeout. Set `__mask` to null to not set the signal
 * mask.
 *
 * Returns the number of ready file descriptors on success, 0 for timeout,
 * and returns -1 and sets `errno` on failure.
 *
 * Available since API level 28.
 */

#if __ANDROID_API__ >= 21
int ppoll(struct pollfd* _Nullable __fds, nfds_t __count, const struct timespec* _Nullable __timeout, const sigset_t* _Nullable __mask) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


/**
 * Like ppoll() but allows setting a signal mask with RT signals even from a 32-bit process.
 */

#if __ANDROID_API__ >= 28
int ppoll64(struct pollfd* _Nullable  __fds, nfds_t __count, const struct timespec* _Nullable __timeout, const sigset64_t* _Nullable __mask) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


#if defined(__BIONIC_INCLUDE_FORTIFY_HEADERS)
#define _POLL_H_
#include <bits/fortify/poll.h>
#undef _POLL_H_
#endif

__END_DECLS
