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

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * [getopt(3)](http://man7.org/linux/man-pages/man3/getopt.3.html) parses command-line options.
 *
 * Returns the next option character on success, returns -1 if all options have been parsed, and
 * returns `'?'` on error.
 */
int getopt(int __argc, char* const __argv[], const char* __options);

/**
 * Points to the text of the corresponding value for options that take an argument.
 */
extern char* optarg;

/**
 * The index of the next element to be processed.
 * On Android, callers should set `optreset = 1` rather than trying to reset `optind` to
 * scan a new argument vector.
 */
extern int optind;

/**
 * Determines whether getopt() outputs error messages.
 * Callers should set this to `0` to disable error messages.
 * Defaults to non-zero.
 */
extern int opterr;

/**
 * The last unrecognized option character, valid when getopt() returns `'?'`.
 */
extern int optopt;

__END_DECLS
