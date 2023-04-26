/*
 * Copyright (C) 2014 The Android Open Source Project
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
 * @addtogroup Media
 * @{
 */

/**
 * @file NdkMediaFormat.h
 */

/*
 * This file defines an NDK API.
 * Do not remove methods.
 * Do not change method signatures.
 * Do not change the value of constants.
 * Do not change the size of any of the classes defined in here.
 * Do not reference types that are not part of the NDK.
 * Do not #include files that aren't part of the NDK.
 */

#ifndef _NDK_MEDIA_FORMAT_H
#define _NDK_MEDIA_FORMAT_H

#include <sys/cdefs.h>
#include <sys/types.h>

#include "NdkMediaError.h"

__BEGIN_DECLS

struct AMediaFormat;
typedef struct AMediaFormat AMediaFormat;

#if __ANDROID_API__ >= 21

/**
 * Available since API level 21.
 */
AMediaFormat *AMediaFormat_new() __INTRODUCED_IN(21);

/**
 * Available since API level 21.
 */
media_status_t AMediaFormat_delete(AMediaFormat*) __INTRODUCED_IN(21);

/**
 * Human readable representation of the format. The returned string is owned by the format,
 * and remains valid until the next call to toString, or until the format is deleted.
 *
 * Available since API level 21.
 */
const char* AMediaFormat_toString(AMediaFormat*) __INTRODUCED_IN(21);

/**
 * Available since API level 21.
 */
bool AMediaFormat_getInt32(AMediaFormat*, const char *name, int32_t *out) __INTRODUCED_IN(21);
/**
 * Available since API level 21.
 */
bool AMediaFormat_getInt64(AMediaFormat*, const char *name, int64_t *out) __INTRODUCED_IN(21);
/**
 * Available since API level 21.
 */
bool AMediaFormat_getFloat(AMediaFormat*, const char *name, float *out) __INTRODUCED_IN(21);
/**
 * Available since API level 21.
 */
bool AMediaFormat_getSize(AMediaFormat*, const char *name, size_t *out) __INTRODUCED_IN(21);
/**
 * The returned data is owned by the format and remains valid as long as the named entry
 * is part of the format.
 *
 * Available since API level 21.
 */
bool AMediaFormat_getBuffer(AMediaFormat*, const char *name, void** data, size_t *size) __INTRODUCED_IN(21);
/**
 * The returned string is owned by the format, and remains valid until the next call to getString,
 * or until the format is deleted.
 *
 * Available since API level 21.
 */
bool AMediaFormat_getString(AMediaFormat*, const char *name, const char **out) __INTRODUCED_IN(21);


/**
 * Available since API level 21.
 */
void AMediaFormat_setInt32(AMediaFormat*, const char* name, int32_t value) __INTRODUCED_IN(21);
/**
 * Available since API level 21.
 */
void AMediaFormat_setInt64(AMediaFormat*, const char* name, int64_t value) __INTRODUCED_IN(21);
/**
 * Available since API level 21.
 */
void AMediaFormat_setFloat(AMediaFormat*, const char* name, float value) __INTRODUCED_IN(21);
/**
 * The provided string is copied into the format.
 *
 * Available since API level 21.
 */
void AMediaFormat_setString(AMediaFormat*, const char* name, const char* value) __INTRODUCED_IN(21);
/**
 * The provided data is copied into the format.
 *
 * Available since API level 21.
 */
void AMediaFormat_setBuffer(AMediaFormat*, const char* name, const void* data, size_t size) __INTRODUCED_IN(21);


extern const char* AMEDIAFORMAT_KEY_AAC_DRC_ATTENUATION_FACTOR __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AAC_DRC_BOOST_FACTOR __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AAC_DRC_HEAVY_COMPRESSION __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AAC_DRC_TARGET_REFERENCE_LEVEL __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AAC_ENCODED_TARGET_LEVEL __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AAC_MAX_OUTPUT_CHANNEL_COUNT __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AAC_PROFILE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_AAC_SBR_MODE __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_AUDIO_SESSION_ID __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_BITRATE_MODE __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_BIT_RATE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_CAPTURE_RATE __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_CHANNEL_COUNT __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_CHANNEL_MASK __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_COLOR_FORMAT __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_COLOR_RANGE __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_COLOR_STANDARD __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_COLOR_TRANSFER __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_COMPLEXITY __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_CSD __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_CSD_0 __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_CSD_1 __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_CSD_2 __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_DISPLAY_CROP __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_DISPLAY_HEIGHT __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_DISPLAY_WIDTH __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_DURATION __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_FLAC_COMPRESSION_LEVEL __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_FRAME_RATE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_GRID_COLUMNS __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_GRID_ROWS __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_HDR_STATIC_INFO __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_HEIGHT __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_INTRA_REFRESH_PERIOD __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_IS_ADTS __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_IS_AUTOSELECT __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_IS_DEFAULT __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_IS_FORCED_SUBTITLE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_I_FRAME_INTERVAL __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_LANGUAGE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_LATENCY __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_LEVEL __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_MAX_HEIGHT __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_MAX_INPUT_SIZE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_MAX_WIDTH __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_MIME __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_MPEG_USER_DATA __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_OPERATING_RATE __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_PCM_ENCODING __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_PRIORITY __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_PROFILE __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_PUSH_BLANK_BUFFERS_ON_STOP __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_REPEAT_PREVIOUS_FRAME_AFTER __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_ROTATION __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_SAMPLE_RATE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_SEI __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_SLICE_HEIGHT __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_STRIDE __INTRODUCED_IN(21);
extern const char* AMEDIAFORMAT_KEY_TEMPORAL_LAYER_ID __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_TEMPORAL_LAYERING __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_TILE_HEIGHT __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_TILE_WIDTH __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_TIME_US __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_TRACK_ID __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_TRACK_INDEX __INTRODUCED_IN(28);
extern const char* AMEDIAFORMAT_KEY_WIDTH __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */

#if __ANDROID_API__ >= 28
/**
 * Available since API level 28.
 */
bool AMediaFormat_getDouble(AMediaFormat*, const char *name, double *out) __INTRODUCED_IN(28);
/**
 * Available since API level 28.
 */
bool AMediaFormat_getRect(AMediaFormat*, const char *name,
        int32_t *left, int32_t *top, int32_t *right, int32_t *bottom) __INTRODUCED_IN(28);

/**
 * Available since API level 28.
 */
void AMediaFormat_setDouble(AMediaFormat*, const char* name, double value) __INTRODUCED_IN(28);
/**
 * Available since API level 28.
 */
void AMediaFormat_setSize(AMediaFormat*, const char* name, size_t value) __INTRODUCED_IN(28);
/**
 * Available since API level 28.
 */
void AMediaFormat_setRect(AMediaFormat*, const char* name,
        int32_t left, int32_t top, int32_t right, int32_t bottom) __INTRODUCED_IN(28);
#endif /* __ANDROID_API__ >= 28 */

#if __ANDROID_API__ >= 29
/**
 * Remove all key/value pairs from the given AMediaFormat.
 *
 * Available since API level 29.
 */
void AMediaFormat_clear(AMediaFormat*) __INTRODUCED_IN(29);

/**
 * Copy one AMediaFormat to another.
 *
 * Available since API level 29.
 */
media_status_t AMediaFormat_copy(AMediaFormat *to, AMediaFormat *from) __INTRODUCED_IN(29);

extern const char* AMEDIAFORMAT_KEY_ALBUM __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ALBUMART __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ALBUMARTIST __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ARTIST __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_AUDIO_PRESENTATION_INFO __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_AUDIO_PRESENTATION_PRESENTATION_ID __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_AUDIO_PRESENTATION_PROGRAM_ID __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_AUTHOR __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_BITS_PER_SAMPLE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CDTRACKNUMBER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_COMPILATION __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_COMPOSER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CREATE_INPUT_SURFACE_SUSPENDED __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_DEFAULT_IV_SIZE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_ENCRYPTED_BYTE_BLOCK __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_ENCRYPTED_SIZES __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_IV __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_KEY __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_MODE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_PLAIN_SIZES __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CRYPTO_SKIP_BYTE_BLOCK __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CSD_AVC __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_CSD_HEVC __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_D263 __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_DATE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_DISCNUMBER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ENCODER_DELAY __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ENCODER_PADDING __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ESDS __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_EXIF_OFFSET __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_EXIF_SIZE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_FRAME_COUNT __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_GENRE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_HAPTIC_CHANNEL_COUNT __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_ICC_PROFILE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_IS_SYNC_FRAME __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_LOCATION __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_LOOP __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_LYRICIST __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_MANUFACTURER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_MAX_BIT_RATE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_MAX_FPS_TO_ENCODER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_MAX_PTS_GAP_TO_ENCODER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_MPEG2_STREAM_HEADER __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_PCM_BIG_ENDIAN __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_PSSH __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_SAR_HEIGHT __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_SAR_WIDTH __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_TARGET_TIME __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_TEMPORAL_LAYER_COUNT __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_TEXT_FORMAT_DATA __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_THUMBNAIL_CSD_HEVC __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_THUMBNAIL_HEIGHT __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_THUMBNAIL_TIME __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_THUMBNAIL_WIDTH __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_TITLE __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_VALID_SAMPLES __INTRODUCED_IN(29);
extern const char* AMEDIAFORMAT_KEY_YEAR __INTRODUCED_IN(29);

#endif /* __ANDROID_API__ >= 29 */

#if __ANDROID_API__ >= 30
/**
 * An optional key describing the low latency decoding mode. This is an optional parameter
 * that applies only to decoders. If enabled, the decoder doesn't hold input and output
 * data more than required by the codec standards.
 * The associated value is an integer (0 or 1): 1 when low-latency decoding is enabled,
 * 0 otherwise. The default value is 0.
 *
 * Available since API level 30.
 */
extern const char* AMEDIAFORMAT_KEY_LOW_LATENCY __INTRODUCED_IN(30);
#endif /* __ANDROID_API__ >= 30 */

__END_DECLS

#endif // _NDK_MEDIA_FORMAT_H

/** @} */
