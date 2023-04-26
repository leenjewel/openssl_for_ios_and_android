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

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/memfd.h>
#include <linux/mman.h>

__BEGIN_DECLS

/** Alternative spelling of the `MAP_ANONYMOUS` flag for mmap(). */
#define MAP_ANON MAP_ANONYMOUS

/** Return value for mmap(). */
#define MAP_FAILED __BIONIC_CAST(reinterpret_cast, void*, -1)

/**
 * [mmap(2)](http://man7.org/linux/man-pages/man2/mmap.2.html)
 * creates a memory mapping for the given range.
 *
 * Returns the address of the mapping on success,
 * and returns `MAP_FAILED` and sets `errno` on failure.
 */
#if defined(__USE_FILE_OFFSET64)
void* mmap(void* __addr, size_t __size, int __prot, int __flags, int __fd, off_t __offset) __RENAME(mmap64);
#else
void* mmap(void* __addr, size_t __size, int __prot, int __flags, int __fd, off_t __offset);
#endif

#if __ANDROID_API__ >= __ANDROID_API_L__
/**
 * mmap64() is a variant of mmap() that takes a 64-bit offset even on LP32.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/docs/32-bit-abi.md
 *
 * mmap64 wasn't really around until L, but we added an inline for it since it
 * allows a lot more code to compile with _FILE_OFFSET_BITS=64.
 */
void* mmap64(void* __addr, size_t __size, int __prot, int __flags, int __fd, off64_t __offset) __INTRODUCED_IN(21);
#endif

/**
 * [munmap(2)](http://man7.org/linux/man-pages/man2/munmap.2.html)
 * deletes a memory mapping for the given range.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int munmap(void* __addr, size_t __size);

/**
 * [msync(2)](http://man7.org/linux/man-pages/man2/msync.2.html)
 * flushes changes to a memory-mapped file to disk.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int msync(void* __addr, size_t __size, int __flags);

/**
 * [mprotect(2)](http://man7.org/linux/man-pages/man2/mprotect.2.html)
 * sets the protection on a memory region.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int mprotect(void* __addr, size_t __size, int __prot);

/** Flag for mremap(). */
#define MREMAP_MAYMOVE  1

/** Flag for mremap(). */
#define MREMAP_FIXED    2

/**
 * [mremap(2)](http://man7.org/linux/man-pages/man2/mremap.2.html)
 * expands or shrinks an existing memory mapping.
 *
 * Returns the address of the mapping on success,
 * and returns `MAP_FAILED` and sets `errno` on failure.
 */
void* mremap(void* __old_addr, size_t __old_size, size_t __new_size, int __flags, ...);

/**
 * [mlockall(2)](http://man7.org/linux/man-pages/man2/mlockall.2.html)
 * locks pages (preventing swapping).
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */

#if __ANDROID_API__ >= 17
int mlockall(int __flags) __INTRODUCED_IN(17);

/**
 * [munlockall(2)](http://man7.org/linux/man-pages/man2/munlockall.2.html)
 * unlocks pages (allowing swapping).
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int munlockall(void) __INTRODUCED_IN(17);
#endif /* __ANDROID_API__ >= 17 */


/**
 * [mlock(2)](http://man7.org/linux/man-pages/man2/mlock.2.html)
 * locks pages (preventing swapping).
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int mlock(const void* __addr, size_t __size);

/**
 * [mlock2(2)](http://man7.org/linux/man-pages/man2/mlock.2.html)
 * locks pages (preventing swapping), with optional flags.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */

#if __ANDROID_API__ >= 30
int mlock2(const void* __addr, size_t __size, int __flags) __INTRODUCED_IN(30);
#endif /* __ANDROID_API__ >= 30 */


/**
 * [munlock(2)](http://man7.org/linux/man-pages/man2/munlock.2.html)
 * unlocks pages (allowing swapping).
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int munlock(const void* __addr, size_t __size);

/**
 * [mincore(2)](http://man7.org/linux/man-pages/man2/mincore.2.html)
 * tests whether pages are resident in memory.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int mincore(void* __addr, size_t __size, unsigned char* __vector);

/**
 * [madvise(2)](http://man7.org/linux/man-pages/man2/madvise.2.html)
 * gives the kernel advice about future usage patterns.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int madvise(void* __addr, size_t __size, int __advice);

#if defined(__USE_GNU)

/**
 * [memfd_create(2)](http://man7.org/linux/man-pages/man2/memfd_create.2.html)
 * creates an anonymous file.
 *
 * Returns an fd on success, and returns -1 and sets `errno` on failure.
 */

#if __ANDROID_API__ >= 30
int memfd_create(const char* __name, unsigned __flags) __INTRODUCED_IN(30);
#endif /* __ANDROID_API__ >= 30 */


#endif

#if __ANDROID_API__ >= __ANDROID_API_M__

/*
 * Some third-party code uses the existence of POSIX_MADV_NORMAL to detect the
 * availability of posix_madvise. This is not correct, since having up-to-date
 * UAPI headers says nothing about the C library, but for the time being we
 * don't want to harm adoption of the unified headers.
 *
 * https://github.com/android-ndk/ndk/issues/395
 */

/** Flag for posix_madvise(). */
#define POSIX_MADV_NORMAL     MADV_NORMAL
/** Flag for posix_madvise(). */
#define POSIX_MADV_RANDOM     MADV_RANDOM
/** Flag for posix_madvise(). */
#define POSIX_MADV_SEQUENTIAL MADV_SEQUENTIAL
/** Flag for posix_madvise(). */
#define POSIX_MADV_WILLNEED   MADV_WILLNEED
/** Flag for posix_madvise(). */
#define POSIX_MADV_DONTNEED   MADV_DONTNEED

#endif

/**
 * [posix_madvise(3)](http://man7.org/linux/man-pages/man3/posix_madvise.3.html)
 * gives the kernel advice about future usage patterns.
 *
 * Returns 0 on success, and returns a positive error number on failure.
 *
 * See also madvise() which has been available much longer.
 */

#if __ANDROID_API__ >= 23
int posix_madvise(void* __addr, size_t __size, int __advice) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


__END_DECLS

#include <android/legacy_sys_mman_inlines.h>
