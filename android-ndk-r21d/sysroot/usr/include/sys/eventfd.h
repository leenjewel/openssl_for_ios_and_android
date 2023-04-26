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
 * @file sys/eventfd.h
 * @brief Event notification file descriptors.
 */

#include <sys/cdefs.h>
#include <fcntl.h>

__BEGIN_DECLS

/** The eventfd() flag to provide semaphore-like semantics for reads. */
#define EFD_SEMAPHORE (1 << 0)
/** The eventfd() flag for a close-on-exec file descriptor. */
#define EFD_CLOEXEC O_CLOEXEC
/** The eventfd() flag for a non-blocking file descriptor. */
#define EFD_NONBLOCK O_NONBLOCK

/**
 * [eventfd(2)](http://man7.org/linux/man-pages/man2/eventfd.2.html) creates a file descriptor
 * for event notification.
 *
 * Returns a new file descriptor on success, and returns -1 and sets `errno` on failure.
 */
int eventfd(unsigned int __initial_value, int __flags);

/** The type used by eventfd_read() and eventfd_write(). */
typedef uint64_t eventfd_t;

/**
 * [eventfd_read(3)](http://man7.org/linux/man-pages/man2/eventfd.2.html) is a convenience
 * wrapper to read an `eventfd_t` from an eventfd file descriptor.
 *
 * Returns 0 on success, or returns -1 otherwise.
 */
int eventfd_read(int __fd, eventfd_t* __value);

/**
 * [eventfd_write(3)](http://man7.org/linux/man-pages/man2/eventfd.2.html) is a convenience
 * wrapper to write an `eventfd_t` to an eventfd file descriptor.
 *
 * Returns 0 on success, or returns -1 otherwise.
 */
int eventfd_write(int __fd, eventfd_t __value);

__END_DECLS
