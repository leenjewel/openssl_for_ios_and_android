/*
 * Copyright (C) 2016 The Android Open Source Project
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
 * @file nl_types.h
 * @brief Message catalogs.
 *
 * Android offers a dummy implementation of these functions to ease porting of historical software.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * catopen() flag to use the current locale.
 */
#define NL_CAT_LOCALE 1

/**
 * catgets() default set number.
 */
#define NL_SETD 1

/** Message catalog type. */
typedef void* nl_catd;

/** The type of the constants in `<langinfo.h>`, used by nl_langinfo(). */
typedef int nl_item;

/**
 * [catopen(3)](http://man7.org/linux/man-pages/man3/catopen.3.html) opens a message catalog.
 *
 * On Android, this always returns failure: `((nl_catd) -1)`.
 *
 * Available since API level 28.
 */

#if __ANDROID_API__ >= 26
nl_catd catopen(const char* __name, int __flag) __INTRODUCED_IN(26);

/**
 * [catgets(3)](http://man7.org/linux/man-pages/man3/catgets.3.html) translates the given message
 * using the given message catalog.
 *
 * On Android, this always returns `__msg`.
 *
 * Available since API level 28.
 */
char* catgets(nl_catd __catalog, int __set_number, int __msg_number, const char* __msg) __INTRODUCED_IN(26);

/**
 * [catclose(3)](http://man7.org/linux/man-pages/man3/catclose.3.html) closes a message catalog.
 *
 * On Android, this always returns -1 with `errno` set to `EBADF`.
 */
int catclose(nl_catd __catalog) __INTRODUCED_IN(26);
#endif /* __ANDROID_API__ >= 26 */


__END_DECLS
