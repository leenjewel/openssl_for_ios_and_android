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
 * @file NdkCameraManager.h
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

#ifndef _NDK_CAMERA_MANAGER_H
#define _NDK_CAMERA_MANAGER_H

#include <sys/cdefs.h>

#include "NdkCameraError.h"
#include "NdkCameraMetadata.h"
#include "NdkCameraDevice.h"

__BEGIN_DECLS

#if __ANDROID_API__ >= 24

/**
 * ACameraManager is opaque type that provides access to camera service.
 *
 * A pointer can be obtained using {@link ACameraManager_create} method.
 */
typedef struct ACameraManager ACameraManager;

/**
 * Create ACameraManager instance.
 *
 * <p>The ACameraManager is responsible for
 * detecting, characterizing, and connecting to {@link ACameraDevice}s.</p>
 *
 * <p>The caller must call {@link ACameraManager_delete} to free the resources once it is done
 * using the ACameraManager instance.</p>
 *
 * @return a {@link ACameraManager} instance.
 *
 */
ACameraManager* ACameraManager_create() __INTRODUCED_IN(24);

/**
 * <p>Delete the {@link ACameraManager} instance and free its resources. </p>
 *
 * @param manager the {@link ACameraManager} instance to be deleted.
 */
void ACameraManager_delete(ACameraManager* manager) __INTRODUCED_IN(24);

/**
 * Create a list of currently connected camera devices, including
 * cameras that may be in use by other camera API clients.
 *
 * <p>Non-removable cameras use integers starting at 0 for their
 * identifiers, while removable cameras have a unique identifier for each
 * individual device, even if they are the same model.</p>
 *
 * <p>ACameraManager_getCameraIdList will allocate and return an {@link ACameraIdList}.
 * The caller must call {@link ACameraManager_deleteCameraIdList} to free the memory</p>
 *
 * <p>Note: the returned camera list might be a subset to the output of <a href=
 * "https://developer.android.com/reference/android/hardware/camera2/CameraManager.html#getCameraIdList()">
 * SDK CameraManager#getCameraIdList API</a> as the NDK API does not support some legacy camera
 * hardware.</p>
 *
 * @param manager the {@link ACameraManager} of interest
 * @param cameraIdList the output {@link ACameraIdList} will be filled in here if the method call
 *        succeeds.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if manager or cameraIdList is NULL.</li>
 *         <li>{@link ACAMERA_ERROR_CAMERA_DISCONNECTED} if connection to camera service fails.</li>
 *         <li>{@link ACAMERA_ERROR_NOT_ENOUGH_MEMORY} if allocating memory fails.</li></ul>
 */
camera_status_t ACameraManager_getCameraIdList(ACameraManager* manager,
        /*out*/ACameraIdList** cameraIdList) __INTRODUCED_IN(24);

/**
 * Delete a list of camera devices allocated via {@link ACameraManager_getCameraIdList}.
 *
 * @param cameraIdList the {@link ACameraIdList} to be deleted.
 */
void ACameraManager_deleteCameraIdList(ACameraIdList* cameraIdList) __INTRODUCED_IN(24);

/**
 * Definition of camera availability callbacks.
 *
 * @param context The optional application context provided by user in
 *                {@link ACameraManager_AvailabilityCallbacks}.
 * @param cameraId The ID of the camera device whose availability is changing. The memory of this
 *                 argument is owned by camera framework and will become invalid immediately after
 *                 this callback returns.
 */
typedef void (*ACameraManager_AvailabilityCallback)(void* context,
        const char* cameraId);

/**
 * Definition of physical camera availability callbacks.
 *
 * @param context The optional application context provided by user in
 *                {@link ACameraManager_AvailabilityCallbacks}.
 * @param cameraId The ID of the logical multi-camera device whose physical camera status is
 *                 changing. The memory of this argument is owned by camera framework and will
 *                 become invalid immediately after this callback returns.
 * @param physicalCameraId The ID of the physical camera device whose status is changing. The
 *                 memory of this argument is owned by camera framework and will become invalid
 *                 immediately after this callback returns.
 */
typedef void (*ACameraManager_PhysicalCameraAvailabilityCallback)(void* context,
        const char* cameraId, const char* physicalCameraId);

/**
 * A listener for camera devices becoming available or unavailable to open.
 *
 * <p>Cameras become available when they are no longer in use, or when a new
 * removable camera is connected. They become unavailable when some
 * application or service starts using a camera, or when a removable camera
 * is disconnected.</p>
 *
 * @see ACameraManager_registerAvailabilityCallback
 */
typedef struct ACameraManager_AvailabilityListener {
    /// Optional application context.
    void*                               context;
    /// Called when a camera becomes available
    ACameraManager_AvailabilityCallback onCameraAvailable;
    /// Called when a camera becomes unavailable
    ACameraManager_AvailabilityCallback onCameraUnavailable;
} ACameraManager_AvailabilityCallbacks;

/**
 * Register camera availability callbacks.
 *
 * <p>onCameraUnavailable will be called whenever a camera device is opened by any camera API client.
 * Other camera API clients may still be able to open such a camera device, evicting the existing
 * client if they have higher priority than the existing client of a camera device.
 * See {@link ACameraManager_openCamera} for more details.</p>
 *
 * <p>The callbacks will be called on a dedicated thread shared among all ACameraManager
 * instances.</p>
 *
 * <p>Since this callback will be registered with the camera service, remember to unregister it
 * once it is no longer needed; otherwise the callback will continue to receive events
 * indefinitely and it may prevent other resources from being released. Specifically, the
 * callbacks will be invoked independently of the general activity lifecycle and independently
 * of the state of individual ACameraManager instances.</p>
 *
 * @param manager the {@link ACameraManager} of interest.
 * @param callback the {@link ACameraManager_AvailabilityCallbacks} to be registered.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if manager or callback is NULL, or
 *                  {ACameraManager_AvailabilityCallbacks#onCameraAvailable} or
 *                  {ACameraManager_AvailabilityCallbacks#onCameraUnavailable} is NULL.</li></ul>
 */
camera_status_t ACameraManager_registerAvailabilityCallback(
        ACameraManager* manager,
        const ACameraManager_AvailabilityCallbacks* callback) __INTRODUCED_IN(24);

/**
 * Unregister camera availability callbacks.
 *
 * <p>Removing a callback that isn't registered has no effect.</p>
 *
 * <p>This function must not be called with a mutex lock also held by
 * the availability callbacks.</p>
 *
 * @param manager the {@link ACameraManager} of interest.
 * @param callback the {@link ACameraManager_AvailabilityCallbacks} to be unregistered.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if callback,
 *                  {ACameraManager_AvailabilityCallbacks#onCameraAvailable} or
 *                  {ACameraManager_AvailabilityCallbacks#onCameraUnavailable} is NULL.</li></ul>
 */
camera_status_t ACameraManager_unregisterAvailabilityCallback(
        ACameraManager* manager,
        const ACameraManager_AvailabilityCallbacks* callback) __INTRODUCED_IN(24);

/**
 * Query the capabilities of a camera device. These capabilities are
 * immutable for a given camera.
 *
 * <p>See {@link ACameraMetadata} document and {@link NdkCameraMetadataTags.h} for more details.</p>
 *
 * <p>The caller must call {@link ACameraMetadata_free} to free the memory of the output
 * characteristics.</p>
 *
 * @param manager the {@link ACameraManager} of interest.
 * @param cameraId the ID string of the camera device of interest.
 * @param characteristics the output {@link ACameraMetadata} will be filled here if the method call
 *        succeeeds.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if manager, cameraId, or characteristics
 *                  is NULL, or cameraId does not match any camera devices connected.</li>
 *         <li>{@link ACAMERA_ERROR_CAMERA_DISCONNECTED} if connection to camera service fails.</li>
 *         <li>{@link ACAMERA_ERROR_NOT_ENOUGH_MEMORY} if allocating memory fails.</li>
 *         <li>{@link ACAMERA_ERROR_UNKNOWN} if the method fails for some other reasons.</li></ul>
 */
camera_status_t ACameraManager_getCameraCharacteristics(
        ACameraManager* manager, const char* cameraId,
        /*out*/ACameraMetadata** characteristics) __INTRODUCED_IN(24);

/**
 * Open a connection to a camera with the given ID. The opened camera device will be
 * returned in the `device` parameter.
 *
 * <p>Use {@link ACameraManager_getCameraIdList} to get the list of available camera
 * devices. Note that even if an id is listed, open may fail if the device
 * is disconnected between the calls to {@link ACameraManager_getCameraIdList} and
 * {@link ACameraManager_openCamera}, or if a higher-priority camera API client begins using the
 * camera device.</p>
 *
 * <p>Devices for which the
 * {@link ACameraManager_AvailabilityCallbacks#onCameraUnavailable} callback has been called due to
 * the device being in use by a lower-priority, background camera API client can still potentially
 * be opened by calling this method when the calling camera API client has a higher priority
 * than the current camera API client using this device.  In general, if the top, foreground
 * activity is running within your application process, your process will be given the highest
 * priority when accessing the camera, and this method will succeed even if the camera device is
 * in use by another camera API client. Any lower-priority application that loses control of the
 * camera in this way will receive an
 * {@link ACameraDevice_StateCallbacks#onDisconnected} callback.</p>
 *
 * <p>Once the camera is successfully opened,the ACameraDevice can then be set up
 * for operation by calling {@link ACameraDevice_createCaptureSession} and
 * {@link ACameraDevice_createCaptureRequest}.</p>
 *
 * <p>If the camera becomes disconnected after this function call returns,
 * {@link ACameraDevice_StateCallbacks#onDisconnected} with a
 * ACameraDevice in the disconnected state will be called.</p>
 *
 * <p>If the camera runs into error after this function call returns,
 * {@link ACameraDevice_StateCallbacks#onError} with a
 * ACameraDevice in the error state will be called.</p>
 *
 * @param manager the {@link ACameraManager} of interest.
 * @param cameraId the ID string of the camera device to be opened.
 * @param callback the {@link ACameraDevice_StateCallbacks} associated with the opened camera device.
 * @param device the opened {@link ACameraDevice} will be filled here if the method call succeeds.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if manager, cameraId, callback, or device
 *                  is NULL, or cameraId does not match any camera devices connected.</li>
 *         <li>{@link ACAMERA_ERROR_CAMERA_DISCONNECTED} if connection to camera service fails.</li>
 *         <li>{@link ACAMERA_ERROR_NOT_ENOUGH_MEMORY} if allocating memory fails.</li>
 *         <li>{@link ACAMERA_ERROR_CAMERA_IN_USE} if camera device is being used by a higher
 *                   priority camera API client.</li>
 *         <li>{@link ACAMERA_ERROR_MAX_CAMERA_IN_USE} if the system-wide limit for number of open
 *                   cameras or camera resources has been reached, and more camera devices cannot be
 *                   opened until previous instances are closed.</li>
 *         <li>{@link ACAMERA_ERROR_CAMERA_DISABLED} if the camera is disabled due to a device
 *                   policy, and cannot be opened.</li>
 *         <li>{@link ACAMERA_ERROR_PERMISSION_DENIED} if the application does not have permission
 *                   to open camera.</li>
 *         <li>{@link ACAMERA_ERROR_UNKNOWN} if the method fails for some other reasons.</li></ul>
 */
camera_status_t ACameraManager_openCamera(
        ACameraManager* manager, const char* cameraId,
        ACameraDevice_StateCallbacks* callback,
        /*out*/ACameraDevice** device) __INTRODUCED_IN(24);

#endif /* __ANDROID_API__ >= 24 */

#if __ANDROID_API__ >= 29

/**
 * Definition of camera access permission change callback.
 *
 * <p>Notification that camera access priorities have changed and the camera may
 * now be openable. An application that was previously denied camera access due to
 * a higher-priority user already using the camera, or that was disconnected from an
 * active camera session due to a higher-priority user trying to open the camera,
 * should try to open the camera again if it still wants to use it.  Note that
 * multiple applications may receive this callback at the same time, and only one of
 * them will succeed in opening the camera in practice, depending on exact access
 * priority levels and timing. This method is useful in cases where multiple
 * applications may be in the resumed state at the same time, and the user switches
 * focus between them, or if the current camera-using application moves between
 * full-screen and Picture-in-Picture (PiP) states. In such cases, the camera
 * available/unavailable callbacks will not be invoked, but another application may
 * now have higher priority for camera access than the current camera-using
 * application.</p>

 * @param context The optional application context provided by user in
 *                {@link ACameraManager_AvailabilityListener}.
 */
typedef void (*ACameraManager_AccessPrioritiesChangedCallback)(void* context);

/**
 * A listener for camera devices becoming available/unavailable to open or when
 * the camera access permissions change.
 *
 * <p>Cameras become available when they are no longer in use, or when a new
 * removable camera is connected. They become unavailable when some
 * application or service starts using a camera, or when a removable camera
 * is disconnected.</p>
 *
 * @see ACameraManager_registerExtendedAvailabilityCallback
 */
typedef struct ACameraManager_ExtendedAvailabilityListener {
    ///
    ACameraManager_AvailabilityCallbacks availabilityCallbacks;

    /// Called when there is camera access permission change
    ACameraManager_AccessPrioritiesChangedCallback onCameraAccessPrioritiesChanged;

    /// Called when a physical camera becomes available
    ACameraManager_PhysicalCameraAvailabilityCallback onPhysicalCameraAvailable __INTRODUCED_IN(30);

    /// Called when a physical camera becomes unavailable
    ACameraManager_PhysicalCameraAvailabilityCallback onPhysicalCameraUnavailable
            __INTRODUCED_IN(30);

    /// Reserved for future use, please ensure that all entries are set to NULL
    void *reserved[4];
} ACameraManager_ExtendedAvailabilityCallbacks;

/**
 * Register camera extended availability callbacks.
 *
 * <p>onCameraUnavailable will be called whenever a camera device is opened by any camera API
 * client. Other camera API clients may still be able to open such a camera device, evicting the
 * existing client if they have higher priority than the existing client of a camera device.
 * See {@link ACameraManager_openCamera} for more details.</p>
 *
 * <p>The callbacks will be called on a dedicated thread shared among all ACameraManager
 * instances.</p>
 *
 * <p>Since this callback will be registered with the camera service, remember to unregister it
 * once it is no longer needed; otherwise the callback will continue to receive events
 * indefinitely and it may prevent other resources from being released. Specifically, the
 * callbacks will be invoked independently of the general activity lifecycle and independently
 * of the state of individual ACameraManager instances.</p>
 *
 * @param manager the {@link ACameraManager} of interest.
 * @param callback the {@link ACameraManager_ExtendedAvailabilityCallbacks} to be registered.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if manager or callback is NULL, or
 *                  {ACameraManager_ExtendedAvailabilityCallbacks#onCameraAccessPrioritiesChanged}
 *                  or {ACameraManager_AvailabilityCallbacks#onCameraAvailable} or
 *                  {ACameraManager_AvailabilityCallbacks#onCameraUnavailable} is NULL.</li></ul>
 */
camera_status_t ACameraManager_registerExtendedAvailabilityCallback(
        ACameraManager* manager,
        const ACameraManager_ExtendedAvailabilityCallbacks* callback) __INTRODUCED_IN(29);

/**
 * Unregister camera extended availability callbacks.
 *
 * <p>Removing a callback that isn't registered has no effect.</p>
 *
 * <p>This function must not be called with a mutex lock also held by
 * the extended availability callbacks.</p>
 *
 * @param manager the {@link ACameraManager} of interest.
 * @param callback the {@link ACameraManager_ExtendedAvailabilityCallbacks} to be unregistered.
 *
 * @return <ul>
 *         <li>{@link ACAMERA_OK} if the method call succeeds.</li>
 *         <li>{@link ACAMERA_ERROR_INVALID_PARAMETER} if callback,
 *                  {ACameraManager_ExtendedAvailabilityCallbacks#onCameraAccessPrioritiesChanged}
 *                  or {ACameraManager_AvailabilityCallbacks#onCameraAvailable} or
 *                  {ACameraManager_AvailabilityCallbacks#onCameraUnavailable} is NULL.</li></ul>
 */
camera_status_t ACameraManager_unregisterExtendedAvailabilityCallback(
        ACameraManager* manager,
        const ACameraManager_ExtendedAvailabilityCallbacks* callback) __INTRODUCED_IN(29);

#ifdef __ANDROID_VNDK__
/**
 * Retrieve the tag value, given the tag name and camera id.
 * This method is device specific since some metadata might be defined by device manufacturers
 * and might only be accessible for specific cameras.
 * @param manager The {@link ACameraManager} of interest.
 * @param cameraId The cameraId, which is used to query camera characteristics.
 * @param name The name of the tag being queried.
 * @param tag The output tag assigned by this method.
 *
 * @return ACAMERA_OK only if the function call was successful.
 */
camera_status_t ACameraManager_getTagFromName(ACameraManager *manager, const char* cameraId,
        const char *name, /*out*/uint32_t *tag)
        __INTRODUCED_IN(29);
#endif

#endif /* __ANDROID_API__ >= 29 */

__END_DECLS

#endif /* _NDK_CAMERA_MANAGER_H */

/** @} */
