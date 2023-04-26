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

#ifndef _STDIO_H_
#error "Never include this file directly; instead, include <stdio.h>"
#endif


#if __ANDROID_API__ >= 17
char* __fgets_chk(char*, int, FILE*, size_t) __INTRODUCED_IN(17);
#endif /* __ANDROID_API__ >= 17 */


#if __ANDROID_API__ >= 24
size_t __fread_chk(void*, size_t, size_t, FILE*, size_t) __INTRODUCED_IN(24);
size_t __fwrite_chk(const void*, size_t, size_t, FILE*, size_t) __INTRODUCED_IN(24);
#endif /* __ANDROID_API__ >= 24 */


#if defined(__BIONIC_FORTIFY) && !defined(__BIONIC_NO_STDIO_FORTIFY)

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __printflike(3, 0)
int vsnprintf(char* const __pass_object_size dest, size_t size, const char* format, va_list ap)
        __overloadable {
    return __builtin___vsnprintf_chk(dest, size, 0, __bos(dest), format, ap);
}

__BIONIC_FORTIFY_INLINE __printflike(2, 0)
int vsprintf(char* const __pass_object_size dest, const char* format, va_list ap) __overloadable {
    return __builtin___vsprintf_chk(dest, 0, __bos(dest), format, ap);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

__BIONIC_ERROR_FUNCTION_VISIBILITY
int sprintf(char* dest, const char* format)
    __overloadable
    __enable_if(__bos_unevaluated_lt(__bos(dest), __builtin_strlen(format)),
                "format string will always overflow destination buffer")
    __errorattr("format string will always overflow destination buffer");

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
__BIONIC_FORTIFY_VARIADIC __printflike(2, 3)
int sprintf(char* const __pass_object_size dest, const char* format, ...) __overloadable {
    va_list va;
    va_start(va, format);
    int result = __builtin___vsprintf_chk(dest, 0, __bos(dest), format, va);
    va_end(va);
    return result;
}

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_VARIADIC __printflike(3, 4)
int snprintf(char* const __pass_object_size dest, size_t size, const char* format, ...)
        __overloadable {
    va_list va;
    va_start(va, format);
    int result = __builtin___vsnprintf_chk(dest, size, 0, __bos(dest), format, va);
    va_end(va);
    return result;
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#define __bos_trivially_ge_mul(bos_val, size, count) \
  __bos_dynamic_check_impl_and(bos_val, >=, (size) * (count), \
                               !__unsafe_check_mul_overflow(size, count))

__BIONIC_FORTIFY_INLINE
size_t fread(void* const __pass_object_size0 buf, size_t size, size_t count, FILE* stream)
        __overloadable
        __clang_error_if(__unsafe_check_mul_overflow(size, count),
                         "in call to 'fread', size * count overflows")
        __clang_error_if(__bos_unevaluated_lt(__bos0(buf), size * count),
                         "in call to 'fread', size * count is too large for the given buffer") {
#if __ANDROID_API__ >= __ANDROID_API_N__
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_mul(bos, size, count)) {
        return __fread_chk(buf, size, count, stream, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_N__ */
    return __call_bypassing_fortify(fread)(buf, size, count, stream);
}

__BIONIC_FORTIFY_INLINE
size_t fwrite(const void* const __pass_object_size0 buf, size_t size, size_t count, FILE* stream)
        __overloadable
        __clang_error_if(__unsafe_check_mul_overflow(size, count),
                         "in call to 'fwrite', size * count overflows")
        __clang_error_if(__bos_unevaluated_lt(__bos0(buf), size * count),
                         "in call to 'fwrite', size * count is too large for the given buffer") {
#if __ANDROID_API__ >= __ANDROID_API_N__
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_mul(bos, size, count)) {
        return __fwrite_chk(buf, size, count, stream, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_N__ */
    return __call_bypassing_fortify(fwrite)(buf, size, count, stream);
}
#undef __bos_trivially_ge_mul

__BIONIC_FORTIFY_INLINE
char* fgets(char* const __pass_object_size dest, int size, FILE* stream)
        __overloadable
        __clang_error_if(size < 0, "in call to 'fgets', size should not be negative")
        __clang_error_if(__bos_unevaluated_lt(__bos(dest), size),
                         "in call to 'fgets', size is larger than the destination buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos = __bos(dest);

    if (!__bos_dynamic_check_impl_and(bos, >=, (size_t)size, size >= 0)) {
        return __fgets_chk(dest, size, stream, bos);
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    return __call_bypassing_fortify(fgets)(dest, size, stream);
}

#endif /* defined(__BIONIC_FORTIFY) */
