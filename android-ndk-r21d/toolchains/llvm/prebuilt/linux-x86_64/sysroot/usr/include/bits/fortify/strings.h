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

#if defined(__BIONIC_FORTIFY)

__BIONIC_FORTIFY_INLINE
void __bionic_bcopy(const void *src, void* const dst __pass_object_size0, size_t len)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos0(dst), len),
                         "'bcopy' called with size bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos = __bos0(dst);
    if (!__bos_trivially_ge(bos, len)) {
        __builtin___memmove_chk(dst, src, len, bos);
        return;
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    __builtin_memmove(dst, src, len);
}

__BIONIC_FORTIFY_INLINE
void __bionic_bzero(void* const b __pass_object_size0, size_t len)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos0(b), len),
                         "'bzero' called with size bigger than buffer") {
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
    size_t bos = __bos0(b);
    if (!__bos_trivially_ge(bos, len)) {
        __builtin___memset_chk(b, 0, len, bos);
        return;
    }
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */
    __builtin_memset(b, 0, len);
}

#endif /* defined(__BIONIC_FORTIFY) */
