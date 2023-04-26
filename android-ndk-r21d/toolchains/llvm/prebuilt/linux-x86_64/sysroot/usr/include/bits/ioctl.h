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
 * @file bits/ioctl.h
 * @brief The ioctl() function.
 */

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * [ioctl(2)](http://man7.org/linux/man-pages/man2/ioctl.2.html) operates on device files.
 */
int ioctl(int __fd, int __request, ...);

/*
 * Work around unsigned -> signed conversion warnings: many common ioctl
 * constants are unsigned.
 *
 * Since this workaround introduces an overload to ioctl, it's possible that it
 * will break existing code that takes the address of ioctl. If such a breakage
 * occurs, you can work around it by either:
 * - specifying a concrete, correct type for ioctl (whether it be through a cast
 *   in `(int (*)(int, int, ...))ioctl`, creating a temporary variable with the
 *   type of the ioctl you prefer, ...), or
 * - defining BIONIC_IOCTL_NO_SIGNEDNESS_OVERLOAD, which will make the
 *   overloading go away.
 */
#if !defined(BIONIC_IOCTL_NO_SIGNEDNESS_OVERLOAD)
/* enable_if(1) just exists to break overloading ties. */
int ioctl(int __fd, unsigned __request, ...) __overloadable __enable_if(1, "") __RENAME(ioctl);
#endif

__END_DECLS
