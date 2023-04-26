/*	$OpenBSD: ctype.h,v 1.19 2005/12/13 00:35:22 millert Exp $	*/
/*	$NetBSD: ctype.h,v 1.14 1994/10/26 00:55:47 cgd Exp $	*/

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
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
 *
 *	@(#)ctype.h	5.3 (Berkeley) 4/3/91
 */

#pragma once

/**
 * @file ctype.h
 * @brief ASCII character classification.
 */

#include <sys/cdefs.h>
#include <xlocale.h>

/** Internal implementation detail. Do not use. */
#define _CTYPE_U 0x01
/** Internal implementation detail. Do not use. */
#define _CTYPE_L 0x02
/** Internal implementation detail. Do not use. */
#define _CTYPE_D 0x04
/** Internal implementation detail. Do not use. */
#define _CTYPE_S 0x08
/** Internal implementation detail. Do not use. */
#define _CTYPE_P 0x10
/** Internal implementation detail. Do not use. */
#define _CTYPE_C 0x20
/** Internal implementation detail. Do not use. */
#define _CTYPE_X 0x40
/** Internal implementation detail. Do not use. */
#define _CTYPE_B 0x80
/** Internal implementation detail. Do not use. */
#define _CTYPE_R (_CTYPE_P|_CTYPE_U|_CTYPE_L|_CTYPE_D|_CTYPE_B)
/** Internal implementation detail. Do not use. */
#define _CTYPE_A (_CTYPE_L|_CTYPE_U)
/** Internal implementation detail. Do not use. */
#define _CTYPE_N _CTYPE_D

__BEGIN_DECLS

/** Internal implementation detail. Do not use. */
extern const char* _ctype_;

/** Returns true if `ch` is in `[A-Za-z0-9]`. */
int isalnum(int __ch);
/** Returns true if `ch` is in `[A-Za-z]`. */
int isalpha(int __ch);
/** Returns true if `ch` is a space or tab. */
int isblank(int __ch);
/** Returns true if `ch` is a control character (any character before space, plus DEL). */
int iscntrl(int __ch);
/** Returns true if `ch` is in `[0-9]`. */
int isdigit(int __ch);
/** Returns true if `ch` is `[A-Za-z0-9]` or punctuation. */
int isgraph(int __ch);
/** Returns true if `ch` is in `[a-z]`. */
int islower(int __ch);
/** Returns true if `ch` is `[A-Za-z0-9]` or punctuation or space. */
int isprint(int __ch);
/** Returns true if `ch` is punctuation. */
int ispunct(int __ch);
/** Returns true if `ch` is in `[ \f\n\r\t\v]`. */
int isspace(int __ch);
/** Returns true if `ch` is in `[A-Z]`. */
int isupper(int __ch);
/** Returns true if `ch` is in `[0-9A-Fa-f]`. */
int isxdigit(int __ch);

/** Returns the corresponding lower-case character if `ch` is upper-case, or `ch` otherwise. */
int tolower(int __ch);

/**
 * Returns the corresponding lower-case character if `ch` is upper-case, or undefined otherwise.
 *
 * Available since API level 21.
 *
 * Prefer tolower() instead.
 */

#if __ANDROID_API__ >= 21
int _tolower(int __ch) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


/** Returns the corresponding upper-case character if `ch` is lower-case, or `ch` otherwise. */
int toupper(int __ch);

/**
 * Returns the corresponding upper-case character if `ch` is lower-case, or undefined otherwise.
 *
 * Available since API level 21.
 *
 * Prefer toupper() instead.
 */

#if __ANDROID_API__ >= 21
int _toupper(int __ch) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


#if __ANDROID_API__ >= __ANDROID_API_L__
/** Like isalnum but with an ignored `locale_t`. */
int isalnum_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isalpha but with an ignored `locale_t`. */
int isalpha_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isblank but with an ignored `locale_t`. */
int isblank_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like iscntrl but with an ignored `locale_t`. */
int iscntrl_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isdigit but with an ignored `locale_t`. */
int isdigit_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isgraph but with an ignored `locale_t`. */
int isgraph_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like islower but with an ignored `locale_t`. */
int islower_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isprint but with an ignored `locale_t`. */
int isprint_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like ispunct but with an ignored `locale_t`. */
int ispunct_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isspace but with an ignored `locale_t`. */
int isspace_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isupper but with an ignored `locale_t`. */
int isupper_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isxdigit but with an ignored `locale_t`. */
int isxdigit_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like tolower but with an ignored `locale_t`. */
int tolower_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like toupper but with an ignored `locale_t`. */
int toupper_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
#else
// Implemented as static inlines before 21.
#endif

/** Returns true if `ch` is less than 0x80. */
int isascii(int __ch);
/** Returns `ch & 0x7f`. */
int toascii(int __ch);

__END_DECLS
