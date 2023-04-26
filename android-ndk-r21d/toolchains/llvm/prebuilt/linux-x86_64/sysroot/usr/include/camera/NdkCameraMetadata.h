/*
 * Copyright (C) 2015 The Android Open Source Project
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
 * @addtogroup Camera
 * @{
 */

/**
 * @file NdkCameraMetadata.h
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

#ifndef _NDK_CAMERA_METADATA_H
#define _NDK_CAMERA_METADATA_H

#include <stdint.h>
#include <sys/cdefs.h>

#ifndef __ANDROID_VNDK__
#if __ANDROID_API__ >= 30
#include "jni.h"
#endif  /* __ANDROID_API__ >= 30 */
#endif  /* __ANDROID_VNDK__ */

#include "NdkCameraError.h"
#include "NdkCameraMetadataTags.h"

__BEGIN_DECLS

#if __ANDROID_API__ >= 24

/**
 * ACameraMetadata is opaque type that provides access to read-only camera metadata like camera
 * characteristics (via {@link ACameraManager_getCameraCharacteristics}) or capture results (via
 * {@link ACameraCaptureSession_captureCallback_result}).
 */
typedef struct ACameraMetadata ACameraMetadata;

/**
 * Possible data types of a metadata entry.
 *
 * Keep in sync with system/media/include/system/camera_metadata.h
 */
enum {
    /// Unsigned 8-bit integer (uint8_t)
    ACAMERA_TYPE_BYTE = 0,
    /// Signed 32-bit integer (int32_t)
    ACAMERA_TYPE_INT32 = 1,
    /// 32-bit float (float)
    ACAMERA_TYPE_FLOAT = 2,
    /// Signed 64-bit integer (int64_t)
    ACAMERA_TYPE_INT64 = 3,
    /// 64-bit float (double)
    ACAMERA_TYPE_DOUBLE = 4,
    /// A 64-bit fraction (ACameraMetadata_rational)
    ACAMERA_TYPE_RATIONAL = 5,
    /// Number of type fields
    ACAMERA_NUM_TYPES
};

/**
 * Definition of rational data type in {@link ACameraMetadata}.
 */
typedef struct ACameraMetadata_rational {
    int32_t numerator;
    int32_t denominator;
} ACameraMetadata_rational;

/**
 * A single camera metadata entry.
 *
 * <p>Each entry is an array of values, though many metadata fields may only have 1 entry in the
 * array.</p>
 */
typedef struct ACameraMetadata_entry {
    /**
     * The tag identifying the entry.
     *
     * <p> It is one of the values defined in {@link NdkCameraMetadataTags.h}, and defines how the
     * entry should be interpreted and which parts of the API provide it.
     * See {@link NdkCameraMetadataTags.h} for more details. </p>
     */
    uint32_t tag;

    /**
     * The data type of this metadata entry.
     *
     * <p>Must be one of ACAMERA_TYPE_* enum values defined above. A particular tag always has the
     * same type.</p>
     */
    uint8_t  type;

    /**
     * Count of elements (NOT count of bytes) in this metadata entry.
     */
    uint32_t count;

    /**
     * Pointer to the data held in this metadata entry.
     *
     * <p>The type field above defines which union member pointer is valid. The count field above
     * defines the length of the data in number of elements.</p>
     */
    union {
        uint8_t *u8;
        int32_t *i32;
        float   *f;
        int64_t *i64;
        double  *d;
        ACameraMetadata_rational* r;
    } data;
} ACameraMetadata_entry;

/**
 * A single read-only camera metadata entry.
 *
 * <p>Each entry is an array of values, though many metadata fields may only have 1 entry in the
 * array.</p>
 */
typedef struct ACameraMetadata_const_entry {
    /**
     * The tag identifying the entry.
     *
     * <p> It is one of the values defined in {@link NdkCameraMetadataTags.h}, and defines how the
     * entry should be interpreted and which parts of the API provide it.
     * See {@link NdkCameraMetadataTags.h} for more details. </p>
     */
    uint32_t tag;

    /**
     * The data type of this metadata entry.
     *
     * <p>Must be one of ACAMERA_TYPE_* enum values defined above. A particular tag always has the
     * same type.</p>
     */
    uint8_t  type;

    /**
     * Count of elements (NOT count of bytes) in this metadata entry.
     */
    uint32_t count;

    /**
     * Pointer to the data held in this metadata entry.
     *
     * <p>The type field above defines which union member pointer is valid. The count field above
     * defines the length of the data in number of elements.</p>
     */
    union {
        const uint8_t *u8;
        const int32_t *i32;
        const float   *f;
        const int64_t *i64;
        const double  *d;
        const ACameraMetadata_rational* r;
    } data;
} ACameraMetadata_const_entry;

/**
 * Get a metadata entry from an input {@link ACameraMetadata}.
 *
 * <p>The memory of the data field in the returned entry is managed by camera framework. Do not
 * attempt to free it.</p>
 *
 * @param metadata the {@link ACameraMetadata} of interest.
 * @param tag the tag value of the camera metadata entry to be get.
 * @param entry the output {@link ACameraMetadata_const_entry} will be filled here if the method
 *        call succeeeds.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if metadata or entry is NULL.</li>
 *         <li>{@link ACAMERA_ERROR_METADATA_NOT_FOUND} if input metadata does not contain an entry
 *             of input tag value.</li></ul>
 */
camera_status_t ACameraMetadata_getConstEntry(
        const ACameraMetadata* metadata,
        uint32_t tag, /*out*/ACameraMetadata_const_entry* entry) __INTRODUCED_IN(24);

/**
 * List all the entry tags in input {@link ACameraMetadata}.
 *
 * @param metadata the {@link ACameraMetadata} of interest.
 * @param numEntries number of metadata entries in input {@link ACameraMetadata}
 * @param tags the tag values of the metadata entries. Length of tags is returned in numEntries
 *             argument. The memory is managed by ACameraMetadata itself and must NOT be free/delete
 *             by application. Do NOT access tags after calling ACameraMetadata_free.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if metadata, numEntries or tags is NULL.</li>
 *         <li>{@link ACAMERA_ERROR_UNKNOWN} if the method fails for some other reasons.</li></ul>
 */
camera_status_t ACameraMetadata_getAllTags(
        const ACameraMetadata* metadata,
        /*out*/int32_t* numEntries, /*out*/const uint32_t** tags) __INTRODUCED_IN(24);

/**
 * Create a copy of input {@link ACameraMetadata}.
 *
 * <p>The returned ACameraMetadata must be freed by the application by {@link ACameraMetadata_free}
 * after application is done using it.</p>
 *
 * @param src the input {@link ACameraMetadata} to be copied.
 *
 * @return a valid ACameraMetadata pointer or NULL if the input metadata cannot be copied.
 */
ACameraMetadata* ACameraMetadata_copy(const ACameraMetadata* src) __INTRODUCED_IN(24);

/**
 * Free a {@link ACameraMetadata} structure.
 *
 * @param metadata the {@link ACameraMetadata} to be freed.
 */
void ACameraMetadata_free(ACameraMetadata* metadata) __INTRODUCED_IN(24);

#endif /* __ANDROID_API__ >= 24 */

#if __ANDROID_API__ >= 29

/**
 * Helper function to check if a camera is logical multi-camera.
 *
 * <p> Check whether a camera device is a logical multi-camera based on its
 * static metadata. If it is, also returns its physical sub camera Ids.</p>
 *
 * @param staticMetadata the static metadata of the camera being checked.
 * @param numPhysicalCameras returns the number of physical cameras.
 * @param physicalCameraIds returns the array of physical camera Ids backing this logical
 *                          camera device. Note that this pointer is only valid
 *                          during the lifetime of the staticMetadata object.
 *
 * @return true if this is a logical multi-camera, false otherwise.
 */
bool ACameraMetadata_isLogicalMultiCamera(const ACameraMetadata* staticMetadata,
        /*out*/size_t* numPhysicalCameras, /*out*/const char* const** physicalCameraIds)
        __INTRODUCED_IN(29);

#endif /* __ANDROID_API__ >= 29 */

#ifndef __ANDROID_VNDK__
#if __ANDROID_API__ >= 30

/**
 * Return a {@link ACameraMetadata} that references the same data as
 * {@link cameraMetadata}, which is an instance of
 * {@link android.hardware.camera2.CameraMetadata} (e.g., a
 * {@link android.hardware.camera2.CameraCharacteristics} or
 * {@link android.hardware.camera2.CaptureResult}).
 *
 * <p>The returned ACameraMetadata must be freed by the application by {@link ACameraMetadata_free}
 * after application is done using it.</p>
 *
 * <p>The ACameraMetadata maintains a reference count to the underlying data, so
 * it can be used independently of the Java object, and it remains valid even if
 * the Java metadata is garbage collected.
 *
 * @param env the JNI environment.
 * @param cameraMetadata the source {@link android.hardware.camera2.CameraMetadata} from which the
 *                       returned {@link ACameraMetadata} is a view.
 *
 * @return a valid ACameraMetadata pointer or NULL if {@link cameraMetadata} is null or not a valid
 *         instance of {@link android.hardware.camera2.CameraMetadata}.
 *
 */
ACameraMetadata* ACameraMetadata_fromCameraMetadata(JNIEnv* env, jobject cameraMetadata)
        __INTRODUCED_IN(30);

#endif /* __ANDROID_API__ >= 30 */
#endif  /* __ANDROID_VNDK__ */

__END_DECLS

#endif /* _NDK_CAMERA_METADATA_H */

/** @} */
