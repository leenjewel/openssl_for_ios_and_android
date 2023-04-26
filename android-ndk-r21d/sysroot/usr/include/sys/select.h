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
 * @file sys/select.h
 * @brief Wait for events on a set of file descriptors (but use <poll.h> instead).
 */

#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/time.h>
#include <signal.h>

__BEGIN_DECLS

typedef unsigned long fd_mask;

/**
 * The limit on the largest fd that can be used with this API.
 * Use <poll.h> instead.
 */
#define FD_SETSIZE 1024
#define NFDBITS (8 * sizeof(fd_mask))

/**
 * The type of a file descriptor set. Limited to 1024 fds.
 * Use <poll.h> instead.
 */
typedef struct {
  fd_mask fds_bits[FD_SETSIZE/NFDBITS];
} fd_set;

#define __FDELT(fd) ((fd) / NFDBITS)
#define __FDMASK(fd) (1UL << ((fd) % NFDBITS))
#define __FDS_BITS(type,set) (__BIONIC_CAST(static_cast, type, set)->fds_bits)

/* Inline loop so we don't have to declare memset. */
#define FD_ZERO(set) \
  do { \
    size_t __i; \
    for (__i = 0; __i < sizeof(fd_set)/sizeof(fd_mask); ++__i) { \
      (set)->fds_bits[__i] = 0; \
    } \
  } while (0)


#if __ANDROID_API__ >= 21
void __FD_CLR_chk(int, fd_set*, size_t) __INTRODUCED_IN(21);
void __FD_SET_chk(int, fd_set*, size_t) __INTRODUCED_IN(21);
int __FD_ISSET_chk(int, const fd_set*, size_t) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


#define __FD_CLR(fd, set) (__FDS_BITS(fd_set*,set)[__FDELT(fd)] &= ~__FDMASK(fd))
#define __FD_SET(fd, set) (__FDS_BITS(fd_set*,set)[__FDELT(fd)] |= __FDMASK(fd))
#define __FD_ISSET(fd, set) ((__FDS_BITS(const fd_set*,set)[__FDELT(fd)] & __FDMASK(fd)) != 0)

#if __ANDROID_API__ >= __ANDROID_API_L__

/** Removes `fd` from the given set. Use <poll.h> instead. */
#define FD_CLR(fd, set) __FD_CLR_chk(fd, set, __bos(set))
/** Adds `fd` to the given set. Use <poll.h> instead. */
#define FD_SET(fd, set) __FD_SET_chk(fd, set, __bos(set))
/** Tests whether `fd` is in the given set. Use <poll.h> instead. */
#define FD_ISSET(fd, set) __FD_ISSET_chk(fd, set, __bos(set))

#else

/** Removes `fd` from the given set. Use <poll.h> instead. */
#define FD_CLR(fd, set) __FD_CLR(fd, set)
/** Adds `fd` to the given set. Use <poll.h> instead. */
#define FD_SET(fd, set) __FD_SET(fd, set)
/** Tests whether `fd` is in the given set. Use <poll.h> instead. */
#define FD_ISSET(fd, set) __FD_ISSET(fd, set)

#endif /* __ANDROID_API >= 21 */

/**
 * [select(2)](http://man7.org/linux/man-pages/man2/select.2.html) waits on a
 * set of file descriptors. Use poll() instead.
 *
 * Returns the number of ready file descriptors on success, 0 for timeout,
 * and returns -1 and sets `errno` on failure.
 */
int select(int __fd_count, fd_set* __read_fds, fd_set* __write_fds, fd_set* __exception_fds, struct timeval* __timeout);

/**
 * [pselect(2)](http://man7.org/linux/man-pages/man2/select.2.html) waits on a
 * set of file descriptors. Use ppoll() instead.
 *
 * Returns the number of ready file descriptors on success, 0 for timeout,
 * and returns -1 and sets `errno` on failure.
 */
int pselect(int __fd_count, fd_set* __read_fds, fd_set* __write_fds, fd_set* __exception_fds, const struct timespec* __timeout, const sigset_t* __mask);

/**
 * [pselect64(2)](http://man7.org/linux/man-pages/man2/select.2.html) waits on a
 * set of file descriptors. Use ppoll64() instead.
 *
 * Returns the number of ready file descriptors on success, 0 for timeout,
 * and returns -1 and sets `errno` on failure.
 *
 * Available since API level 28.
 */

#if __ANDROID_API__ >= 28
int pselect64(int __fd_count, fd_set* __read_fds, fd_set* __write_fds, fd_set* __exception_fds, const struct timespec* __timeout, const sigset64_t* __mask) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


__END_DECLS
