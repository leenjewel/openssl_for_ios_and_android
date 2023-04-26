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
 * @file sys/timerfd.h
 * @brief Timer file descriptors.
 */

#include <fcntl.h> /* For O_CLOEXEC and O_NONBLOCK. */
#include <time.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/** The timerfd_create() flag for a close-on-exec file descriptor. */
#define TFD_CLOEXEC O_CLOEXEC
/** The timerfd_create() flag for a non-blocking file descriptor. */
#define TFD_NONBLOCK O_NONBLOCK

/**
 * [timerfd_create(2)](http://man7.org/linux/man-pages/man2/timerfd_create.2.html) creates a
 * timer file descriptor.
 *
 * Returns the new file descriptor on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 19.
 */

#if __ANDROID_API__ >= 19
int timerfd_create(clockid_t __clock, int __flags) __INTRODUCED_IN(19);
#endif /* __ANDROID_API__ >= 19 */


/** The timerfd_settime() flag to use absolute rather than relative times. */
#define TFD_TIMER_ABSTIME (1 << 0)
/** The timerfd_settime() flag to cancel an absolute timer if the realtime clock changes. */
#define TFD_TIMER_CANCEL_ON_SET (1 << 1)

/**
 * [timerfd_settime(2)](http://man7.org/linux/man-pages/man2/timerfd_settime.2.html) starts or
 * stops a timer.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 19.
 */

#if __ANDROID_API__ >= 19
int timerfd_settime(int __fd, int __flags, const struct itimerspec* __new_value, struct itimerspec* __old_value) __INTRODUCED_IN(19);

/**
 * [timerfd_gettime(2)](http://man7.org/linux/man-pages/man2/timerfd_gettime.2.html) queries the
 * current timer settings.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 *
 * Available since API level 19.
 */
int timerfd_gettime(int __fd, struct itimerspec* __current_value) __INTRODUCED_IN(19);
#endif /* __ANDROID_API__ >= 19 */


__END_DECLS
