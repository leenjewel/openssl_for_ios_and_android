/*
 * Copyright (C) 2018 The Android Open Source Project
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
 * @addtogroup NdkBinder
 * @{
 */

/**
 * @file binder_status.h
 */

#pragma once

#include <errno.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS
#if __ANDROID_API__ >= 29

enum {
    STATUS_OK = 0,

    STATUS_UNKNOWN_ERROR = (-2147483647 - 1),  // INT32_MIN value
    STATUS_NO_MEMORY = -ENOMEM,
    STATUS_INVALID_OPERATION = -ENOSYS,
    STATUS_BAD_VALUE = -EINVAL,
    STATUS_BAD_TYPE = (STATUS_UNKNOWN_ERROR + 1),
    STATUS_NAME_NOT_FOUND = -ENOENT,
    STATUS_PERMISSION_DENIED = -EPERM,
    STATUS_NO_INIT = -ENODEV,
    STATUS_ALREADY_EXISTS = -EEXIST,
    STATUS_DEAD_OBJECT = -EPIPE,
    STATUS_FAILED_TRANSACTION = (STATUS_UNKNOWN_ERROR + 2),
    STATUS_BAD_INDEX = -EOVERFLOW,
    STATUS_NOT_ENOUGH_DATA = -ENODATA,
    STATUS_WOULD_BLOCK = -EWOULDBLOCK,
    STATUS_TIMED_OUT = -ETIMEDOUT,
    STATUS_UNKNOWN_TRANSACTION = -EBADMSG,
    STATUS_FDS_NOT_ALLOWED = (STATUS_UNKNOWN_ERROR + 7),
    STATUS_UNEXPECTED_NULL = (STATUS_UNKNOWN_ERROR + 8),
};

/**
 * One of the STATUS_* values.
 *
 * All unrecognized values are coerced into STATUS_UNKNOWN_ERROR.
 */
typedef int32_t binder_status_t;

enum {
    EX_NONE = 0,
    EX_SECURITY = -1,
    EX_BAD_PARCELABLE = -2,
    EX_ILLEGAL_ARGUMENT = -3,
    EX_NULL_POINTER = -4,
    EX_ILLEGAL_STATE = -5,
    EX_NETWORK_MAIN_THREAD = -6,
    EX_UNSUPPORTED_OPERATION = -7,
    EX_SERVICE_SPECIFIC = -8,
    EX_PARCELABLE = -9,

    /**
     * This is special, and indicates to native binder proxies that the
     * transaction has failed at a low level.
     */
    EX_TRANSACTION_FAILED = -129,
};

/**
 * One of the EXCEPTION_* types.
 *
 * All unrecognized values are coerced into EXCEPTION_TRANSACTION_FAILED.
 *
 * These exceptions values are used by the SDK for parcelables. Also see Parcel.java.
 */
typedef int32_t binder_exception_t;

/**
 * This is a helper class that encapsulates a standard way to keep track of and chain binder errors
 * along with service specific errors.
 *
 * It is not required to be used in order to parcel/receive transactions, but it is required in
 * order to be compatible with standard AIDL transactions since it is written as the header to the
 * out parcel for transactions which get executed (don't fail during unparceling of input arguments
 * or sooner).
 */
struct AStatus;
typedef struct AStatus AStatus;

/**
 * New status which is considered a success.
 *
 * Available since API level 29.
 *
 * \return a newly constructed status object that the caller owns.
 */
__attribute__((warn_unused_result)) AStatus* AStatus_newOk() __INTRODUCED_IN(29);

/**
 * New status with exception code.
 *
 * Available since API level 29.
 *
 * \param exception the code that this status should represent. If this is EX_NONE, then this
 * constructs an non-error status object.
 *
 * \return a newly constructed status object that the caller owns.
 */
__attribute__((warn_unused_result)) AStatus* AStatus_fromExceptionCode(binder_exception_t exception)
        __INTRODUCED_IN(29);

/**
 * New status with exception code and message.
 *
 * Available since API level 29.
 *
 * \param exception the code that this status should represent. If this is EX_NONE, then this
 * constructs an non-error status object.
 * \param message the error message to associate with this status object.
 *
 * \return a newly constructed status object that the caller owns.
 */
__attribute__((warn_unused_result)) AStatus* AStatus_fromExceptionCodeWithMessage(
        binder_exception_t exception, const char* message) __INTRODUCED_IN(29);

/**
 * New status with a service speciic error.
 *
 * This is considered to be EX_TRANSACTION_FAILED with extra information.
 *
 * Available since API level 29.
 *
 * \param serviceSpecific an implementation defined error code.
 *
 * \return a newly constructed status object that the caller owns.
 */
__attribute__((warn_unused_result)) AStatus* AStatus_fromServiceSpecificError(
        int32_t serviceSpecific) __INTRODUCED_IN(29);

/**
 * New status with a service specific error and message.
 *
 * This is considered to be EX_TRANSACTION_FAILED with extra information.
 *
 * Available since API level 29.
 *
 * \param serviceSpecific an implementation defined error code.
 * \param message the error message to associate with this status object.
 *
 * \return a newly constructed status object that the caller owns.
 */
__attribute__((warn_unused_result)) AStatus* AStatus_fromServiceSpecificErrorWithMessage(
        int32_t serviceSpecific, const char* message) __INTRODUCED_IN(29);

/**
 * New status with binder_status_t. This is typically for low level failures when a binder_status_t
 * is returned by an API on AIBinder or AParcel, and that is to be returned from a method returning
 * an AStatus instance.
 *
 * Available since API level 29.
 *
 * \param a low-level error to associate with this status object.
 *
 * \return a newly constructed status object that the caller owns.
 */
__attribute__((warn_unused_result)) AStatus* AStatus_fromStatus(binder_status_t status)
        __INTRODUCED_IN(29);

/**
 * Whether this object represents a successful transaction. If this function returns true, then
 * AStatus_getExceptionCode will return EX_NONE.
 *
 * Available since API level 29.
 *
 * \param status the status being queried.
 *
 * \return whether the status represents a successful transaction. For more details, see below.
 */
bool AStatus_isOk(const AStatus* status) __INTRODUCED_IN(29);

/**
 * The exception that this status object represents.
 *
 * Available since API level 29.
 *
 * \param status the status being queried.
 *
 * \return the exception code that this object represents.
 */
binder_exception_t AStatus_getExceptionCode(const AStatus* status) __INTRODUCED_IN(29);

/**
 * The service specific error if this object represents one. This function will only ever return a
 * non-zero result if AStatus_getExceptionCode returns EX_SERVICE_SPECIFIC. If this function returns
 * 0, the status object may still represent a different exception or status. To find out if this
 * transaction as a whole is okay, use AStatus_isOk instead.
 *
 * Available since API level 29.
 *
 * \param status the status being queried.
 *
 * \return the service-specific error code if the exception code is EX_SERVICE_SPECIFIC or 0.
 */
int32_t AStatus_getServiceSpecificError(const AStatus* status) __INTRODUCED_IN(29);

/**
 * The status if this object represents one. This function will only ever return a non-zero result
 * if AStatus_getExceptionCode returns EX_TRANSACTION_FAILED. If this function return 0, the status
 * object may represent a different exception or a service specific error. To find out if this
 * transaction as a whole is okay, use AStatus_isOk instead.
 *
 * Available since API level 29.
 *
 * \param status the status being queried.
 *
 * \return the status code if the exception code is EX_TRANSACTION_FAILED or 0.
 */
binder_status_t AStatus_getStatus(const AStatus* status) __INTRODUCED_IN(29);

/**
 * If there is a message associated with this status, this will return that message. If there is no
 * message, this will return an empty string.
 *
 * The returned string has the lifetime of the status object passed into this function.
 *
 * Available since API level 29.
 *
 * \param status the status being queried.
 *
 * \return the message associated with this error.
 */
const char* AStatus_getMessage(const AStatus* status) __INTRODUCED_IN(29);

/**
 * Get human-readable description for debugging.
 *
 * Available since API level 30.
 *
 * \param status the status being queried.
 *
 * \return a description, must be deleted with AStatus_deleteDescription.
 */
__attribute__((warn_unused_result)) const char* AStatus_getDescription(const AStatus* status)
        __INTRODUCED_IN(30);

/**
 * Delete description.
 *
 * \param description value from AStatus_getDescription
 */
void AStatus_deleteDescription(const char* description) __INTRODUCED_IN(30);

/**
 * Deletes memory associated with the status instance.
 *
 * Available since API level 29.
 *
 * \param status the status to delete, returned from AStatus_newOk or one of the AStatus_from* APIs.
 */
void AStatus_delete(AStatus* status) __INTRODUCED_IN(29);

#endif  //__ANDROID_API__ >= 29
__END_DECLS

/** @} */
