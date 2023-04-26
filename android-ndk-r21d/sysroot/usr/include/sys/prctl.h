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
 * @file sys/prctl.h
 * @brief Process-specific operations.
 */

#include <sys/cdefs.h>

#include <linux/prctl.h>

/**
 * Names a VMA (mmap'ed region). The second argument must be `PR_SET_VMA_ANON_NAME`,
 * the third and fourth are a `void*` pointer to the VMA and its `size_t` length in
 * bytes, and the final argument is a `const char*` pointer to the name.
 *
 * Note that the kernel keeps the pointer to the name rather than copying the name,
 * so the lifetime of the string should be at least as long as that of the VMA.
 */
#define PR_SET_VMA 0x53564d41

/**
 * For use with `PR_SET_VMA`.
 */
#define PR_SET_VMA_ANON_NAME 0

__BEGIN_DECLS

/**
 * [prctl(2)](http://man7.org/linux/man-pages/man2/prctl.2.html) performs a variety of
 * operations based on the `PR_` constant passed as the first argument.
 *
 * Returns -1 and sets `errno` on failure; success values vary by option.
 */
int prctl(int __option, ...);

__END_DECLS
