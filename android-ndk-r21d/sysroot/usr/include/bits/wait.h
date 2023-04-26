/*
 * Copyright (C) 2017 The Android Open Source Project
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
 * @file bits/wait.h
 * @brief Process exit status macros.
 */

#include <sys/cdefs.h>

#include <linux/wait.h>

/** Returns the exit status from a process for which `WIFEXITED` is true. */
#define WEXITSTATUS(__status) (((__status) & 0xff00) >> 8)

/** Returns true if a process dumped core. */
#define WCOREDUMP(__status) ((__status) & 0x80)

/** Returns the terminating signal from a process, or 0 if it exited normally. */
#define WTERMSIG(__status) ((__status) & 0x7f)

/** Returns the signal that stopped the process, if `WIFSTOPPED` is true. */
#define WSTOPSIG(__status) WEXITSTATUS(__status)

/** Returns true if the process exited normally. */
#define WIFEXITED(__status) (WTERMSIG(__status) == 0)

/** Returns true if the process was stopped by a signal. */
#define WIFSTOPPED(__status) (WTERMSIG(__status) == 0x7f)

/** Returns true if the process was terminated by a signal. */
#define WIFSIGNALED(__status) (WTERMSIG((__status)+1) >= 2)

/** Returns true if the process was resumed . */
#define WIFCONTINUED(__status) ((__status) == 0xffff)

/** Constructs a status value from the given exit code and signal number. */
#define W_EXITCODE(__exit_code, __signal_number) ((__exit_code) << 8 | (__signal_number))

/** Constructs a status value for a process stopped by the given signal. */
#define W_STOPCODE(__signal_number) ((__signal_number) << 8 | 0x7f)
