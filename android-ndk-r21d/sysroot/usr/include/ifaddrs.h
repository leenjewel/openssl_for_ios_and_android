/*
 * Copyright (C) 2015 The Android Open Source Project
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
 * @file ifaddrs.h
 * @brief Access to network interface addresses.
 */

#include <sys/cdefs.h>
#include <netinet/in.h>
#include <sys/socket.h>

__BEGIN_DECLS

/**
 * Returned by getifaddrs() and freed by freeifaddrs().
 */
struct ifaddrs {
  /** Pointer to the next element in the linked list. */
  struct ifaddrs* ifa_next;

  /** Interface name. */
  char* ifa_name;
  /** Interface flags (like `SIOCGIFFLAGS`). */
  unsigned int ifa_flags;
  /** Interface address. */
  struct sockaddr* ifa_addr;
  /** Interface netmask. */
  struct sockaddr* ifa_netmask;

  union {
    /** Interface broadcast address (if IFF_BROADCAST is set). */
    struct sockaddr* ifu_broadaddr;
    /** Interface destination address (if IFF_POINTOPOINT is set). */
    struct sockaddr* ifu_dstaddr;
  } ifa_ifu;

  /** Unused. */
  void* ifa_data;
};

/** Synonym for `ifa_ifu.ifu_broadaddr` in `struct ifaddrs`. */
#define ifa_broadaddr ifa_ifu.ifu_broadaddr
/** Synonym for `ifa_ifu.ifu_dstaddr` in `struct ifaddrs`. */
#define ifa_dstaddr ifa_ifu.ifu_dstaddr

/**
 * [getifaddrs(3)](http://man7.org/linux/man-pages/man3/getifaddrs.3.html) creates a linked list
 * of `struct ifaddrs`. The list must be freed by freeifaddrs().
 *
 * Returns 0 and stores the list in `*__list_ptr` on success,
 * and returns -1 and sets `errno` on failure.
 *
 * Available since API level 24.
 */

#if __ANDROID_API__ >= 24
int getifaddrs(struct ifaddrs** __list_ptr) __INTRODUCED_IN(24);

/**
 * [freeifaddrs(3)](http://man7.org/linux/man-pages/man3/freeifaddrs.3.html) frees a linked list
 * of `struct ifaddrs` returned by getifaddrs().
 *
 * Available since API level 24.
 */
void freeifaddrs(struct ifaddrs* __ptr) __INTRODUCED_IN(24);
#endif /* __ANDROID_API__ >= 24 */


__END_DECLS
