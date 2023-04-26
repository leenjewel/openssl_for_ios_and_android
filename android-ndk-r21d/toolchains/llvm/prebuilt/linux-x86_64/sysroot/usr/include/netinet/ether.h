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
 * @file netinet/ether.h
 * @brief Ethernet (MAC) addresses.
 */

#include <sys/cdefs.h>
#include <netinet/if_ether.h>

__BEGIN_DECLS

/**
 * [ether_ntoa(3)](http://man7.org/linux/man-pages/man3/ether_ntoa.3.html) returns a string
 * representation of the given Ethernet address.
 *
 * Returns a pointer to a static buffer.
 */
char* ether_ntoa(const struct ether_addr* __addr);

/**
 * [ether_ntoa_r(3)](http://man7.org/linux/man-pages/man3/ether_ntoa_r.3.html) returns a string
 * representation of the given Ethernet address.
 *
 * Returns a pointer to the given buffer.
 */
char* ether_ntoa_r(const struct ether_addr* __addr, char* __buf);

/**
 * [ether_aton(3)](http://man7.org/linux/man-pages/man3/ether_aton.3.html) returns an `ether_addr`
 * corresponding to the given Ethernet address string.
 *
 * Returns a pointer to a static buffer, or NULL if the given string isn't a valid MAC address.
 */
struct ether_addr* ether_aton(const char* __ascii);

/**
 * [ether_aton_r(3)](http://man7.org/linux/man-pages/man3/ether_aton_r.3.html) returns an
 * `ether_addr` corresponding to the given Ethernet address string.
 *
 * Returns a pointer to the given buffer, or NULL if the given string isn't a valid MAC address.
 */
struct ether_addr* ether_aton_r(const char* __ascii, struct ether_addr* __addr);

__END_DECLS
