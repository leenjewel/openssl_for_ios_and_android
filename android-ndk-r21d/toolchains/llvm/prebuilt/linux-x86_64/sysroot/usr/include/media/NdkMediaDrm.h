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
 * @file NdkMediaDrm.h
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

#ifndef _NDK_MEDIA_DRM_H
#define _NDK_MEDIA_DRM_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

#include "NdkMediaError.h"

__BEGIN_DECLS

struct AMediaDrm;
typedef struct AMediaDrm AMediaDrm;

typedef struct {
    const uint8_t *ptr;
    size_t length;
} AMediaDrmByteArray;

typedef AMediaDrmByteArray AMediaDrmSessionId;
typedef AMediaDrmByteArray AMediaDrmScope;
typedef AMediaDrmByteArray AMediaDrmKeySetId;
typedef AMediaDrmByteArray AMediaDrmSecureStop;
typedef AMediaDrmByteArray AMediaDrmKeyId;

typedef enum AMediaDrmEventType {
    /**
     * This event type indicates that the app needs to request a certificate from
     * the provisioning server.  The request message data is obtained using
     * AMediaDrm_getProvisionRequest.
     */
    EVENT_PROVISION_REQUIRED = 1,

    /**
     * This event type indicates that the app needs to request keys from a license
     * server.  The request message data is obtained using AMediaDrm_getKeyRequest.
     */
    EVENT_KEY_REQUIRED = 2,

    /**
     * This event type indicates that the licensed usage duration for keys in a session
     * has expired.  The keys are no longer valid.
     */
    EVENT_KEY_EXPIRED = 3,

    /**
     * This event may indicate some specific vendor-defined condition, see your
     * DRM provider documentation for details
     */
    EVENT_VENDOR_DEFINED = 4,

    /**
     * This event indicates that a session opened by the app has been reclaimed
     * by the resource manager.
     */
    EVENT_SESSION_RECLAIMED = 5,
} AMediaDrmEventType;

typedef enum AMediaDrmKeyType {
    /**
     * This key request type specifies that the keys will be for online use, they will
     * not be saved to the device for subsequent use when the device is not connected
     * to a network.
     */
    KEY_TYPE_STREAMING = 1,

    /**
     * This key request type specifies that the keys will be for offline use, they
     * will be saved to the device for use when the device is not connected to a network.
     */
    KEY_TYPE_OFFLINE = 2,

    /**
     * This key request type specifies that previously saved offline keys should be released.
     */
    KEY_TYPE_RELEASE = 3
} AMediaDrmKeyType;

/**
 *  Data type containing {key, value} pair
 */
typedef struct AMediaDrmKeyValuePair {
    const char *mKey;
    const char *mValue;
} AMediaDrmKeyValue;

typedef enum AMediaKeyStatusType {
    /**
     * The key is currently usable to decrypt media data.
     */
    KEY_STATUS_TYPE_USABLE,

    /**
     * The key is no longer usable to decrypt media data because its expiration
     * time has passed.
     */
    KEY_STATUS_TYPE_EXPIRED,

    /**
     * The key is not currently usable to decrypt media data because its output
     * requirements cannot currently be met.
     */
    KEY_STATUS_TYPE_OUTPUTNOTALLOWED,

    /**
     * The status of the key is not yet known and is being determined.
     */
    KEY_STATUS_TYPE_STATUSPENDING,

    /**
     * The key is not currently usable to decrypt media data because of an
     * internal error in processing unrelated to input parameters.
     */
    KEY_STATUS_TYPE_INTERNALERROR,

} AMediaDrmKeyStatusType;

typedef struct AMediaDrmKeyStatus {
    AMediaDrmKeyId keyId;
    AMediaDrmKeyStatusType keyType;
} AMediaDrmKeyStatus;

typedef void (*AMediaDrmEventListener)(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        AMediaDrmEventType eventType, int extra, const uint8_t *data, size_t dataSize);

typedef void (*AMediaDrmExpirationUpdateListener)(AMediaDrm *,
        const AMediaDrmSessionId *sessionId, int64_t expiryTimeInMS);

typedef void (*AMediaDrmKeysChangeListener)(AMediaDrm *,
        const AMediaDrmSessionId *sessionId, const AMediaDrmKeyStatus *keyStatus,
        size_t numKeys, bool hasNewUsableKey);

#if __ANDROID_API__ >= 21

/**
 * Query if the given scheme identified by its UUID is supported on this device, and
 * whether the drm plugin is able to handle the media container format specified by mimeType.
 *
 * uuid identifies the universal unique ID of the crypto scheme. uuid must be 16 bytes.
 * mimeType is the MIME type of the media container, e.g. "video/mp4".  If mimeType
 * is not known or required, it can be provided as NULL.
 *
 * Available since API level 21.
 */
bool AMediaDrm_isCryptoSchemeSupported(const uint8_t *uuid,
        const char *mimeType) __INTRODUCED_IN(21);

/**
 * Create a MediaDrm instance from a UUID.
 * uuid identifies the universal unique ID of the crypto scheme. uuid must be 16 bytes.
 *
 * Available since API level 21.
 */
AMediaDrm* AMediaDrm_createByUUID(const uint8_t *uuid) __INTRODUCED_IN(21);

/**
 * Release a MediaDrm object.
 *
 * Available since API level 21.
 */
void AMediaDrm_release(AMediaDrm *) __INTRODUCED_IN(21);

/**
 * Register a callback to be invoked when an event occurs.
 *
 * listener is the callback that will be invoked on event.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_setOnEventListener(AMediaDrm *,
        AMediaDrmEventListener listener) __INTRODUCED_IN(21);

/**
 * Register a callback to be invoked when an expiration update event occurs.
 *
 * listener is the callback that will be invoked on event.
 *
 * Available since API level 29.
 */
media_status_t AMediaDrm_setOnExpirationUpdateListener(AMediaDrm *,
        AMediaDrmExpirationUpdateListener listener) __INTRODUCED_IN(29);

/**
 * Register a callback to be invoked when a key status change event occurs.
 *
 * listener is the callback that will be invoked on event.
 *
 * Available since API level 29.
 */
media_status_t AMediaDrm_setOnKeysChangeListener(AMediaDrm *,
        AMediaDrmKeysChangeListener listener) __INTRODUCED_IN(29);

/**
 * Open a new session with the MediaDrm object.  A session ID is returned.
 *
 * Returns MEDIADRM_NOT_PROVISIONED_ERROR if provisioning is needed.
 * Returns MEDIADRM_RESOURCE_BUSY_ERROR if required resources are in use.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_openSession(AMediaDrm *,
        AMediaDrmSessionId *sessionId) __INTRODUCED_IN(21);

/**
 * Close a session on the MediaDrm object that was previously opened
 * with AMediaDrm_openSession.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_closeSession(AMediaDrm *,
        const AMediaDrmSessionId *sessionId) __INTRODUCED_IN(21);

/**
 * A key request/response exchange occurs between the app and a license server
 * to obtain or release keys used to decrypt encrypted content.
 * AMediaDrm_getKeyRequest is used to obtain an opaque key request byte array that
 * is delivered to the license server.  The opaque key request byte array is
 * returned in KeyRequest.data.
 *
 * After the app has received the key request response from the server,
 * it should deliver to the response to the DRM engine plugin using the method
 * AMediaDrm_provideKeyResponse.
 *
 * scope may be a sessionId or a keySetId, depending on the specified keyType.
 * When the keyType is KEY_TYPE_STREAMING or KEY_TYPE_OFFLINE, scope should be set
 * to the sessionId the keys will be provided to.  When the keyType is
 * KEY_TYPE_RELEASE, scope should be set to the keySetId of the keys being released.
 * Releasing keys from a device invalidates them for all sessions.
 *
 * init container-specific data, its meaning is interpreted based on the mime type
 * provided in the mimeType parameter.  It could contain, for example, the content
 * ID, key ID or other data obtained from the content metadata that is required in
 * generating the key request. init may be null when keyType is KEY_TYPE_RELEASE.
 *
 * initSize is the number of bytes of initData
 *
 * mimeType identifies the mime type of the content.
 *
 * keyType specifes the type of the request. The request may be to acquire keys for
 *   streaming or offline content, or to release previously acquired keys, which are
 *   identified by a keySetId.
 *
 * optionalParameters are included in the key request message to allow a client
 *   application to provide additional message parameters to the server.
 *
 * numOptionalParameters indicates the number of optional parameters provided
 *   by the caller
 *
 * On exit:
 *   1. The keyRequest pointer will reference the opaque key request data.  It
 *       will reside in memory owned by the AMediaDrm object, and will remain
 *       accessible until the next call to AMediaDrm_getKeyRequest or until the
 *       MediaDrm object is released.
 *   2. keyRequestSize will be set to the size of the request
 *
 * Returns MEDIADRM_NOT_PROVISIONED_ERROR if reprovisioning is needed, due to a
 * problem with the device certificate.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_getKeyRequest(AMediaDrm *, const AMediaDrmScope *scope,
        const uint8_t *init, size_t initSize, const char *mimeType, AMediaDrmKeyType keyType,
        const AMediaDrmKeyValue *optionalParameters, size_t numOptionalParameters,
        const uint8_t **keyRequest, size_t *keyRequestSize) __INTRODUCED_IN(21);

/**
 * A key response is received from the license server by the app, then it is
 * provided to the DRM engine plugin using provideKeyResponse.  When the
 * response is for an offline key request, a keySetId is returned that can be
 * used to later restore the keys to a new session with AMediaDrm_restoreKeys.
 * When the response is for a streaming or release request, a null keySetId is
 * returned.
 *
 * scope may be a sessionId or keySetId depending on the type of the
 * response.  Scope should be set to the sessionId when the response is for either
 * streaming or offline key requests.  Scope should be set to the keySetId when
 * the response is for a release request.
 *
 * response points to the opaque response from the server
 * responseSize should be set to the size of the response in bytes
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_provideKeyResponse(AMediaDrm *, const AMediaDrmScope *scope,
        const uint8_t *response, size_t responseSize,
        AMediaDrmKeySetId *keySetId) __INTRODUCED_IN(21);

/**
 * Restore persisted offline keys into a new session.  keySetId identifies the
 * keys to load, obtained from a prior call to AMediaDrm_provideKeyResponse.
 *
 * sessionId is the session ID for the DRM session.
 * keySetId identifies the saved key set to restore.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_restoreKeys(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        const AMediaDrmKeySetId *keySetId) __INTRODUCED_IN(21);

/**
 * Remove the current keys from a session.
 *
 * keySetId identifies keys to remove.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_removeKeys(AMediaDrm *,
        const AMediaDrmSessionId *keySetId) __INTRODUCED_IN(21);

/**
 * Request an informative description of the key status for the session.  The status is
 * in the form of {key, value} pairs.  Since DRM license policies vary by vendor,
 * the specific status field names are determined by each DRM vendor.  Refer to your
 * DRM provider documentation for definitions of the field names for a particular
 * DRM engine plugin.
 *
 * On entry, numPairs should be set by the caller to the maximum number of pairs
 * that can be returned (the size of the array).  On exit, numPairs will be set
 * to the number of entries written to the array.  If the number of {key, value} pairs
 * to be returned is greater than *numPairs, MEDIADRM_SHORT_BUFFER will be returned
 * and numPairs will be set to the number of pairs available.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_queryKeyStatus(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        AMediaDrmKeyValue *keyValuePairs, size_t *numPairs) __INTRODUCED_IN(21);


/**
 * A provision request/response exchange occurs between the app and a provisioning
 * server to retrieve a device certificate.  If provisionining is required, the
 * EVENT_PROVISION_REQUIRED event will be sent to the event handler.
 * getProvisionRequest is used to obtain the opaque provision request byte array that
 * should be delivered to the provisioning server.
 * On exit:
 *    1. The provision request data will be referenced by provisionRequest, in
 *        memory owned by the AMediaDrm object.  It will remain accessible until the
 *        next call to getProvisionRequest.
 *    2. provisionRequestSize will be set to the size of the request data.
 *    3. serverUrl will reference a NULL terminated string containing the URL
 *       the provisioning request should be sent to.  It will remain accessible until
 *       the next call to getProvisionRequest.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_getProvisionRequest(AMediaDrm *, const uint8_t **provisionRequest,
        size_t *provisionRequestSize, const char **serverUrl) __INTRODUCED_IN(21);


/**
 * After a provision response is received by the app, it is provided to the DRM
 * engine plugin using this method.
 *
 * response is the opaque provisioning response byte array to provide to the
 *   DRM engine plugin.
 * responseSize is the length of the provisioning response in bytes.
 *
 * Returns MEDIADRM_DEVICE_REVOKED_ERROR if the response indicates that the
 * server rejected the request
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_provideProvisionResponse(AMediaDrm *,
        const uint8_t *response, size_t responseSize) __INTRODUCED_IN(21);


/**
 * A means of enforcing limits on the number of concurrent streams per subscriber
 * across devices is provided via SecureStop. This is achieved by securely
 * monitoring the lifetime of sessions.
 *
 * Information from the server related to the current playback session is written
 * to persistent storage on the device when each MediaCrypto object is created.
 *
 * In the normal case, playback will be completed, the session destroyed and the
 * Secure Stops will be queried. The app queries secure stops and forwards the
 * secure stop message to the server which verifies the signature and notifies the
 * server side database that the session destruction has been confirmed. The persisted
 * record on the client is only removed after positive confirmation that the server
 * received the message using releaseSecureStops().
 *
 * numSecureStops is set by the caller to the maximum number of secure stops to
 * return.  On exit, *numSecureStops will be set to the number actually returned.
 * If *numSecureStops is too small for the number of secure stops available,
 * MEDIADRM_SHORT_BUFFER will be returned and *numSecureStops will be set to the
 * number required.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_getSecureStops(AMediaDrm *,
        AMediaDrmSecureStop *secureStops, size_t *numSecureStops) __INTRODUCED_IN(21);

/**
 * Process the SecureStop server response message ssRelease.  After authenticating
 * the message, remove the SecureStops identified in the response.
 *
 * ssRelease is the server response indicating which secure stops to release
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_releaseSecureStops(AMediaDrm *,
        const AMediaDrmSecureStop *ssRelease) __INTRODUCED_IN(21);

/**
 * String property name: identifies the maker of the DRM engine plugin
 */
#define PROPERTY_VENDOR "vendor"

/**
 * String property name: identifies the version of the DRM engine plugin
 */
#define PROPERTY_VERSION "version"

/**
 * String property name: describes the DRM engine plugin
 */
#define PROPERTY_DESCRIPTION "description"

/**
 * String property name: a comma-separated list of cipher and mac algorithms
 * supported by CryptoSession.  The list may be empty if the DRM engine
 * plugin does not support CryptoSession operations.
 */
#define PROPERTY_ALGORITHMS "algorithms"

/**
 * Read a DRM engine plugin String property value, given the property name string.
 *
 * propertyName identifies the property to query
 * On return, propertyValue will be set to point to the property value.  The
 * memory that the value resides in is owned by the NDK MediaDrm API and
 * will remain valid until the next call to AMediaDrm_getPropertyString.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_getPropertyString(AMediaDrm *, const char *propertyName,
        const char **propertyValue) __INTRODUCED_IN(21);

/**
 * Byte array property name: the device unique identifier is established during
 * device provisioning and provides a means of uniquely identifying each device.
 */
#define PROPERTY_DEVICE_UNIQUE_ID "deviceUniqueId"

/**
 * Read a DRM engine plugin byte array property value, given the property name string.
 * On return, *propertyValue will be set to point to the property value.  The
 * memory that the value resides in is owned by the NDK MediaDrm API and
 * will remain valid until the next call to AMediaDrm_getPropertyByteArray.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_getPropertyByteArray(AMediaDrm *, const char *propertyName,
        AMediaDrmByteArray *propertyValue) __INTRODUCED_IN(21);

/**
 * Set a DRM engine plugin String property value.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_setPropertyString(AMediaDrm *, const char *propertyName,
        const char *value) __INTRODUCED_IN(21);

/**
 * Set a DRM engine plugin byte array property value.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_setPropertyByteArray(AMediaDrm *, const char *propertyName,
        const uint8_t *value, size_t valueSize) __INTRODUCED_IN(21);

/**
 * In addition to supporting decryption of DASH Common Encrypted Media, the
 * MediaDrm APIs provide the ability to securely deliver session keys from
 * an operator's session key server to a client device, based on the factory-installed
 * root of trust, and then perform encrypt, decrypt, sign and verify operations
 * with the session key on arbitrary user data.
 *
 * Operators create session key servers that receive session key requests and provide
 * encrypted session keys which can be used for general purpose crypto operations.
 *
 * Generic encrypt/decrypt/sign/verify methods are based on the established session
 * keys.  These keys are exchanged using the getKeyRequest/provideKeyResponse methods.
 *
 * Applications of this capability include securing various types of purchased or
 * private content, such as applications, books and other media, photos or media
 * delivery protocols.
 */

/*
 * Encrypt the data referenced by input of length dataSize using algorithm specified
 * by cipherAlgorithm, and write the encrypted result into output.  The caller must
 * ensure that the output buffer is large enough to accept dataSize bytes. The key
 * to use is identified by the 16 byte keyId.  The key must have been loaded into
 * the session using provideKeyResponse.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_encrypt(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        const char *cipherAlgorithm, uint8_t *keyId, uint8_t *iv,
        const uint8_t *input, uint8_t *output, size_t dataSize) __INTRODUCED_IN(21);

/*
 * Decrypt the data referenced by input of length dataSize using algorithm specified
 * by cipherAlgorithm, and write the decrypted result into output.  The caller must
 * ensure that the output buffer is large enough to accept dataSize bytes.  The key
 * to use is identified by the 16 byte keyId.  The key must have been loaded into
 * the session using provideKeyResponse.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_decrypt(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        const char *cipherAlgorithm, uint8_t *keyId, uint8_t *iv,
        const uint8_t *input, uint8_t *output, size_t dataSize) __INTRODUCED_IN(21);

/*
 * Generate a signature using the specified macAlgorithm over the message data
 * referenced by message of size messageSize and store the signature in the
 * buffer referenced signature of max size *signatureSize.  If the buffer is not
 * large enough to hold the signature, MEDIADRM_SHORT_BUFFER is returned and
 * *signatureSize is set to the buffer size required.  The key to use is identified
 * by the 16 byte keyId.  The key must have been loaded into the session using
 * provideKeyResponse.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_sign(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        const char *macAlgorithm, uint8_t *keyId, uint8_t *message, size_t messageSize,
        uint8_t *signature, size_t *signatureSize) __INTRODUCED_IN(21);

/*
 * Perform a signature verification using the specified macAlgorithm over the message
 * data referenced by the message parameter of size messageSize. Returns MEDIADRM_OK
 * if the signature matches, otherwise MEDAIDRM_VERIFY_FAILED is returned. The key to
 * use is identified by the 16 byte keyId.  The key must have been loaded into the
 * session using provideKeyResponse.
 *
 * Available since API level 21.
 */
media_status_t AMediaDrm_verify(AMediaDrm *, const AMediaDrmSessionId *sessionId,
        const char *macAlgorithm, uint8_t *keyId, const uint8_t *message, size_t messageSize,
        const uint8_t *signature, size_t signatureSize) __INTRODUCED_IN(21);

#endif /* __ANDROID_API__ >= 21 */

__END_DECLS

#endif //_NDK_MEDIA_DRM_H

/** @} */
