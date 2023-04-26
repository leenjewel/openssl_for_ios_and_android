/*
 * Copyright (C) 2014 The Android Open Source Project
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
 * @file pty.h
 * @brief Pseudoterminal functions.
 */

#include <sys/cdefs.h>

#include <termios.h>
#include <sys/ioctl.h>

__BEGIN_DECLS

/**
 * [openpty(3)](http://man7.org/linux/man-pages/man3/openpty.3.html) finds
 * a free pseudoterminal and configures it with the given terminal and window
 * size settings.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
int openpty(int* _Nonnull __master_fd, int* _Nonnull __slave_fd, char* _Nullable __slave_name, const struct termios* _Nullable __termios_ptr, const struct winsize* _Nullable __winsize_ptr) __INTRODUCED_IN(23);

/**
 * [forkpty(3)](http://man7.org/linux/man-pages/man3/forkpty.3.html) creates
 * a new process connected to a pseudoterminal from openpty().
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 *
 * Available since API level 23.
 */
int forkpty(int* _Nonnull __master_fd, char* _Nullable __slave_name, const struct termios* _Nullable __termios_ptr, const struct winsize* _Nullable __winsize_ptr) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


__END_DECLS
