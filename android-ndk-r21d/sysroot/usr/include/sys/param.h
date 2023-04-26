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
 * @file sys/param.h
 * @brief Various macros.
 */

#include <endian.h>
#include <limits.h>
#include <linux/param.h>
#include <sys/cdefs.h>

/** The unit of `st_blocks` in `struct stat`. */
#define DEV_BSIZE 512

/** A historical name for PATH_MAX. */
#define MAXPATHLEN  PATH_MAX

#define MAXSYMLINKS 8

#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif
#define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))

/**
 * Returns true if the binary representation of the argument is all zeros
 * or has exactly one bit set. Contrary to the macro name, this macro
 * DOES NOT determine if the provided value is a power of 2. In particular,
 * this function falsely returns true for powerof2(0) and some negative
 * numbers.
 */
#define powerof2(x)                                               \
  ({                                                              \
    __typeof__(x) _x = (x);                                       \
    __typeof__(x) _x2;                                            \
    __builtin_add_overflow(_x, -1, &_x2) ? 1 : ((_x2 & _x) == 0); \
  })

/** Returns the lesser of its two arguments. */
#define MIN(a,b) (((a)<(b))?(a):(b))
/** Returns the greater of its two arguments. */
#define MAX(a,b) (((a)>(b))?(a):(b))
