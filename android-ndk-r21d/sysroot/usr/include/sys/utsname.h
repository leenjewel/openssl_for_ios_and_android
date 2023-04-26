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
 * @file sys/utsname.h
 * @brief The uname() function.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/** The maximum length of any field in `struct utsname`. */
#define SYS_NMLN 65

/** The information returned by uname(). */
struct utsname {
  /** The OS name. "Linux" on Android. */
  char sysname[SYS_NMLN];
  /** The name on the network. Typically "localhost" on Android. */
  char nodename[SYS_NMLN];
  /** The OS release. Typically something like "4.4.115-g442ad7fba0d" on Android. */
  char release[SYS_NMLN];
  /** The OS version. Typically something like "#1 SMP PREEMPT" on Android. */
  char version[SYS_NMLN];
  /** The hardware architecture. Typically "aarch64" on Android. */
  char machine[SYS_NMLN];
  /** The domain name set by setdomainname(). Typically "localdomain" on Android. */
  char domainname[SYS_NMLN];
};

/**
 * [uname(2)](http://man7.org/linux/man-pages/man2/uname.2.html) returns information
 * about the kernel.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int uname(struct utsname* __buf);

__END_DECLS
