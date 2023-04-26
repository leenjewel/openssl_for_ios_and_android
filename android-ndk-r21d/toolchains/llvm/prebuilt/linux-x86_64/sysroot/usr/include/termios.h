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
 * @file termios.h
 * @brief General terminal interfaces.
 */

#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/termios.h>

__BEGIN_DECLS

#if __ANDROID_API__ >= __ANDROID_API_L__
// This file is implemented as static inlines before API level 21.

/**
 * [cfgetispeed(3)](http://man7.org/linux/man-pages/man3/cfgetispeed.3.html)
 * returns the terminal input baud rate.
 */
speed_t cfgetispeed(const struct termios* __t) __INTRODUCED_IN(21);

/**
 * [cfgetospeed(3)](http://man7.org/linux/man-pages/man3/cfgetospeed.3.html)
 * returns the terminal output baud rate.
 */
speed_t cfgetospeed(const struct termios* __t) __INTRODUCED_IN(21);

/**
 * [cfmakeraw(3)](http://man7.org/linux/man-pages/man3/cfmakeraw.3.html)
 * configures the terminal for "raw" mode.
 */
void cfmakeraw(struct termios* __t) __INTRODUCED_IN(21);

/**
 * [cfsetspeed(3)](http://man7.org/linux/man-pages/man3/cfsetspeed.3.html)
 * sets the terminal input and output baud rate.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int cfsetspeed(struct termios* __t, speed_t __speed) __INTRODUCED_IN(21);

/**
 * [cfsetispeed(3)](http://man7.org/linux/man-pages/man3/cfsetispeed.3.html)
 * sets the terminal input baud rate.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int cfsetispeed(struct termios* __t, speed_t __speed) __INTRODUCED_IN(21);

/**
 * [cfsetospeed(3)](http://man7.org/linux/man-pages/man3/cfsetospeed.3.html)
 * sets the terminal output baud rate.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int cfsetospeed(struct termios* __t, speed_t __speed) __INTRODUCED_IN(21);

/**
 * [tcdrain(3)](http://man7.org/linux/man-pages/man3/tcdrain.3.html)
 * waits until all output has been written.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int tcdrain(int __fd) __INTRODUCED_IN(21);

/**
 * [tcflow(3)](http://man7.org/linux/man-pages/man3/tcflow.3.html)
 * suspends (`TCOOFF`) or resumes (`TCOON`) output, or transmits a
 * stop (`TCIOFF`) or start (`TCION`) to suspend or resume input.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int tcflow(int __fd, int __action) __INTRODUCED_IN(21);

/**
 * [tcflush(3)](http://man7.org/linux/man-pages/man3/tcflush.3.html)
 * discards pending input (`TCIFLUSH`), output (`TCOFLUSH`), or
 * both (`TCIOFLUSH`). (In `<stdio.h>` terminology, this is a purge rather
 * than a flush.)
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int tcflush(int __fd, int __queue) __INTRODUCED_IN(21);

/**
 * [tcgetattr(3)](http://man7.org/linux/man-pages/man3/tcgetattr.3.html)
 * reads the configuration of the given terminal.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int tcgetattr(int __fd, struct termios* __t) __INTRODUCED_IN(21);

/**
 * [tcgetsid(3)](http://man7.org/linux/man-pages/man3/tcgetsid.3.html)
 * returns the session id corresponding to the given fd.
 *
 * Returns a non-negative session id on success and
 * returns -1 and sets `errno` on failure.
 */
pid_t tcgetsid(int __fd) __INTRODUCED_IN(21);

/**
 * [tcsendbreak(3)](http://man7.org/linux/man-pages/man3/tcsendbreak.3.html)
 * sends a break.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int tcsendbreak(int __fd, int __duration) __INTRODUCED_IN(21);

/**
 * [tcsetattr(3)](http://man7.org/linux/man-pages/man3/tcsetattr.3.html)
 * writes the configuration of the given terminal.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int tcsetattr(int __fd, int __optional_actions, const struct termios* __t) __INTRODUCED_IN(21);

#endif

__END_DECLS

#include <android/legacy_termios_inlines.h>
