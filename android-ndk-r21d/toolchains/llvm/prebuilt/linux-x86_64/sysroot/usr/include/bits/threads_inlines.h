/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <threads.h>

#include <errno.h>
#include <sched.h>
#include <stdlib.h>

#if defined(__BIONIC_THREADS_INLINE)

__BEGIN_DECLS

static __inline int __bionic_thrd_error(int __pthread_code) {
  switch (__pthread_code) {
    case 0: return 0;
    case ENOMEM: return thrd_nomem;
    case ETIMEDOUT: return thrd_timedout;
    case EBUSY: return thrd_busy;
    default: return thrd_error;
  }
}

__BIONIC_THREADS_INLINE void call_once(once_flag* __flag,
                                       void (*__function)(void)) {
  pthread_once(__flag, __function);
}



__BIONIC_THREADS_INLINE int cnd_broadcast(cnd_t* __cnd) {
  return __bionic_thrd_error(pthread_cond_broadcast(__cnd));
}

__BIONIC_THREADS_INLINE void cnd_destroy(cnd_t* __cnd) {
  pthread_cond_destroy(__cnd);
}

__BIONIC_THREADS_INLINE int cnd_init(cnd_t* __cnd) {
  return __bionic_thrd_error(pthread_cond_init(__cnd, NULL));
}

__BIONIC_THREADS_INLINE int cnd_signal(cnd_t* __cnd) {
  return __bionic_thrd_error(pthread_cond_signal(__cnd));
}

__BIONIC_THREADS_INLINE int cnd_timedwait(cnd_t* __cnd,
                                          mtx_t* __mtx,
                                          const struct timespec* __timeout) {
  return __bionic_thrd_error(pthread_cond_timedwait(__cnd, __mtx, __timeout));
}

__BIONIC_THREADS_INLINE int cnd_wait(cnd_t* __cnd, mtx_t* __mtx) {
  return __bionic_thrd_error(pthread_cond_wait(__cnd, __mtx));
}



__BIONIC_THREADS_INLINE void mtx_destroy(mtx_t* __mtx) {
  pthread_mutex_destroy(__mtx);
}

__BIONIC_THREADS_INLINE int mtx_init(mtx_t* __mtx, int __type) {
  int __pthread_type = (__type & mtx_recursive) ? PTHREAD_MUTEX_RECURSIVE
                                                : PTHREAD_MUTEX_NORMAL;
  __type &= ~mtx_recursive;
  if (__type != mtx_plain && __type != mtx_timed) return thrd_error;

  pthread_mutexattr_t __attr;
  pthread_mutexattr_init(&__attr);
  pthread_mutexattr_settype(&__attr, __pthread_type);
  return __bionic_thrd_error(pthread_mutex_init(__mtx, &__attr));
}

__BIONIC_THREADS_INLINE int mtx_lock(mtx_t* __mtx) {
  return __bionic_thrd_error(pthread_mutex_lock(__mtx));
}

#if __ANDROID_API__ >= __ANDROID_API_L__
__BIONIC_THREADS_INLINE int mtx_timedlock(mtx_t* __mtx,
                                          const struct timespec* __timeout) {
  return __bionic_thrd_error(pthread_mutex_timedlock(__mtx, __timeout));
}
#endif

__BIONIC_THREADS_INLINE int mtx_trylock(mtx_t* __mtx) {
  return __bionic_thrd_error(pthread_mutex_trylock(__mtx));
}

__BIONIC_THREADS_INLINE int mtx_unlock(mtx_t* __mtx) {
  return __bionic_thrd_error(pthread_mutex_unlock(__mtx));
}



struct __bionic_thrd_data {
  thrd_start_t __func;
  void* __arg;
};

static inline void* __bionic_thrd_trampoline(void* __arg) {
  struct __bionic_thrd_data __data =
      *__BIONIC_CAST(static_cast, struct __bionic_thrd_data*, __arg);
  free(__arg);
  int __result = __data.__func(__data.__arg);
  return __BIONIC_CAST(reinterpret_cast, void*,
                       __BIONIC_CAST(static_cast, uintptr_t, __result));
}

__BIONIC_THREADS_INLINE int thrd_create(thrd_t* __thrd,
                                        thrd_start_t __func,
                                        void* __arg) {
  struct __bionic_thrd_data* __pthread_arg =
      __BIONIC_CAST(static_cast, struct __bionic_thrd_data*,
                    malloc(sizeof(struct __bionic_thrd_data)));
  __pthread_arg->__func = __func;
  __pthread_arg->__arg = __arg;
  int __result = __bionic_thrd_error(pthread_create(__thrd, NULL,
                                                    __bionic_thrd_trampoline,
                                                    __pthread_arg));
  if (__result != thrd_success) free(__pthread_arg);
  return __result;
}

__BIONIC_THREADS_INLINE thrd_t thrd_current(void) {
  return pthread_self();
}

__BIONIC_THREADS_INLINE int thrd_detach(thrd_t __thrd) {
  return __bionic_thrd_error(pthread_detach(__thrd));
}

__BIONIC_THREADS_INLINE int thrd_equal(thrd_t __lhs, thrd_t __rhs) {
  return pthread_equal(__lhs, __rhs);
}

__BIONIC_THREADS_INLINE void thrd_exit(int __result) {
  pthread_exit(__BIONIC_CAST(reinterpret_cast, void*,
                             __BIONIC_CAST(static_cast, uintptr_t, __result)));
}

__BIONIC_THREADS_INLINE int thrd_join(thrd_t __thrd, int* __result) {
  void* __pthread_result;
  if (pthread_join(__thrd, &__pthread_result) != 0) return thrd_error;
  if (__result) {
    *__result = __BIONIC_CAST(reinterpret_cast, intptr_t, __pthread_result);
  }
  return thrd_success;
}

__BIONIC_THREADS_INLINE int thrd_sleep(const struct timespec* __duration,
                                       struct timespec* __remaining) {
  int __rc = nanosleep(__duration, __remaining);
  if (__rc == 0) return 0;
  return (errno == EINTR) ? -1 : -2;
}

__BIONIC_THREADS_INLINE void thrd_yield(void) {
  sched_yield();
}



__BIONIC_THREADS_INLINE int tss_create(tss_t* __key, tss_dtor_t __dtor) {
  return __bionic_thrd_error(pthread_key_create(__key, __dtor));
}

__BIONIC_THREADS_INLINE void tss_delete(tss_t __key) {
  pthread_key_delete(__key);
}

__BIONIC_THREADS_INLINE void* tss_get(tss_t __key) {
  return pthread_getspecific(__key);
}

__BIONIC_THREADS_INLINE int tss_set(tss_t __key, void* __value) {
  return __bionic_thrd_error(pthread_setspecific(__key, __value));
}

__END_DECLS

#endif  // __BIONIC_THREADS_INLINE
