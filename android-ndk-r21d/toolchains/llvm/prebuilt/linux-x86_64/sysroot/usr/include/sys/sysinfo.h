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
 * @file sys/sysinfo.h
 * @brief System information.
 */

#include <sys/cdefs.h>
#include <linux/kernel.h>

__BEGIN_DECLS

/**
 * [sysinfo(2)](http://man7.org/linux/man-pages/man2/sysinfo.2.html) queries system information.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int sysinfo(struct sysinfo* __info);

/**
 * [get_nprocs_conf(3)](http://man7.org/linux/man-pages/man3/get_nprocs_conf.3.html) returns
 * the total number of processors in the system.
 *
 * Available since API level 23.
 *
 * See also sysconf().
 */

#if __ANDROID_API__ >= 23
int get_nprocs_conf(void) __INTRODUCED_IN(23);

/**
 * [get_nprocs(3)](http://man7.org/linux/man-pages/man3/get_nprocs.3.html) returns
 * the number of processors in the system that are currently on-line.
 *
 * Available since API level 23.
 *
 * See also sysconf().
 */
int get_nprocs(void) __INTRODUCED_IN(23);

/**
 * [get_phys_pages(3)](http://man7.org/linux/man-pages/man3/get_phys_pages.3.html) returns
 * the total number of physical pages in the system.
 *
 * Available since API level 23.
 *
 * See also sysconf().
 */
long get_phys_pages(void) __INTRODUCED_IN(23);

/**
 * [get_avphys_pages(3)](http://man7.org/linux/man-pages/man3/get_avphys_pages.3.html) returns
 * the number of physical pages in the system that are currently available.
 *
 * Available since API level 23.
 *
 * See also sysconf().
 */
long get_avphys_pages(void) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


__END_DECLS
