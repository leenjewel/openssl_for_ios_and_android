/*
 * Copyright (C) 2015 The Android Open Source Project
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
 * @file error.h
 * @brief GNU error reporting functions.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * [error_print_progname(3)](http://man7.org/linux/man-pages/man3/error_print_progname.3.html) is
 * a function pointer that, if non-null, is called by error() instead of prefixing errors with the
 * program name.
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
extern void (*error_print_progname)(void) __INTRODUCED_IN(23);

/**
 * [error_message_count(3)](http://man7.org/linux/man-pages/man3/error_message_count.3.html) is
 * a global count of the number of calls to error() and error_at_line().
 *
 * Available since API level 23.
 */
extern unsigned int error_message_count __INTRODUCED_IN(23);

/**
 * [error_one_per_line(3)](http://man7.org/linux/man-pages/man3/error_one_per_line.3.html) is
 * a global flag that if non-zero disables printing multiple errors with the same filename and
 * line number.
 *
 * Available since API level 23.
 */
extern int error_one_per_line __INTRODUCED_IN(23);

/**
 * [error(3)](http://man7.org/linux/man-pages/man3/error.3.html) formats the given printf()-like
 * error message, preceded by the program name. Calls exit if `__status` is non-zero, and appends
 * the result of strerror() if `__errno` is non-zero.
 *
 * Available since API level 23.
 */
void error(int __status, int __errno, const char* __fmt, ...) __printflike(3, 4) __INTRODUCED_IN(23);

/**
 * [error_at_line(3)](http://man7.org/linux/man-pages/man3/error_at_line.3.html) formats the given
 * printf()-like error message, preceded by the program name and the given filename and line number.
 * Calls exit if `__status` is non-zero, and appends the result of strerror() if `__errno` is
 * non-zero.
 *
 * Available since API level 23.
 */
void error_at_line(int __status, int __errno, const char* __filename, unsigned int __line_number, const char* __fmt, ...) __printflike(5, 6) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


__END_DECLS
