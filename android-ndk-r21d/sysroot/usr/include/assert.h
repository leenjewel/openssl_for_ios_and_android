/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 */

/**
 * @file assert.h
 * @brief Assertions.
 *
 * There's no include guard in this file because <assert.h> may usefully be
 * included multiple times, with and without NDEBUG defined.
 */

#include <sys/cdefs.h>

#undef assert
#undef __assert_no_op

/** Internal implementation detail. Do not use. */
#define __assert_no_op __BIONIC_CAST(static_cast, void, 0)

#ifdef NDEBUG
# define assert(e) __assert_no_op
#else
# if defined(__cplusplus) || __STDC_VERSION__ >= 199901L
#  define assert(e) ((e) ? __assert_no_op : __assert2(__FILE__, __LINE__, __PRETTY_FUNCTION__, #e))
# else
/**
 * assert() aborts the program after logging an error message, if the
 * expression evaluates to false.
 *
 * On Android, the error goes to both stderr and logcat.
 */
#  define assert(e) ((e) ? __assert_no_op : __assert(__FILE__, __LINE__, #e))
# endif
#endif

#if !defined(__cplusplus) && __STDC_VERSION__ >= 201112L
# undef static_assert
# define static_assert _Static_assert
#endif

__BEGIN_DECLS

/**
 * __assert() is called by assert() on failure. Most users want assert()
 * instead, but this can be useful for reporting other failures.
 */
void __assert(const char* __file, int __line, const char* __msg) __noreturn;

/**
 * __assert2() is called by assert() on failure. Most users want assert()
 * instead, but this can be useful for reporting other failures.
 */
void __assert2(const char* __file, int __line, const char* __function, const char* __msg) __noreturn;

__END_DECLS
