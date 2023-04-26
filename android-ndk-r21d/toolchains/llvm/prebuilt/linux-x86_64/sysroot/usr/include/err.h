/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)err.h	8.1 (Berkeley) 6/2/93
 */

#pragma once

/**
 * @file err.h
 * @brief BSD error reporting functions. See `<error.h>` for the GNU equivalent.
 */

#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * [err(3)](http://man7.org/linux/man-pages/man3/err.3.html) outputs the program name,
 * the printf()-like formatted message, and the result of strerror() if `errno` is non-zero.
 *
 * Calls exit() with `__status`.
 *
 * New code should consider error() in `<error.h>`.
 */
__noreturn void err(int __status, const char* __fmt, ...) __printflike(2, 3);

/**
 * [verr(3)](http://man7.org/linux/man-pages/man3/verr.3.html) outputs the program name,
 * the vprintf()-like formatted message, and the result of strerror() if `errno` is non-zero.
 *
 * Calls exit() with `__status`.
 *
 * New code should consider error() in `<error.h>`.
 */
__noreturn void verr(int __status, const char* __fmt, va_list __args) __printflike(2, 0);

/**
 * [errx(3)](http://man7.org/linux/man-pages/man3/errx.3.html) outputs the program name, and
 * the printf()-like formatted message.
 *
 * Calls exit() with `__status`.
 *
 * New code should consider error() in `<error.h>`.
 */
__noreturn void errx(int __status, const char* __fmt, ...) __printflike(2, 3);

/**
 * [verrx(3)](http://man7.org/linux/man-pages/man3/err.3.html) outputs the program name, and
 * the vprintf()-like formatted message.
 *
 * Calls exit() with `__status`.
 *
 * New code should consider error() in `<error.h>`.
 */
__noreturn void verrx(int __status, const char* __fmt, va_list __args) __printflike(2, 0);

/**
 * [warn(3)](http://man7.org/linux/man-pages/man3/warn.3.html) outputs the program name,
 * the printf()-like formatted message, and the result of strerror() if `errno` is non-zero.
 *
 * New code should consider error() in `<error.h>`.
 */
void warn(const char* __fmt, ...) __printflike(1, 2);

/**
 * [vwarn(3)](http://man7.org/linux/man-pages/man3/vwarn.3.html) outputs the program name,
 * the vprintf()-like formatted message, and the result of strerror() if `errno` is non-zero.
 *
 * New code should consider error() in `<error.h>`.
 */
void vwarn(const char* __fmt, va_list __args) __printflike(1, 0);

/**
 * [warnx(3)](http://man7.org/linux/man-pages/man3/warnx.3.html) outputs the program name, and
 * the printf()-like formatted message.
 *
 * New code should consider error() in `<error.h>`.
 */
void warnx(const char* __fmt, ...) __printflike(1, 2);

/**
 * [vwarnx(3)](http://man7.org/linux/man-pages/man3/warn.3.html) outputs the program name, and
 * the vprintf()-like formatted message.
 *
 * New code should consider error() in `<error.h>`.
 */
void vwarnx(const char* __fmt, va_list __args) __printflike(1, 0);

__END_DECLS
