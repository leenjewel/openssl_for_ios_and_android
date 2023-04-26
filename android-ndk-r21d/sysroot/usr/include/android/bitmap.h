/*
 * Copyright (C) 2009 The Android Open Source Project
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

/**
 * @addtogroup Bitmap
 * @{
 */

/**
 * @file bitmap.h
 */

#ifndef ANDROID_BITMAP_H
#define ANDROID_BITMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** AndroidBitmap functions result code. */
enum {
    /** Operation was successful. */
    ANDROID_BITMAP_RESULT_SUCCESS           = 0,
    /** Bad parameter. */
    ANDROID_BITMAP_RESULT_BAD_PARAMETER     = -1,
    /** JNI exception occured. */
    ANDROID_BITMAP_RESULT_JNI_EXCEPTION     = -2,
    /** Allocation failed. */
    ANDROID_BITMAP_RESULT_ALLOCATION_FAILED = -3,
};

/** Backward compatibility: this macro used to be misspelled. */
#define ANDROID_BITMAP_RESUT_SUCCESS ANDROID_BITMAP_RESULT_SUCCESS

/** Bitmap pixel format. */
enum AndroidBitmapFormat {
    /** No format. */
    ANDROID_BITMAP_FORMAT_NONE      = 0,
    /** Red: 8 bits, Green: 8 bits, Blue: 8 bits, Alpha: 8 bits. **/
    ANDROID_BITMAP_FORMAT_RGBA_8888 = 1,
    /** Red: 5 bits, Green: 6 bits, Blue: 5 bits. **/
    ANDROID_BITMAP_FORMAT_RGB_565   = 4,
    /** Deprecated in API level 13. Because of the poor quality of this configuration, it is advised to use ARGB_8888 instead. **/
    ANDROID_BITMAP_FORMAT_RGBA_4444 = 7,
    /** Alpha: 8 bits. */
    ANDROID_BITMAP_FORMAT_A_8       = 8,
    /** Each component is stored as a half float. **/
    ANDROID_BITMAP_FORMAT_RGBA_F16  = 9,
};

/** Bitmap alpha format */
enum {
    /** Pixel components are premultiplied by alpha. */
    ANDROID_BITMAP_FLAGS_ALPHA_PREMUL   = 0,
    /** Pixels are opaque. */
    ANDROID_BITMAP_FLAGS_ALPHA_OPAQUE   = 1,
    /** Pixel components are independent of alpha. */
    ANDROID_BITMAP_FLAGS_ALPHA_UNPREMUL = 2,
    /** Bit mask for AndroidBitmapFormat.flags to isolate the alpha. */
    ANDROID_BITMAP_FLAGS_ALPHA_MASK     = 0x3,
    /** Shift for AndroidBitmapFormat.flags to isolate the alpha. */
    ANDROID_BITMAP_FLAGS_ALPHA_SHIFT    = 0,
};

enum {
    /** If this bit is set in AndroidBitmapInfo.flags, the Bitmap uses the
      * HARDWARE Config, and its {@link AHardwareBuffer} can be retrieved via
      * {@link AndroidBitmap_getHardwareBuffer}.
      */
    ANDROID_BITMAP_FLAGS_IS_HARDWARE = 1 << 31,
};

/** Bitmap info, see AndroidBitmap_getInfo(). */
typedef struct {
    /** The bitmap width in pixels. */
    uint32_t    width;
    /** The bitmap height in pixels. */
    uint32_t    height;
    /** The number of byte per row. */
    uint32_t    stride;
    /** The bitmap pixel format. See {@link AndroidBitmapFormat} */
    int32_t     format;
    /** Bitfield containing information about the bitmap.
     *
     * <p>Two bits are used to encode alpha. Use {@link ANDROID_BITMAP_FLAGS_ALPHA_MASK}
     * and {@link ANDROID_BITMAP_FLAGS_ALPHA_SHIFT} to retrieve them.</p>
     *
     * <p>One bit is used to encode whether the Bitmap uses the HARDWARE Config. Use
     * {@link ANDROID_BITMAP_FLAGS_IS_HARDWARE} to know.</p>
     *
     * <p>These flags were introduced in API level 30.</p>
     */
    uint32_t    flags;
} AndroidBitmapInfo;

/**
 * Given a java bitmap object, fill out the {@link AndroidBitmapInfo} struct for it.
 * If the call fails, the info parameter will be ignored.
 */
int AndroidBitmap_getInfo(JNIEnv* env, jobject jbitmap,
                          AndroidBitmapInfo* info);

#if __ANDROID_API__ >= 30

/**
 * Given a java bitmap object, return its {@link ADataSpace}.
 *
 * Note that {@link ADataSpace} only exposes a few values. This may return
 * {@link ADATASPACE_UNKNOWN}, even for Named ColorSpaces, if they have no
 * corresponding ADataSpace.
 */
int32_t AndroidBitmap_getDataSpace(JNIEnv* env, jobject jbitmap)  __INTRODUCED_IN(30);

#endif // __ANDROID_API__ >= 30

/**
 * Given a java bitmap object, attempt to lock the pixel address.
 * Locking will ensure that the memory for the pixels will not move
 * until the unlockPixels call, and ensure that, if the pixels had been
 * previously purged, they will have been restored.
 *
 * If this call succeeds, it must be balanced by a call to
 * AndroidBitmap_unlockPixels, after which time the address of the pixels should
 * no longer be used.
 *
 * If this succeeds, *addrPtr will be set to the pixel address. If the call
 * fails, addrPtr will be ignored.
 */
int AndroidBitmap_lockPixels(JNIEnv* env, jobject jbitmap, void** addrPtr);

/**
 * Call this to balance a successful call to AndroidBitmap_lockPixels.
 */
int AndroidBitmap_unlockPixels(JNIEnv* env, jobject jbitmap);

#if __ANDROID_API__ >= 30

// Note: these values match android.graphics.Bitmap#compressFormat.

/**
 *  Specifies the formats that can be compressed to with
 *  {@link AndroidBitmap_compress}.
 */
enum AndroidBitmapCompressFormat {
    /**
     * Compress to the JPEG format. quality of 0 means
     * compress for the smallest size. 100 means compress for max
     * visual quality.
     */
    ANDROID_BITMAP_COMPRESS_FORMAT_JPEG = 0,
    /**
     * Compress to the PNG format. PNG is lossless, so quality is
     * ignored.
     */
    ANDROID_BITMAP_COMPRESS_FORMAT_PNG = 1,
    /**
     * Compress to the WEBP lossy format. quality of 0 means
     * compress for the smallest size. 100 means compress for max
     * visual quality.
     */
    ANDROID_BITMAP_COMPRESS_FORMAT_WEBP_LOSSY = 3,
    /**
     * Compress to the WEBP lossless format. quality refers to how
     * much effort to put into compression. A value of 0 means to
     * compress quickly, resulting in a relatively large file size.
     * 100 means to spend more time compressing, resulting in a
     * smaller file.
     */
    ANDROID_BITMAP_COMPRESS_FORMAT_WEBP_LOSSLESS = 4,
};

/**
 *  User-defined function for writing the output of compression.
 *
 *  @param userContext Pointer to user-defined data passed to
 *         {@link AndroidBitmap_compress}.
 *  @param data Compressed data of |size| bytes to write.
 *  @param size Length in bytes of data to write.
 *  @return Whether the operation succeeded.
 */
typedef bool (*AndroidBitmap_CompressWriteFunc)(void* userContext,
                                                const void* data,
                                                size_t size) __INTRODUCED_IN(30);

/**
 *  Compress |pixels| as described by |info|.
 *
 *  @param info Description of the pixels to compress.
 *  @param dataspace {@link ADataSpace} describing the color space of the
 *                   pixels.
 *  @param pixels Pointer to pixels to compress.
 *  @param format {@link AndroidBitmapCompressFormat} to compress to.
 *  @param quality Hint to the compressor, 0-100. The value is interpreted
 *                 differently depending on the
 *                 {@link AndroidBitmapCompressFormat}.
 *  @param userContext User-defined data which will be passed to the supplied
 *                     {@link AndroidBitmap_CompressWriteFunc} each time it is
 *                     called. May be null.
 *  @param fn Function that writes the compressed data. Will be called each time
 *            the compressor has compressed more data that is ready to be
 *            written. May be called more than once for each call to this method.
 *            May not be null.
 *  @return AndroidBitmap functions result code.
 */
int AndroidBitmap_compress(const AndroidBitmapInfo* info,
                           int32_t dataspace,
                           const void* pixels,
                           int32_t format, int32_t quality,
                           void* userContext,
                           AndroidBitmap_CompressWriteFunc fn) __INTRODUCED_IN(30);

struct AHardwareBuffer;
typedef struct AHardwareBuffer AHardwareBuffer;

/**
 *  Retrieve the native object associated with a HARDWARE Bitmap.
 *
 *  Client must not modify it while a Bitmap is wrapping it.
 *
 *  @param bitmap Handle to an android.graphics.Bitmap.
 *  @param outBuffer On success, is set to a pointer to the
 *         {@link AHardwareBuffer} associated with bitmap. This acquires
 *         a reference on the buffer, and the client must call
 *         {@link AHardwareBuffer_release} when finished with it.
 *  @return AndroidBitmap functions result code.
 *          {@link ANDROID_BITMAP_RESULT_BAD_PARAMETER} if bitmap is not a
 *          HARDWARE Bitmap.
 */
int AndroidBitmap_getHardwareBuffer(JNIEnv* env, jobject bitmap,
        AHardwareBuffer** outBuffer) __INTRODUCED_IN(30);

#endif // __ANDROID_API__ >= 30

#ifdef __cplusplus
}
#endif

#endif

/** @} */
