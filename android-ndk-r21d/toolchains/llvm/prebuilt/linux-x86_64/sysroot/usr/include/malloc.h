/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

/**
 * @file malloc.h
 * @brief Heap memory allocation.
 *
 * [Debugging Native Memory Use](https://source.android.com/devices/tech/debug/native-memory)
 * is the canonical source for documentation on Android's heap debugging
 * features.
 */

#include <sys/cdefs.h>
#include <stddef.h>
#include <stdio.h>

__BEGIN_DECLS

#define __BIONIC_ALLOC_SIZE(...) __attribute__((__alloc_size__(__VA_ARGS__)))

/**
 * [malloc(3)](http://man7.org/linux/man-pages/man3/malloc.3.html) allocates
 * memory on the heap.
 *
 * Returns a pointer to the allocated memory on success and returns a null
 * pointer and sets `errno` on failure.
 */
void* malloc(size_t __byte_count) __mallocfunc __BIONIC_ALLOC_SIZE(1) __wur;

/**
 * [calloc(3)](http://man7.org/linux/man-pages/man3/calloc.3.html) allocates
 * and clears memory on the heap.
 *
 * Returns a pointer to the allocated memory on success and returns a null
 * pointer and sets `errno` on failure.
 */
void* calloc(size_t __item_count, size_t __item_size) __mallocfunc __BIONIC_ALLOC_SIZE(1,2) __wur;

/**
 * [realloc(3)](http://man7.org/linux/man-pages/man3/realloc.3.html) resizes
 * allocated memory on the heap.
 *
 * Returns a pointer (which may be different from `__ptr`) to the resized
 * memory on success and returns a null pointer and sets `errno` on failure.
 */
void* realloc(void* __ptr, size_t __byte_count) __BIONIC_ALLOC_SIZE(2) __wur;

/**
 * [reallocarray(3)](http://man7.org/linux/man-pages/man3/realloc.3.html) resizes
 * allocated memory on the heap.
 *
 * Equivalent to `realloc(__ptr, __item_count * __item_size)` but fails if the
 * multiplication overflows.
 *
 * Returns a pointer (which may be different from `__ptr`) to the resized
 * memory on success and returns a null pointer and sets `errno` on failure.
 */

#if __ANDROID_API__ >= 29
void* reallocarray(void* __ptr, size_t __item_count, size_t __item_size) __BIONIC_ALLOC_SIZE(2, 3) __wur __INTRODUCED_IN(29);
#endif /* __ANDROID_API__ >= 29 */


/**
 * [free(3)](http://man7.org/linux/man-pages/man3/free.3.html) deallocates
 * memory on the heap.
 */
void free(void* __ptr);

/**
 * [memalign(3)](http://man7.org/linux/man-pages/man3/memalign.3.html) allocates
 * memory on the heap with the required alignment.
 *
 * Returns a pointer to the allocated memory on success and returns a null
 * pointer and sets `errno` on failure.
 *
 * See also posix_memalign().
 */
void* memalign(size_t __alignment, size_t __byte_count) __mallocfunc __BIONIC_ALLOC_SIZE(2) __wur;

/**
 * [malloc_usable_size(3)](http://man7.org/linux/man-pages/man3/malloc_usable_size.3.html)
 * returns the actual size of the given heap block.
 *
 * Available since API level 17.
 */

#if __ANDROID_API__ >= 17
size_t malloc_usable_size(const void* __ptr) __INTRODUCED_IN(17);
#endif /* __ANDROID_API__ >= 17 */


#ifndef STRUCT_MALLINFO_DECLARED
#define STRUCT_MALLINFO_DECLARED 1
struct mallinfo {
  /** Total number of non-mmapped bytes currently allocated from OS. */
  size_t arena;
  /** Number of free chunks. */
  size_t ordblks;
  /** (Unused.) */
  size_t smblks;
  /** (Unused.) */
  size_t hblks;
  /** Total number of bytes in mmapped regions. */
  size_t hblkhd;
  /** Maximum total allocated space; greater than total if trimming has occurred. */
  size_t usmblks;
  /** (Unused.) */
  size_t fsmblks;
  /** Total allocated space (normal or mmapped.) */
  size_t uordblks;
  /** Total free space. */
  size_t fordblks;
  /** Upper bound on number of bytes releasable by a trim operation. */
  size_t keepcost;
};
#endif

/**
 * [mallinfo(3)](http://man7.org/linux/man-pages/man3/mallinfo.3.html) returns
 * information about the current state of the heap. Note that mallinfo() is
 * inherently unreliable and consider using malloc_info() instead.
 */
struct mallinfo mallinfo(void);

/**
 * [malloc_info(3)](http://man7.org/linux/man-pages/man3/malloc_info.3.html)
 * writes information about the current state of the heap to the given stream.
 *
 * The XML structure for malloc_info() is as follows:
 * ```
 * <malloc version="jemalloc-1">
 *   <heap nr="INT">
 *     <allocated-large>INT</allocated-large>
 *     <allocated-huge>INT</allocated-huge>
 *     <allocated-bins>INT</allocated-bins>
 *     <bins-total>INT</bins-total>
 *     <bin nr="INT">
 *       <allocated>INT</allocated>
 *       <nmalloc>INT</nmalloc>
 *       <ndalloc>INT</ndalloc>
 *     </bin>
 *     <!-- more bins -->
 *   </heap>
 *   <!-- more heaps -->
 * </malloc>
 * ```
 *
 * Available since API level 23.
 */

#if __ANDROID_API__ >= 23
int malloc_info(int __must_be_zero, FILE* __fp) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


/**
 * mallopt() option to set the decay time. Valid values are 0 and 1.
 *
 * Available since API level 27.
 */
#define M_DECAY_TIME -100
/**
 * mallopt() option to immediately purge any memory not in use. This
 * will release the memory back to the kernel. The value is ignored.
 *
 * Available since API level 28.
 */
#define M_PURGE -101

/**
 * [mallopt(3)](http://man7.org/linux/man-pages/man3/mallopt.3.html) modifies
 * heap behavior. Values of `__option` are the `M_` constants from this header.
 *
 * Returns 1 on success, 0 on error.
 *
 * Available since API level 26.
 */

#if __ANDROID_API__ >= 26
int mallopt(int __option, int __value) __INTRODUCED_IN(26);
#endif /* __ANDROID_API__ >= 26 */


/**
 * [__malloc_hook(3)](http://man7.org/linux/man-pages/man3/__malloc_hook.3.html)
 * is called to implement malloc(). By default this points to the system's
 * implementation.
 *
 * Available since API level 28.
 *
 * See also: [extra documentation](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_hooks/README.md)
 */

#if __ANDROID_API__ >= 28
extern void* (*volatile __malloc_hook)(size_t __byte_count, const void* __caller) __INTRODUCED_IN(28);

/**
 * [__realloc_hook(3)](http://man7.org/linux/man-pages/man3/__realloc_hook.3.html)
 * is called to implement realloc(). By default this points to the system's
 * implementation.
 *
 * Available since API level 28.
 *
 * See also: [extra documentation](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_hooks/README.md)
 */
extern void* (*volatile __realloc_hook)(void* __ptr, size_t __byte_count, const void* __caller) __INTRODUCED_IN(28);

/**
 * [__free_hook(3)](http://man7.org/linux/man-pages/man3/__free_hook.3.html)
 * is called to implement free(). By default this points to the system's
 * implementation.
 *
 * Available since API level 28.
 *
 * See also: [extra documentation](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_hooks/README.md)
 */
extern void (*volatile __free_hook)(void* __ptr, const void* __caller) __INTRODUCED_IN(28);

/**
 * [__memalign_hook(3)](http://man7.org/linux/man-pages/man3/__memalign_hook.3.html)
 * is called to implement memalign(). By default this points to the system's
 * implementation.
 *
 * Available since API level 28.
 *
 * See also: [extra documentation](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_hooks/README.md)
 */
extern void* (*volatile __memalign_hook)(size_t __alignment, size_t __byte_count, const void* __caller) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */


__END_DECLS
