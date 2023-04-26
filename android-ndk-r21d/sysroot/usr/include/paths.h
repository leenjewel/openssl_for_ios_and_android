/*
 * Copyright (c) 1989, 1993
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
 * 4. Neither the name of the University nor the names of its contributors
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
 *	@(#)paths.h	8.1 (Berkeley) 6/2/93
 */

#pragma once

/**
 * @file paths.h
 * @brief Default paths.
 */

#include <sys/cdefs.h>

#ifndef _PATH_BSHELL
/** Path to the default system shell. Historically the 'B' was to specify the Bourne shell. */
#define _PATH_BSHELL "/system/bin/sh"
#endif

/** Path to the system console. */
#define _PATH_CONSOLE "/dev/console"

/** Default shell search path. */
#define _PATH_DEFPATH "/product/bin:/apex/com.android.runtime/bin:/apex/com.android.art/bin:/system/bin:/system/xbin:/odm/bin:/vendor/bin:/vendor/xbin"

/** Path to the directory containing device files. */
#define _PATH_DEV "/dev/"

/** Path to `/dev/null`. */
#define _PATH_DEVNULL "/dev/null"

/** Path to the kernel log. */
#define _PATH_KLOG "/proc/kmsg"

/** Path to `/proc/mounts` for setmntent(). */
#define _PATH_MOUNTED "/proc/mounts"

/** Path to the calling process' tty. */
#define _PATH_TTY "/dev/tty"
