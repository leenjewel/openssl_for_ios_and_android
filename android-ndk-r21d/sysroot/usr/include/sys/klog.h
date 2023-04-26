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
 * @file sys/klog.h
 * @brief
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/** Used with klogctl(). */
#define KLOG_CLOSE 0
/** Used with klogctl(). */
#define KLOG_OPEN 1
/** Used with klogctl(). */
#define KLOG_READ 2
/** Used with klogctl(). */
#define KLOG_READ_ALL 3
/** Used with klogctl(). */
#define KLOG_READ_CLEAR 4
/** Used with klogctl(). */
#define KLOG_CLEAR 5
/** Used with klogctl(). */
#define KLOG_CONSOLE_OFF 6
/** Used with klogctl(). */
#define KLOG_CONSOLE_ON 7
/** Used with klogctl(). */
#define KLOG_CONSOLE_LEVEL 8
/** Used with klogctl(). */
#define KLOG_SIZE_UNREAD 9
/** Used with klogctl(). */
#define KLOG_SIZE_BUFFER 10

/**
 * [klogctl(2)](http://man7.org/linux/man-pages/man2/syslog.2.html) operates on the kernel log.
 *
 * This system call is not available to applications.
 * Use syslog() or `<android/log.h>` instead.
 */
int klogctl(int __type, char* __buf, int __buf_size);

__END_DECLS
