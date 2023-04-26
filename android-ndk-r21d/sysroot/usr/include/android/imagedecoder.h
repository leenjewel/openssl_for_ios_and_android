/*
 * Copyright (C) 2019 The Android Open Source Project
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
 * @defgroup ImageDecoder
 *
 * Functions for converting encoded images into RGBA pixels.
 *
 * Similar to the Java counterpart android.graphics.ImageDecoder, it can be used
 * to decode images in the following formats:
 * - JPEG
 * - PNG
 * - GIF
 * - WebP
 * - BMP
 * - ICO
 * - WBMP
 * - HEIF
 * - Digital negatives (via the DNG SDK)
 * <p>It has similar options for scaling, cropping, and choosing the output format.
 * Unlike the Java API, which can create an android.graphics.Bitmap or
 * android.graphics.drawable.Drawable object, AImageDecoder decodes directly
 * into memory provided by the client. For more information, see the
 * <a href="https://developer.android.com/ndk/guides/image-decoder">Image decoder</a>
 * developer guide.
 * @{
 */

/**
 * @file imagedecoder.h
 * @brief API for decoding images.
 */

#ifndef ANDROID_IMAGE_DECODER_H
#define ANDROID_IMAGE_DECODER_H

#include "bitmap.h"
#include <android/rect.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AAsset;

#if __ANDROID_API__ >= 30

/**
 *  {@link AImageDecoder} functions result code. Many functions will return one of these
 *  to indicate success ({@link ANDROID_IMAGE_DECODER_SUCCESS}) or the reason
 *  for the failure. On failure, any out-parameters should be considered
 *  uninitialized, except where specified.
 */
enum {
    /**
     * Decoding was successful and complete.
     */
    ANDROID_IMAGE_DECODER_SUCCESS = 0,
    /**
     * The input is incomplete.
     */
    ANDROID_IMAGE_DECODER_INCOMPLETE = -1,
    /**
     * The input contained an error after decoding some lines.
     */
    ANDROID_IMAGE_DECODER_ERROR = -2,
    /**
     * Could not convert. For example, attempting to decode an image with
     * alpha to an opaque format.
     */
    ANDROID_IMAGE_DECODER_INVALID_CONVERSION = -3,
    /**
     * The scale is invalid. It may have overflowed, or it may be incompatible
     * with the current alpha setting.
     */
    ANDROID_IMAGE_DECODER_INVALID_SCALE = -4,
    /**
     * Some other parameter is invalid.
     */
    ANDROID_IMAGE_DECODER_BAD_PARAMETER = -5,
    /**
     * Input was invalid before decoding any pixels.
     */
    ANDROID_IMAGE_DECODER_INVALID_INPUT = -6,
    /**
     * A seek was required and it failed.
     */
    ANDROID_IMAGE_DECODER_SEEK_ERROR = -7,
    /**
     * Some other error. For example, an internal allocation failed.
     */
    ANDROID_IMAGE_DECODER_INTERNAL_ERROR = -8,
    /**
     * AImageDecoder did not recognize the format.
     */
    ANDROID_IMAGE_DECODER_UNSUPPORTED_FORMAT = -9
};

struct AImageDecoder;

/**
 * Opaque handle for decoding images.
 *
 * Create using one of the following:
 * - {@link AImageDecoder_createFromAAsset}
 * - {@link AImageDecoder_createFromFd}
 * - {@link AImageDecoder_createFromBuffer}
 *
 * After creation, {@link AImageDecoder_getHeaderInfo} can be used to retrieve
 * information about the encoded image. Other functions, like
 * {@link AImageDecoder_setTargetSize}, can be used to specify how to decode, and
 * {@link AImageDecoder_decode} will decode into client provided memory.
 *
 * {@link AImageDecoder} objects are NOT thread-safe, and should not be shared across
 * threads.
 */
typedef struct AImageDecoder AImageDecoder;

/**
 * Create a new {@link AImageDecoder} from an {@link AAsset}.
 *
 * @param asset {@link AAsset} containing encoded image data. Client is still
 *              responsible for calling {@link AAsset_close} on it, which may be
 *              done after deleting the returned {@link AImageDecoder}.
 * @param outDecoder On success (i.e. return value is
 *                   {@link ANDROID_IMAGE_DECODER_SUCCESS}), this will be set to
 *                   a newly created {@link AImageDecoder}. Caller is
 *                   responsible for calling {@link AImageDecoder_delete} on it.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_INCOMPLETE}: The asset was truncated before
 *   reading the image header.
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: One of the parameters is
 *   null.
 * - {@link ANDROID_IMAGE_DECODER_INVALID_INPUT}: There is an error in the
 *   header.
 * - {@link ANDROID_IMAGE_DECODER_SEEK_ERROR}: The asset failed to seek.
 * - {@link ANDROID_IMAGE_DECODER_INTERNAL_ERROR}: Some other error, like a
 *   failure to allocate memory.
 * - {@link ANDROID_IMAGE_DECODER_UNSUPPORTED_FORMAT}: The format is not
 *   supported.
 */
int AImageDecoder_createFromAAsset(struct AAsset* asset, AImageDecoder** outDecoder)
        __INTRODUCED_IN(30);

/**
 * Create a new {@link AImageDecoder} from a file descriptor.
 *
 * @param fd Seekable, readable, open file descriptor for encoded data.
 *           Client is still responsible for closing it, which may be done
 *           after deleting the returned {@link AImageDecoder}.
 * @param outDecoder On success (i.e. return value is
 *                   {@link ANDROID_IMAGE_DECODER_SUCCESS}), this will be set to
 *                   a newly created {@link AImageDecoder}. Caller is
 *                   responsible for calling {@link AImageDecoder_delete} on it.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_INCOMPLETE}: The file was truncated before
 *   reading the image header.
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The {@link AImageDecoder} is
 *   null, or |fd| does not represent a valid, seekable file descriptor.
 * - {@link ANDROID_IMAGE_DECODER_INVALID_INPUT}: There is an error in the
 *   header.
 * - {@link ANDROID_IMAGE_DECODER_SEEK_ERROR}: The descriptor failed to seek.
 * - {@link ANDROID_IMAGE_DECODER_INTERNAL_ERROR}: Some other error, like a
 *   failure to allocate memory.
 * - {@link ANDROID_IMAGE_DECODER_UNSUPPORTED_FORMAT}: The format is not
 *   supported.
 */
int AImageDecoder_createFromFd(int fd, AImageDecoder** outDecoder) __INTRODUCED_IN(30);

/**
 * Create a new AImageDecoder from a buffer.
 *
 * @param buffer Pointer to encoded data. Must be valid for the entire time
 *               the {@link AImageDecoder} is used.
 * @param length Byte length of buffer.
 * @param outDecoder On success (i.e. return value is
 *                   {@link ANDROID_IMAGE_DECODER_SUCCESS}), this will be set to
 *                   a newly created {@link AImageDecoder}. Caller is
 *                   responsible for calling {@link AImageDecoder_delete} on it.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_INCOMPLETE}: The encoded image was truncated before
 *   reading the image header.
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: One of the parameters is
 *   invalid.
 * - {@link ANDROID_IMAGE_DECODER_INVALID_INPUT}: There is an error in the
 *   header.
 * - {@link ANDROID_IMAGE_DECODER_INTERNAL_ERROR}: Some other error, like a
 *   failure to allocate memory.
 * - {@link ANDROID_IMAGE_DECODER_UNSUPPORTED_FORMAT}: The format is not
 *   supported.
 */
int AImageDecoder_createFromBuffer(const void* buffer, size_t length,
                                   AImageDecoder** outDecoder) __INTRODUCED_IN(30);

/**
 * Delete the AImageDecoder.
 */
void AImageDecoder_delete(AImageDecoder* decoder) __INTRODUCED_IN(30);

/**
 * Choose the desired output format.
 *
 * @param format {@link AndroidBitmapFormat} to use for the output.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure. On failure, the
 *         {@link AImageDecoder} uses the format it was already planning
 *         to use (either its default or a previously successful setting
 *         from this function).
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The
 *   {@link AImageDecoder} is null or |format| does not correspond to an
 *   {@link AndroidBitmapFormat}.
 * - {@link ANDROID_IMAGE_DECODER_INVALID_CONVERSION}: The
 *   {@link AndroidBitmapFormat} is incompatible with the image.
 */
int AImageDecoder_setAndroidBitmapFormat(AImageDecoder*,
        int32_t format) __INTRODUCED_IN(30);

/**
 * Specify whether the output's pixels should be unpremultiplied.
 *
 * By default, {@link AImageDecoder_decodeImage} will premultiply the pixels, if they have alpha.
 * Pass true to this method to leave them unpremultiplied. This has no effect on an
 * opaque image.
 *
 * @param unpremultipliedRequired Pass true to leave the pixels unpremultiplied.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_INVALID_CONVERSION}: Unpremultiplied is not
 *   possible due to an existing scale set by
 *   {@link AImageDecoder_setTargetSize}.
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The
 *   {@link AImageDecoder} is null.
 */
int AImageDecoder_setUnpremultipliedRequired(AImageDecoder*,
                                             bool unpremultipliedRequired) __INTRODUCED_IN(30);

/**
 * Choose the dataspace for the output.
 *
 * Ignored by {@link ANDROID_BITMAP_FORMAT_A_8}, which does not support
 * an {@link ADataSpace}.
 *
 * @param dataspace The {@link ADataSpace} to decode into. An ADataSpace
 *                  specifies how to interpret the colors. By default,
 *                  AImageDecoder will decode into the ADataSpace specified by
 *                  {@link AImageDecoderHeaderInfo_getDataSpace}. If this
 *                  parameter is set to a different ADataSpace, AImageDecoder
 *                  will transform the output into the specified ADataSpace.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The
 *   {@link AImageDecoder} is null or |dataspace| does not correspond to an
 *   {@link ADataSpace} value.
 */
int AImageDecoder_setDataSpace(AImageDecoder*, int32_t dataspace) __INTRODUCED_IN(30);

/**
 * Specify the output size for a decoded image.
 *
 * Future calls to {@link AImageDecoder_decodeImage} will sample or scale the
 * encoded image to reach the desired size. If a crop rect is set (via
 * {@link AImageDecoder_setCrop}), it must be contained within the dimensions
 * specified by width and height, and the output image will be the size of the
 * crop rect.
 *
 * @param width Width of the output (prior to cropping).
 *              This will affect future calls to
 *              {@link AImageDecoder_getMinimumStride}, which will now return
 *              a value based on this width.
 * @param height Height of the output (prior to cropping).
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The
 *   {@link AImageDecoder} is null.
 * - {@link ANDROID_IMAGE_DECODER_INVALID_SCALE}: |width| or |height| is <= 0,
 *   the size is too big, any existing crop is not contained by the new image dimensions,
 *   or the scale is incompatible with a previous call to
 *   {@link AImageDecoder_setUnpremultipliedRequired}(true).
 */
int AImageDecoder_setTargetSize(AImageDecoder*, int32_t width, int32_t height) __INTRODUCED_IN(30);


/**
 * Compute the dimensions to use for a given sampleSize.
 *
 * Although AImageDecoder can scale to an arbitrary target size (see
 * {@link AImageDecoder_setTargetSize}), some sizes may be more efficient than
 * others. This computes the most efficient target size to use to reach a
 * particular sampleSize.
 *
 * @param sampleSize A subsampling rate of the original image. Must be greater
 *                   than or equal to 1. A sampleSize of 2 means to skip every
 *                   other pixel/line, resulting in a width and height that are
 *                   1/2 of the original dimensions, with 1/4 the number of
 *                   pixels.
 * @param width Out parameter for the width sampled by sampleSize, and rounded
 *              in the direction that the decoder can do most efficiently.
 * @param height Out parameter for the height sampled by sampleSize, and rounded
 *               in the direction that the decoder can do most efficiently.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The
 *   {@link AImageDecoder}, |width| or |height| is null or |sampleSize| is < 1.
 */
int AImageDecoder_computeSampledSize(const AImageDecoder*, int sampleSize,
                                     int32_t* width, int32_t* height) __INTRODUCED_IN(30);
/**
 * Specify how to crop the output after scaling (if any).
 *
 * Future calls to {@link AImageDecoder_decodeImage} will crop their output to
 * the specified {@link ARect}. Clients will only need to allocate enough memory
 * for the cropped ARect.
 *
 * @param crop Rectangle describing a crop of the decode. It must be contained inside of
 *             the (possibly scaled, by {@link AImageDecoder_setTargetSize})
 *             image dimensions. This will affect future calls to
 *             {@link AImageDecoder_getMinimumStride}, which will now return a
 *             value based on the width of the crop. An empty ARect -
 *             specifically { 0, 0, 0, 0 } - may be used to remove the cropping
 *             behavior. Any other empty or unsorted ARects will result in
 *             returning {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The
 *   {@link AImageDecoder} is null or the crop is not contained by the
 *   (possibly scaled) image dimensions.
 */
int AImageDecoder_setCrop(AImageDecoder*, ARect crop) __INTRODUCED_IN(30);

struct AImageDecoderHeaderInfo;
/**
 * Opaque handle for representing information about the encoded image. Retrieved
 * using {@link AImageDecoder_getHeaderInfo} and passed to methods like
 * {@link AImageDecoderHeaderInfo_getWidth} and
 * {@link AImageDecoderHeaderInfo_getHeight}.
 */
typedef struct AImageDecoderHeaderInfo AImageDecoderHeaderInfo;

/**
 * Return an opaque handle for reading header info.
 *
 * This is owned by the {@link AImageDecoder} and will be destroyed when the
 * AImageDecoder is destroyed via {@link AImageDecoder_delete}.
 */
const AImageDecoderHeaderInfo* AImageDecoder_getHeaderInfo(
        const AImageDecoder*) __INTRODUCED_IN(30);

/**
 * Report the native width of the encoded image. This is also the logical
 * pixel width of the output, unless {@link AImageDecoder_setTargetSize} is
 * used to choose a different size or {@link AImageDecoder_setCrop} is used to
 * set a crop rect.
 */
int32_t AImageDecoderHeaderInfo_getWidth(const AImageDecoderHeaderInfo*) __INTRODUCED_IN(30);

/**
 * Report the native height of the encoded image. This is also the logical
 * pixel height of the output, unless {@link AImageDecoder_setTargetSize} is
 * used to choose a different size or {@link AImageDecoder_setCrop} is used to
 * set a crop rect.
 */
int32_t AImageDecoderHeaderInfo_getHeight(const AImageDecoderHeaderInfo*) __INTRODUCED_IN(30);

/**
 * Report the mimeType of the encoded image.
 *
 * @return a string literal describing the mime type.
 */
const char* AImageDecoderHeaderInfo_getMimeType(
        const AImageDecoderHeaderInfo*) __INTRODUCED_IN(30);

/**
 * Report the {@link AndroidBitmapFormat} the AImageDecoder will decode to
 * by default. {@link AImageDecoder} will try to choose one that is sensible
 * for the image and the system. Note that this does not indicate the
 * encoded format of the image.
 */
int32_t AImageDecoderHeaderInfo_getAndroidBitmapFormat(
        const AImageDecoderHeaderInfo*) __INTRODUCED_IN(30);

/**
 * Report how the {@link AImageDecoder} will handle alpha by default. If the image
 * contains no alpha (according to its header), this will return
 * {@link ANDROID_BITMAP_FLAGS_ALPHA_OPAQUE}. If the image may contain alpha,
 * this returns {@link ANDROID_BITMAP_FLAGS_ALPHA_PREMUL}, because
 * {@link AImageDecoder_decodeImage} will premultiply pixels by default.
 */
int AImageDecoderHeaderInfo_getAlphaFlags(
        const AImageDecoderHeaderInfo*) __INTRODUCED_IN(30);

/**
 * Report the dataspace the AImageDecoder will decode to by default.
 *
 * By default, {@link AImageDecoder_decodeImage} will not do any color
 * conversion.
 *
 * @return The {@link ADataSpace} representing the way the colors
 *         are encoded (or {@link ADATASPACE_UNKNOWN} if there is not a
 *         corresponding ADataSpace). This specifies how to interpret the colors
 *         in the decoded image, unless {@link AImageDecoder_setDataSpace} is
 *         called to decode to a different ADataSpace.
 *
 *         Note that ADataSpace only exposes a few values. This may return
 *         {@link ADATASPACE_UNKNOWN}, even for Named ColorSpaces, if they have
 *         no corresponding {@link ADataSpace}.
 */
int32_t AImageDecoderHeaderInfo_getDataSpace(
        const AImageDecoderHeaderInfo*) __INTRODUCED_IN(30);

/**
 * Return the minimum stride that can be used in
 * {@link AImageDecoder_decodeImage).
 *
 * This stride provides no padding, meaning it will be exactly equal to the
 * width times the number of bytes per pixel for the {@link AndroidBitmapFormat}
 * being used.
 *
 * If the output is scaled (via {@link AImageDecoder_setTargetSize}) and/or
 * cropped (via {@link AImageDecoder_setCrop}), this takes those into account.
 */
size_t AImageDecoder_getMinimumStride(AImageDecoder*) __INTRODUCED_IN(30);

/**
 * Decode the image into pixels, using the settings of the {@link AImageDecoder}.
 *
 * @param decoder Opaque object representing the decoder.
 * @param pixels On success, will be filled with the result
 *               of the decode. Must be large enough to hold |size| bytes.
 * @param stride Width in bytes of a single row. Must be at least
 *               {@link AImageDecoder_getMinimumStride} and a multiple of the
 *               bytes per pixel of the {@link AndroidBitmapFormat}.
 * @param size Size of the pixel buffer in bytes. Must be at least
 *             stride * (height - 1) +
 *             {@link AImageDecoder_getMinimumStride}.
 * @return {@link ANDROID_IMAGE_DECODER_SUCCESS} on success or a value
 *         indicating the reason for the failure.
 *
 * Errors:
 * - {@link ANDROID_IMAGE_DECODER_INCOMPLETE}: The image was truncated. A
 *   partial image was decoded, and undecoded lines have been initialized to all
 *   zeroes.
 * - {@link ANDROID_IMAGE_DECODER_ERROR}: The image contained an error. A
 *   partial image was decoded, and undecoded lines have been initialized to all
 *   zeroes.
 * - {@link ANDROID_IMAGE_DECODER_BAD_PARAMETER}: The {@link AImageDecoder} or
 *   |pixels| is null, the stride is not large enough or not pixel aligned, or
 *   |size| is not large enough.
 * - {@link ANDROID_IMAGE_DECODER_SEEK_ERROR}: The asset or file descriptor
 *   failed to seek.
 * - {@link ANDROID_IMAGE_DECODER_INTERNAL_ERROR}: Some other error, like a
 *   failure to allocate memory.
 */
int AImageDecoder_decodeImage(AImageDecoder* decoder,
                              void* pixels, size_t stride,
                              size_t size) __INTRODUCED_IN(30);

#endif // __ANDROID_API__ >= 30

#ifdef __cplusplus
}
#endif

#endif // ANDROID_IMAGE_DECODER_H

/** @} */
