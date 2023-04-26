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


/*
 * This file defines an NDK API.
 * Do not remove methods.
 * Do not change method signatures.
 * Do not change the value of constants.
 * Do not change the size of any of the classes defined in here.
 * Do not reference types that are not part of the NDK.
 * Do not #include files that aren't part of the NDK.
 */

#ifndef _NDK_MEDIA_DATASOURCE_H
#define _NDK_MEDIA_DATASOURCE_H

#include <sys/cdefs.h>
#include <sys/types.h>

#include <media/NdkMediaError.h>

__BEGIN_DECLS

struct AMediaDataSource;
typedef struct AMediaDataSource AMediaDataSource;

#if __ANDROID_API__ >= 28

/*
 * AMediaDataSource's callbacks will be invoked on an implementation-defined thread
 * or thread pool. No guarantees are provided about which thread(s) will be used for
 * callbacks. For example, |close| can be invoked from a different thread than the
 * thread invoking |readAt|. As such, the Implementations of AMediaDataSource callbacks
 * must be threadsafe.
 */

/**
 * Called to request data from the given |offset|.
 *
 * Implementations should should write up to |size| bytes into
 * |buffer|, and return the number of bytes written.
 *
 * Return 0 if size is zero (thus no bytes are read).
 *
 * Return -1 to indicate that end of stream is reached.
 */
typedef ssize_t (*AMediaDataSourceReadAt)(
        void *userdata, off64_t offset, void * buffer, size_t size);

/**
 * Called to get the size of the data source.
 *
 * Return the size of data source in bytes, or -1 if the size is unknown.
 */
typedef ssize_t (*AMediaDataSourceGetSize)(void *userdata);

/**
 * Called to close the data source, unblock reads, and release associated
 * resources.
 *
 * The NDK media framework guarantees that after the first |close| is
 * called, no future callbacks will be invoked on the data source except
 * for |close| itself.
 *
 * Closing a data source allows readAt calls that were blocked waiting
 * for I/O data to return promptly.
 *
 * When using AMediaDataSource as input to AMediaExtractor, closing
 * has the effect of unblocking slow reads inside of setDataSource
 * and readSampleData.
 */
typedef void (*AMediaDataSourceClose)(void *userdata);

/**
 * Create new media data source. Returns NULL if memory allocation
 * for the new data source object fails.
 *
 * Available since API level 28.
 */
AMediaDataSource* AMediaDataSource_new() __INTRODUCED_IN(28);

#if __ANDROID_API__ >= 29

/**
 * Called to get an estimate of the number of bytes that can be read from this data source
 * starting at |offset| without blocking for I/O.
 *
 * Return -1 when such an estimate is not possible.
 */
typedef ssize_t (*AMediaDataSourceGetAvailableSize)(void *userdata, off64_t offset);

/**
 * Create new media data source. Returns NULL if memory allocation
 * for the new data source object fails.
 *
 * Set the |uri| from which the data source will read,
 * plus additional http headers when initiating the request.
 *
 * Headers will contain corresponding items from |key_values|
 * in the following fashion:
 *
 * key_values[0]:key_values[1]
 * key_values[2]:key_values[3]
 * ...
 * key_values[(numheaders - 1) * 2]:key_values[(numheaders - 1) * 2 + 1]
 *
 * Available since API level 29.
 */
AMediaDataSource* AMediaDataSource_newUri(const char *uri,
        int numheaders,
        const char * const *key_values) __INTRODUCED_IN(29);

#endif  /*__ANDROID_API__ >= 29 */

/**
 * Delete a previously created media data source.
 *
 * Available since API level 28.
 */
void AMediaDataSource_delete(AMediaDataSource*) __INTRODUCED_IN(28);

/**
 * Set an user provided opaque handle. This opaque handle is passed as
 * the first argument to the data source callbacks.
 *
 * Available since API level 28.
 */
void AMediaDataSource_setUserdata(
        AMediaDataSource*, void *userdata) __INTRODUCED_IN(28);

/**
 * Set a custom callback for supplying random access media data to the
 * NDK media framework.
 *
 * Implement this if your app has special requirements for the way media
 * data is obtained, or if you need a callback when data is read by the
 * NDK media framework.
 *
 * Please refer to the definition of AMediaDataSourceReadAt for
 * additional details.
 *
 * Available since API level 28.
 */
void AMediaDataSource_setReadAt(
        AMediaDataSource*,
        AMediaDataSourceReadAt) __INTRODUCED_IN(28);

/**
 * Set a custom callback for supplying the size of the data source to the
 * NDK media framework.
 *
 * Please refer to the definition of AMediaDataSourceGetSize for
 * additional details.
 *
 * Available since API level 28.
 */
void AMediaDataSource_setGetSize(
        AMediaDataSource*,
        AMediaDataSourceGetSize) __INTRODUCED_IN(28);

/**
 * Set a custom callback to receive signal from the NDK media framework
 * when the data source is closed.
 *
 * Please refer to the definition of AMediaDataSourceClose for
 * additional details.
 *
 * Available since API level 28.
 */
void AMediaDataSource_setClose(
        AMediaDataSource*,
        AMediaDataSourceClose) __INTRODUCED_IN(28);

#endif  /*__ANDROID_API__ >= 28 */

#if __ANDROID_API__ >= 29

/**
 * Close the data source, unblock reads, and release associated resources.
 *
 * Please refer to the definition of AMediaDataSourceClose for
 * additional details.
 *
 * Available since API level 29.
 */
void AMediaDataSource_close(AMediaDataSource*) __INTRODUCED_IN(29);

/**
 * Set a custom callback for supplying the estimated number of bytes
 * that can be read from this data source starting at an offset without
 * blocking for I/O.
 *
 * Please refer to the definition of AMediaDataSourceGetAvailableSize
 * for additional details.
 *
 * Available since API level 29.
 */
void AMediaDataSource_setGetAvailableSize(
        AMediaDataSource*,
        AMediaDataSourceGetAvailableSize) __INTRODUCED_IN(29);

#endif  /*__ANDROID_API__ >= 29 */

__END_DECLS

#endif // _NDK_MEDIA_DATASOURCE_H
