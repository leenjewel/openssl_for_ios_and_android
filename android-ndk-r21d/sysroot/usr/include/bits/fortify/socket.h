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

#ifndef _SYS_SOCKET_H_
#error "Never include this file directly; instead, include <sys/socket.h>"
#endif


#if __ANDROID_API__ >= 26
extern ssize_t __sendto_chk(int, const void*, size_t, size_t, int, const struct sockaddr*,
        socklen_t) __INTRODUCED_IN(26);
#endif /* __ANDROID_API__ >= 26 */


#if __ANDROID_API__ >= 21
ssize_t __recvfrom_chk(int, void*, size_t, size_t, int, struct sockaddr*,
        socklen_t*) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


#if defined(__BIONIC_FORTIFY)

__BIONIC_FORTIFY_INLINE
ssize_t recvfrom(int fd, void* const buf __pass_object_size0, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addr_len)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'recvfrom' called with size bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_N__
  size_t bos = __bos0(buf);

  if (!__bos_trivially_ge(bos, len)) {
    return __recvfrom_chk(fd, buf, len, bos, flags, src_addr, addr_len);
  }
#endif /* __ANDROID_API__ >= __ANDROID_API_N__ */
  return __call_bypassing_fortify(recvfrom)(fd, buf, len, flags, src_addr, addr_len);
}

__BIONIC_FORTIFY_INLINE
ssize_t sendto(int fd, const void* const buf __pass_object_size0, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addr_len)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'sendto' called with size bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_N_MR1__
  size_t bos = __bos0(buf);

  if (!__bos_trivially_ge(bos, len)) {
    return __sendto_chk(fd, buf, len, bos, flags, dest_addr, addr_len);
  }
#endif /* __ANDROID_API__ >= __ANDROID_API_N_MR1__ */
  return __call_bypassing_fortify(sendto)(fd, buf, len, flags, dest_addr, addr_len);
}

__BIONIC_FORTIFY_INLINE
ssize_t recv(int socket, void* const buf __pass_object_size0, size_t len, int flags)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'recv' called with size bigger than buffer") {
  return recvfrom(socket, buf, len, flags, NULL, 0);
}

__BIONIC_FORTIFY_INLINE
ssize_t send(int socket, const void* const buf __pass_object_size0, size_t len, int flags)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'send' called with size bigger than buffer") {
  return sendto(socket, buf, len, flags, NULL, 0);
}

#endif /* __BIONIC_FORTIFY */
