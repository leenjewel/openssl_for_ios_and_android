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

/**
 * @file threads.h
 * @brief C11 threads.
 */

#include <sys/cdefs.h>

#include <pthread.h>
#include <time.h>

#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS

/** The type for a condition variable. */
typedef pthread_cond_t cnd_t;
/** The type for a thread. */
typedef pthread_t thrd_t;
/** The type for a thread-specific storage key. */
typedef pthread_key_t tss_t;
/** The type for a mutex. */
typedef pthread_mutex_t mtx_t;

/** The type for a thread-specific storage destructor. */
typedef void (*tss_dtor_t)(void*);
/** The type of the function passed to thrd_create() to create a new thread. */
typedef int (*thrd_start_t)(void*);

/** The type used by call_once(). */
typedef pthread_once_t once_flag;

enum {
  mtx_plain = 0x1,
  mtx_recursive = 0x2,
  mtx_timed = 0x4,
};

enum {
  thrd_success = 0,
  thrd_busy = 1,
  thrd_error = 2,
  thrd_nomem = 3,
  thrd_timedout = 4,
};

#if !defined(__cplusplus)
#define thread_local _Thread_local
#endif

__BEGIN_DECLS

#if __ANDROID_API__ >= __ANDROID_API_R__
// This file is implemented as static inlines before API level 30.

/** Uses `__flag` to ensure that `__function` is called exactly once. */
void call_once(once_flag* __flag, void (*__function)(void)) __INTRODUCED_IN(30);



/**
 * Unblocks all threads blocked on `__cond`.
 */
int cnd_broadcast(cnd_t* __cond) __INTRODUCED_IN(30);

/**
 * Destroys a condition variable.
 */
void cnd_destroy(cnd_t* __cond) __INTRODUCED_IN(30);

/**
 * Creates a condition variable.
 */
int cnd_init(cnd_t* __cond) __INTRODUCED_IN(30);

/**
 * Unblocks one thread blocked on `__cond`.
 */
int cnd_signal(cnd_t* __cond) __INTRODUCED_IN(30);

/**
 * Unlocks `__mutex` and blocks until `__cond` is signaled or `__timeout` occurs.
 */
int cnd_timedwait(cnd_t* __cond, mtx_t* __mutex, const struct timespec* __timeout)
    __INTRODUCED_IN(30);

/**
 * Unlocks `__mutex` and blocks until `__cond` is signaled.
 */
int cnd_wait(cnd_t* __cond, mtx_t* __mutex) __INTRODUCED_IN(30);



/**
 * Destroys a mutex.
 */
void mtx_destroy(mtx_t* __mutex) __INTRODUCED_IN(30);

/**
 * Creates a mutex.
 */
int mtx_init(mtx_t* __mutex, int __type) __INTRODUCED_IN(30);

/**
 * Blocks until `__mutex` is acquired.
 */
int mtx_lock(mtx_t* __mutex) __INTRODUCED_IN(30);

/**
 * Blocks until `__mutex` is acquired or `__timeout` expires.
 */
int mtx_timedlock(mtx_t* __mutex, const struct timespec* __timeout) __INTRODUCED_IN(30);

/**
 * Acquires `__mutex` or returns `thrd_busy`.
 */
int mtx_trylock(mtx_t* __mutex) __INTRODUCED_IN(30);

/**
 * Unlocks `__mutex`.
 */
int mtx_unlock(mtx_t* __mutex) __INTRODUCED_IN(30);



/**
 * Creates a new thread running `__function(__arg)`, and sets `*__thrd` to
 * the new thread.
 */
int thrd_create(thrd_t* __thrd, thrd_start_t __function, void* __arg) __INTRODUCED_IN(30);

/**
 * Returns the `thrd_t` corresponding to the caller.
 */
thrd_t thrd_current(void) __INTRODUCED_IN(30);

/**
 * Tells the OS to automatically dispose of `__thrd` when it exits.
 */
int thrd_detach(thrd_t __thrd) __INTRODUCED_IN(30);

/**
 * Tests whether two threads are the same thread.
 */
int thrd_equal(thrd_t __lhs, thrd_t __rhs) __INTRODUCED_IN(30);

/**
 * Terminates the calling thread, setting its result to `__result`.
 */
void thrd_exit(int __result) __noreturn __INTRODUCED_IN(30);

/**
 * Blocks until `__thrd` terminates. If `__result` is not null, `*__result`
 * is set to the exiting thread's result.
 */
int thrd_join(thrd_t __thrd, int* __result) __INTRODUCED_IN(30);

/**
 * Blocks the caller for at least `__duration` unless a signal is delivered.
 * If a signal causes the sleep to end early and `__remaining` is not null,
 * `*__remaining` is set to the time remaining.
 *
 * Returns 0 on success, or -1 if a signal was delivered.
 */
int thrd_sleep(const struct timespec* __duration, struct timespec* __remaining) __INTRODUCED_IN(30);

/**
 * Request that other threads should be scheduled.
 */
void thrd_yield(void) __INTRODUCED_IN(30);



/**
 * Creates a thread-specific storage key with the associated destructor (which
 * may be null).
 */
int tss_create(tss_t* __key, tss_dtor_t __dtor) __INTRODUCED_IN(30);

/**
 * Destroys a thread-specific storage key.
 */
void tss_delete(tss_t __key) __INTRODUCED_IN(30);

/**
 * Returns the value for the current thread held in the thread-specific storage
 * identified by `__key`.
 */
void* tss_get(tss_t __key) __INTRODUCED_IN(30);

/**
 * Sets the current thread's value for the thread-specific storage identified
 * by `__key` to `__value`.
 */
int tss_set(tss_t __key, void* __value) __INTRODUCED_IN(30);

#endif

__END_DECLS

#include <android/legacy_threads_inlines.h>
