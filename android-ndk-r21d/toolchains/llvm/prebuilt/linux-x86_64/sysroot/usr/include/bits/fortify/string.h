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

#ifndef _STRING_H
#error "Never include this file directly; instead, include <string.h>"
#endif


#if __ANDROID_API__ >= 23
void* __memchr_chk(const void*, int, size_t, size_t) __INTRODUCED_IN(23);
void* __memrchr_chk(const void*, int, size_t, size_t) __INTRODUCED_IN(23);
#endif /* __ANDROID_API__ >= 23 */


#if __ANDROID_API__ >= 21
char* __stpncpy_chk2(char*, const char*, size_t, size_t, size_t) __INTRODUCED_IN(21);
char* __strncpy_chk2(char*, const char*, size_t, size_t, size_t) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


#if __ANDROID_API__ >= 17
size_t __strlcpy_chk(char*, const char*, size_t, size_t) __INTRODUCED_IN(17);
size_t __strlcat_chk(char*, const char*, size_t, size_t) __INTRODUCED_IN(17);
#endif /* __ANDROID_API__ >= 17 */


#if defined(__BIONIC_FORTIFY)
extern void* __memrchr_real(const void*, int, size_t) __RENAME(memrchr);

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE
void* memcpy(void* const dst __pass_object_size0, const void* src, size_t copy_amount)
        __overloadable {
    size_t bos_dst = __bos0(dst);
    if (__bos_trivially_ge(bos_dst, copy_amount)) {
        return __builtin_memcpy(dst, src, copy_amount);
    }
    return __builtin___memcpy_chk(dst, src, copy_amount, bos_dst);
}

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE
void* memmove(void* const dst __pass_object_size0, const void* src, size_t len) __overloadable {
    size_t bos_dst = __bos0(dst);
    if (__bos_trivially_ge(bos_dst, len)) {
        return __builtin_memmove(dst, src, len);
    }
    return __builtin___memmove_chk(dst, src, len, bos_dst);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#if defined(__USE_GNU)
#if __ANDROID_API__ >= __ANDROID_API_R__
__BIONIC_FORTIFY_INLINE
void* mempcpy(void* const dst __pass_object_size0, const void* src, size_t copy_amount)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos0(dst), copy_amount),
                         "'mempcpy' called with size bigger than buffer") {
    size_t bos_dst = __bos0(dst);
    if (__bos_trivially_ge(bos_dst, copy_amount)) {
        return __builtin_mempcpy(dst, src, copy_amount);
    }
    return __builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_R__ */
#endif /* __USE_GNU */

__BIONIC_FORTIFY_INLINE
char* stpcpy(char* const dst __pass_object_size, const char* src)
        __overloadable
        __clang_error_if(__bos_unevaluated_le(__bos(dst), __builtin_strlen(src)),
                         "'stpcpy' called with string bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_L__
    size_t bos_dst = __bos(dst);
    if (!__bos_trivially_gt(bos_dst, __builtin_strlen(src))) {
        return __builtin___stpcpy_chk(dst, src, bos_dst);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_L__ */
    return __builtin_stpcpy(dst, src);
}

__BIONIC_FORTIFY_INLINE
char* strcpy(char* const dst __pass_object_size, const char* src)
        __overloadable
        __clang_error_if(__bos_unevaluated_le(__bos(dst), __builtin_strlen(src)),
                         "'strcpy' called with string bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos_dst = __bos(dst);
    if (!__bos_trivially_gt(bos_dst, __builtin_strlen(src))) {
        return __builtin___strcpy_chk(dst, src, bos_dst);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    return __builtin_strcpy(dst, src);
}

__BIONIC_FORTIFY_INLINE
char* strcat(char* const dst __pass_object_size, const char* src)
        __overloadable
        __clang_error_if(__bos_unevaluated_le(__bos(dst), __builtin_strlen(src)),
                         "'strcat' called with string bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    return __builtin___strcat_chk(dst, src, __bos(dst));
#else
    return __builtin_strcat(dst, src);
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
}

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE
char* strncat(char* const dst __pass_object_size, const char* src, size_t n) __overloadable {
    return __builtin___strncat_chk(dst, src, n, __bos(dst));
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE
void* memset(void* const s __pass_object_size0, int c, size_t n) __overloadable
        /* If you're a user who wants this warning to go away: use `(&memset)(foo, bar, baz)`. */
        __clang_warning_if(c && !n, "'memset' will set 0 bytes; maybe the arguments got flipped?") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos = __bos0(s);
    if (!__bos_trivially_ge(bos, n)) {
        return __builtin___memset_chk(s, c, n, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    return __builtin_memset(s, c, n);
}

#if __ANDROID_API__ >= __ANDROID_API_M__
__BIONIC_FORTIFY_INLINE
void* memchr(const void* const s __pass_object_size, int c, size_t n) __overloadable {
    size_t bos = __bos(s);

    if (__bos_trivially_ge(bos, n)) {
        return __builtin_memchr(s, c, n);
    }

    return __memchr_chk(s, c, n, bos);
}

__BIONIC_FORTIFY_INLINE
void* __memrchr_fortify(const void* const __pass_object_size s, int c, size_t n) __overloadable {
    size_t bos = __bos(s);

    if (__bos_trivially_ge(bos, n)) {
        return __memrchr_real(s, c, n);
    }

    return __memrchr_chk(s, c, n, bos);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_M__ */

#if __ANDROID_API__ >= __ANDROID_API_L__
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE
char* stpncpy(char* const dst __pass_object_size, const char* const src __pass_object_size, size_t n)
        __overloadable {
    size_t bos_dst = __bos(dst);
    size_t bos_src = __bos(src);

    /* Ignore dst size checks; they're handled in strncpy_chk */
    if (bos_src == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin___stpncpy_chk(dst, src, n, bos_dst);
    }

    return __stpncpy_chk2(dst, src, n, bos_dst, bos_src);
}

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE
char* strncpy(char* const dst __pass_object_size, const char* const src __pass_object_size, size_t n)
        __overloadable {
    size_t bos_dst = __bos(dst);
    size_t bos_src = __bos(src);

    /* Ignore dst size checks; they're handled in strncpy_chk */
    if (bos_src == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin___strncpy_chk(dst, src, n, bos_dst);
    }

    return __strncpy_chk2(dst, src, n, bos_dst, bos_src);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_L__ */

__BIONIC_FORTIFY_INLINE
size_t strlcpy(char* const dst __pass_object_size, const char* src, size_t size)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos(dst), size),
                         "'strlcpy' called with size bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos = __bos(dst);

    if (bos != __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strlcpy_chk(dst, src, size, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    return __call_bypassing_fortify(strlcpy)(dst, src, size);
}

__BIONIC_FORTIFY_INLINE
size_t strlcat(char* const dst __pass_object_size, const char* src, size_t size)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos(dst), size),
                         "'strlcat' called with size bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos = __bos(dst);

    if (bos != __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strlcat_chk(dst, src, size, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    return __call_bypassing_fortify(strlcat)(dst, src, size);
}

__BIONIC_FORTIFY_INLINE
size_t strlen(const char* const s __pass_object_size0) __overloadable {
    size_t bos = __bos0(s);

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    if (!__bos_trivially_gt(bos, __builtin_strlen(s))) {
        return __strlen_chk(s, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    return __builtin_strlen(s);
}

__BIONIC_FORTIFY_INLINE
char* strchr(const char* const s __pass_object_size, int c) __overloadable {
#if  __ANDROID_API__ >= __ANDROID_API_J_MR2__
    size_t bos = __bos(s);

    if (bos != __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strchr_chk(s, c, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR2__ */
    return __builtin_strchr(s, c);
}

__BIONIC_FORTIFY_INLINE
char* strrchr(const char* const s __pass_object_size, int c) __overloadable {
#if  __ANDROID_API__ >= __ANDROID_API_J_MR2__
    size_t bos = __bos(s);

    if (bos != __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strrchr_chk(s, c, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR2__ */
    return __builtin_strrchr(s, c);
}

#if __ANDROID_API__ >= __ANDROID_API_M__
#if defined(__cplusplus)
extern "C++" {
__BIONIC_FORTIFY_INLINE
void* memrchr(void* const __pass_object_size s, int c, size_t n) {
    return __memrchr_fortify(s, c, n);
}

__BIONIC_FORTIFY_INLINE
const void* memrchr(const void* const __pass_object_size s, int c, size_t n) {
    return __memrchr_fortify(s, c, n);
}
}
#else
__BIONIC_FORTIFY_INLINE
void* memrchr(const void* const __pass_object_size s, int c, size_t n) __overloadable {
    return __memrchr_fortify(s, c, n);
}
#endif
#endif /* __ANDROID_API__ >= __ANDROID_API_M__ */

#endif /* defined(__BIONIC_FORTIFY) */
