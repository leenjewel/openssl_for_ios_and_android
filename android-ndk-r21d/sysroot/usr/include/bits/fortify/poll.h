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

#ifndef _POLL_H_
#error "Never include this file directly; instead, include <poll.h>"
#endif


#if __ANDROID_API__ >= 23
int __poll_chk(struct pollfd*, nfds_t, int, size_t) __INTRODUCED_IN(23);
int __ppoll_chk(struct pollfd*, nfds_t, const struct timespec*, const sigset_t*, size_t) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


#if __ANDROID_API__ >= 28
int __ppoll64_chk(struct pollfd*, nfds_t, const struct timespec*, const sigset64_t*, size_t) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


#if defined(__BIONIC_FORTIFY)
#define __bos_fd_count_trivially_safe(bos_val, fds, fd_count)              \
  __bos_dynamic_check_impl_and((bos_val), >=, (sizeof(*fds) * (fd_count)), \
                               (fd_count) <= __BIONIC_CAST(static_cast, nfds_t, -1) / sizeof(*fds))

__BIONIC_FORTIFY_INLINE
int poll(struct pollfd* const fds __pass_object_size, nfds_t fd_count, int timeout)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos(fds), sizeof(*fds) * fd_count),
                     "in call to 'poll', fd_count is larger than the given buffer") {
#if __ANDROID_API__ >= __ANDROID_API_M__
  size_t bos_fds = __bos(fds);

  if (!__bos_fd_count_trivially_safe(bos_fds, fds, fd_count)) {
    return __poll_chk(fds, fd_count, timeout, bos_fds);
  }
#endif /* __ANDROID_API__ >= __ANDROID_API_M__ */
  return __call_bypassing_fortify(poll)(fds, fd_count, timeout);
}

#if __ANDROID_API__ >= __ANDROID_API_L__
__BIONIC_FORTIFY_INLINE
int ppoll(struct pollfd* const fds __pass_object_size, nfds_t fd_count, const struct timespec* timeout, const sigset_t* mask)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos(fds), sizeof(*fds) * fd_count),
                     "in call to 'ppoll', fd_count is larger than the given buffer") {
#if __ANDROID_API__ >= __ANDROID_API_M__
  size_t bos_fds = __bos(fds);

  if (!__bos_fd_count_trivially_safe(bos_fds, fds, fd_count)) {
    return __ppoll_chk(fds, fd_count, timeout, mask, bos_fds);
  }
#endif /* __ANDROID_API__ >= __ANDROID_API_M__ */
  return __call_bypassing_fortify(ppoll)(fds, fd_count, timeout, mask);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_L__ */

#if __ANDROID_API__ >= __ANDROID_API_P__
__BIONIC_FORTIFY_INLINE
int ppoll64(struct pollfd* const fds __pass_object_size, nfds_t fd_count, const struct timespec* timeout, const sigset64_t* mask)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos(fds), sizeof(*fds) * fd_count),
                     "in call to 'ppoll64', fd_count is larger than the given buffer") {
  size_t bos_fds = __bos(fds);

  if (!__bos_fd_count_trivially_safe(bos_fds, fds, fd_count)) {
    return __ppoll64_chk(fds, fd_count, timeout, mask, bos_fds);
  }
  return __call_bypassing_fortify(ppoll64)(fds, fd_count, timeout, mask);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_P__ */

#undef __bos_fd_count_trivially_safe

#endif /* defined(__BIONIC_FORTIFY) */
