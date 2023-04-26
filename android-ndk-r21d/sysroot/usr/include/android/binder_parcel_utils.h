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
 * @file binder_parcel_utils.h
 * @brief A collection of helper wrappers for AParcel.
 */

#pragma once

#include <android/binder_auto_utils.h>
#include <android/binder_parcel.h>

#include <optional>
#include <string>
#include <vector>

namespace ndk {

/**
 * This retrieves and allocates a vector to size 'length' and returns the underlying buffer.
 */
template <typename T>
static inline bool AParcel_stdVectorAllocator(void* vectorData, int32_t length, T** outBuffer) {
    if (length < 0) return false;

    std::vector<T>* vec = static_cast<std::vector<T>*>(vectorData);
    if (static_cast<size_t>(length) > vec->max_size()) return false;

    vec->resize(length);
    *outBuffer = vec->data();
    return true;
}

/**
 * This retrieves and allocates a vector to size 'length' and returns the underlying buffer.
 */
template <typename T>
static inline bool AParcel_nullableStdVectorAllocator(void* vectorData, int32_t length,
                                                      T** outBuffer) {
    std::optional<std::vector<T>>* vec = static_cast<std::optional<std::vector<T>>*>(vectorData);

    if (length < 0) {
        *vec = std::nullopt;
        return true;
    }

    *vec = std::optional<std::vector<T>>(std::vector<T>{});

    if (static_cast<size_t>(length) > (*vec)->max_size()) return false;
    (*vec)->resize(length);

    *outBuffer = (*vec)->data();
    return true;
}

/**
 * This allocates a vector to size 'length' and returns whether the allocation is successful.
 *
 * See also AParcel_stdVectorAllocator. Types used with this allocator have their sizes defined
 * externally with respect to the NDK, and that size information is not passed into the NDK.
 * Instead, it is used in cases where callbacks are used. Note that when this allocator is used,
 * null arrays are not supported.
 *
 * See AParcel_readVector(const AParcel* parcel, std::vector<bool>)
 * See AParcel_readVector(const AParcel* parcel, std::vector<std::string>)
 */
template <typename T>
static inline bool AParcel_stdVectorExternalAllocator(void* vectorData, int32_t length) {
    if (length < 0) return false;

    std::vector<T>* vec = static_cast<std::vector<T>*>(vectorData);
    if (static_cast<size_t>(length) > vec->max_size()) return false;

    vec->resize(length);
    return true;
}

/**
 * This allocates a vector to size 'length' and returns whether the allocation is successful.
 *
 * See also AParcel_stdVectorAllocator. Types used with this allocator have their sizes defined
 * externally with respect to the NDK, and that size information is not passed into the NDK.
 * Instead, it is used in cases where callbacks are used. Note, when this allocator is used,
 * the vector itself can be nullable.
 *
 * See AParcel_readVector(const AParcel* parcel,
 * std::optional<std::vector<std::optional<std::string>>>)
 */
template <typename T>
static inline bool AParcel_nullableStdVectorExternalAllocator(void* vectorData, int32_t length) {
    std::optional<std::vector<T>>* vec = static_cast<std::optional<std::vector<T>>*>(vectorData);

    if (length < 0) {
        *vec = std::nullopt;
        return true;
    }

    *vec = std::optional<std::vector<T>>(std::vector<T>{});

    if (static_cast<size_t>(length) > (*vec)->max_size()) return false;
    (*vec)->resize(length);

    return true;
}

/**
 * This retrieves the underlying value in a vector which may not be contiguous at index from a
 * corresponding vectorData.
 */
template <typename T>
static inline T AParcel_stdVectorGetter(const void* vectorData, size_t index) {
    const std::vector<T>* vec = static_cast<const std::vector<T>*>(vectorData);
    return (*vec)[index];
}

/**
 * This sets the underlying value in a corresponding vectorData which may not be contiguous at
 * index.
 */
template <typename T>
static inline void AParcel_stdVectorSetter(void* vectorData, size_t index, T value) {
    std::vector<T>* vec = static_cast<std::vector<T>*>(vectorData);
    (*vec)[index] = value;
}

/**
 * This sets the underlying value in a corresponding vectorData which may not be contiguous at
 * index.
 */
template <typename T>
static inline void AParcel_nullableStdVectorSetter(void* vectorData, size_t index, T value) {
    std::optional<std::vector<T>>* vec = static_cast<std::optional<std::vector<T>>*>(vectorData);
    vec->value()[index] = value;
}

/**
 * Convenience method to write a nullable strong binder.
 */
static inline binder_status_t AParcel_writeNullableStrongBinder(AParcel* parcel,
                                                                const SpAIBinder& binder) {
    return AParcel_writeStrongBinder(parcel, binder.get());
}

/**
 * Convenience method to read a nullable strong binder.
 */
static inline binder_status_t AParcel_readNullableStrongBinder(const AParcel* parcel,
                                                               SpAIBinder* binder) {
    AIBinder* readBinder;
    binder_status_t status = AParcel_readStrongBinder(parcel, &readBinder);
    if (status == STATUS_OK) {
        binder->set(readBinder);
    }
    return status;
}

/**
 * Convenience method to write a strong binder but return an error if it is null.
 */
static inline binder_status_t AParcel_writeRequiredStrongBinder(AParcel* parcel,
                                                                const SpAIBinder& binder) {
    if (binder.get() == nullptr) {
        return STATUS_UNEXPECTED_NULL;
    }
    return AParcel_writeStrongBinder(parcel, binder.get());
}

/**
 * Convenience method to read a strong binder but return an error if it is null.
 */
static inline binder_status_t AParcel_readRequiredStrongBinder(const AParcel* parcel,
                                                               SpAIBinder* binder) {
    AIBinder* readBinder;
    binder_status_t ret = AParcel_readStrongBinder(parcel, &readBinder);
    if (ret == STATUS_OK) {
        if (readBinder == nullptr) {
            return STATUS_UNEXPECTED_NULL;
        }

        binder->set(readBinder);
    }
    return ret;
}

/**
 * Convenience method to write a ParcelFileDescriptor where -1 represents a null value.
 */
static inline binder_status_t AParcel_writeNullableParcelFileDescriptor(
        AParcel* parcel, const ScopedFileDescriptor& fd) {
    return AParcel_writeParcelFileDescriptor(parcel, fd.get());
}

/**
 * Convenience method to read a ParcelFileDescriptor where -1 represents a null value.
 */
static inline binder_status_t AParcel_readNullableParcelFileDescriptor(const AParcel* parcel,
                                                                       ScopedFileDescriptor* fd) {
    int readFd;
    binder_status_t status = AParcel_readParcelFileDescriptor(parcel, &readFd);
    if (status == STATUS_OK) {
        fd->set(readFd);
    }
    return status;
}

/**
 * Convenience method to write a valid ParcelFileDescriptor.
 */
static inline binder_status_t AParcel_writeRequiredParcelFileDescriptor(
        AParcel* parcel, const ScopedFileDescriptor& fd) {
    if (fd.get() < 0) {
        return STATUS_UNEXPECTED_NULL;
    }
    return AParcel_writeParcelFileDescriptor(parcel, fd.get());
}

/**
 * Convenience method to read a valid ParcelFileDescriptor.
 */
static inline binder_status_t AParcel_readRequiredParcelFileDescriptor(const AParcel* parcel,
                                                                       ScopedFileDescriptor* fd) {
    int readFd;
    binder_status_t status = AParcel_readParcelFileDescriptor(parcel, &readFd);
    if (status == STATUS_OK) {
        if (readFd < 0) {
            return STATUS_UNEXPECTED_NULL;
        }
        fd->set(readFd);
    }
    return status;
}

/**
 * Allocates a std::string to length and returns the underlying buffer. For use with
 * AParcel_readString. See use below in AParcel_readString(const AParcel*, std::string*).
 */
static inline bool AParcel_stdStringAllocator(void* stringData, int32_t length, char** buffer) {
    if (length <= 0) return false;

    std::string* str = static_cast<std::string*>(stringData);
    str->resize(length - 1);
    *buffer = &(*str)[0];
    return true;
}

/**
 * Allocates a string in a std::optional<std::string> to size 'length' (or to std::nullopt when
 * length is -1) and returns the underlying buffer. For use with AParcel_readString. See use below
 * in AParcel_readString(const AParcel*, std::optional<std::string>*).
 */
static inline bool AParcel_nullableStdStringAllocator(void* stringData, int32_t length,
                                                      char** buffer) {
    if (length == 0) return false;

    std::optional<std::string>* str = static_cast<std::optional<std::string>*>(stringData);

    if (length < 0) {
        *str = std::nullopt;
        return true;
    }

    *str = std::optional<std::string>(std::string{});
    (*str)->resize(length - 1);
    *buffer = &(**str)[0];
    return true;
}

/**
 * Allocates a std::string inside of a std::vector<std::string> at index 'index' to size 'length'.
 */
static inline bool AParcel_stdVectorStringElementAllocator(void* vectorData, size_t index,
                                                           int32_t length, char** buffer) {
    std::vector<std::string>* vec = static_cast<std::vector<std::string>*>(vectorData);
    std::string& element = vec->at(index);
    return AParcel_stdStringAllocator(static_cast<void*>(&element), length, buffer);
}

/**
 * This gets the length and buffer of a std::string inside of a std::vector<std::string> at index
 * index.
 */
static inline const char* AParcel_stdVectorStringElementGetter(const void* vectorData, size_t index,
                                                               int32_t* outLength) {
    const std::vector<std::string>* vec = static_cast<const std::vector<std::string>*>(vectorData);
    const std::string& element = vec->at(index);

    *outLength = element.size();
    return element.c_str();
}

/**
 * Allocates a string in a std::optional<std::string> inside of a
 * std::optional<std::vector<std::optional<std::string>>> at index 'index' to size 'length' (or to
 * std::nullopt when length is -1).
 */
static inline bool AParcel_nullableStdVectorStringElementAllocator(void* vectorData, size_t index,
                                                                   int32_t length, char** buffer) {
    std::optional<std::vector<std::optional<std::string>>>* vec =
            static_cast<std::optional<std::vector<std::optional<std::string>>>*>(vectorData);
    std::optional<std::string>& element = vec->value().at(index);
    return AParcel_nullableStdStringAllocator(static_cast<void*>(&element), length, buffer);
}

/**
 * This gets the length and buffer of a std::optional<std::string> inside of a
 * std::vector<std::string> at index index. If the string is null, then it returns null and a length
 * of -1.
 */
static inline const char* AParcel_nullableStdVectorStringElementGetter(const void* vectorData,
                                                                       size_t index,
                                                                       int32_t* outLength) {
    const std::optional<std::vector<std::optional<std::string>>>* vec =
            static_cast<const std::optional<std::vector<std::optional<std::string>>>*>(vectorData);
    const std::optional<std::string>& element = vec->value().at(index);

    if (!element) {
        *outLength = -1;
        return nullptr;
    }

    *outLength = element->size();
    return element->c_str();
}

/**
 * Convenience API for writing a std::string.
 */
static inline binder_status_t AParcel_writeString(AParcel* parcel, const std::string& str) {
    return AParcel_writeString(parcel, str.c_str(), str.size());
}

/**
 * Convenience API for reading a std::string.
 */
static inline binder_status_t AParcel_readString(const AParcel* parcel, std::string* str) {
    void* stringData = static_cast<void*>(str);
    return AParcel_readString(parcel, stringData, AParcel_stdStringAllocator);
}

/**
 * Convenience API for writing a std::optional<std::string>.
 */
static inline binder_status_t AParcel_writeString(AParcel* parcel,
                                                  const std::optional<std::string>& str) {
    if (!str) {
        return AParcel_writeString(parcel, nullptr, -1);
    }

    return AParcel_writeString(parcel, str->c_str(), str->size());
}

/**
 * Convenience API for reading a std::optional<std::string>.
 */
static inline binder_status_t AParcel_readString(const AParcel* parcel,
                                                 std::optional<std::string>* str) {
    void* stringData = static_cast<void*>(str);
    return AParcel_readString(parcel, stringData, AParcel_nullableStdStringAllocator);
}

/**
 * Convenience API for writing a std::vector<std::string>
 */
static inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                                  const std::vector<std::string>& vec) {
    const void* vectorData = static_cast<const void*>(&vec);
    return AParcel_writeStringArray(parcel, vectorData, vec.size(),
                                    AParcel_stdVectorStringElementGetter);
}

/**
 * Convenience API for reading a std::vector<std::string>
 */
static inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                                 std::vector<std::string>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readStringArray(parcel, vectorData,
                                   AParcel_stdVectorExternalAllocator<std::string>,
                                   AParcel_stdVectorStringElementAllocator);
}

/**
 * Convenience API for writing a std::optional<std::vector<std::optional<std::string>>>
 */
static inline binder_status_t AParcel_writeVector(
        AParcel* parcel, const std::optional<std::vector<std::optional<std::string>>>& vec) {
    const void* vectorData = static_cast<const void*>(&vec);
    return AParcel_writeStringArray(parcel, vectorData, (vec ? vec->size() : -1),
                                    AParcel_nullableStdVectorStringElementGetter);
}

/**
 * Convenience API for reading a std::optional<std::vector<std::optional<std::string>>>
 */
static inline binder_status_t AParcel_readVector(
        const AParcel* parcel, std::optional<std::vector<std::optional<std::string>>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readStringArray(
            parcel, vectorData,
            AParcel_nullableStdVectorExternalAllocator<std::optional<std::string>>,
            AParcel_nullableStdVectorStringElementAllocator);
}

/**
 * Convenience API for writing a non-null parcelable.
 */
template <typename P>
static inline binder_status_t AParcel_writeParcelable(AParcel* parcel, const P& p) {
    binder_status_t status = AParcel_writeInt32(parcel, 1);  // non-null
    if (status != STATUS_OK) {
        return status;
    }
    return p.writeToParcel(parcel);
}

/**
 * Convenience API for reading a non-null parcelable.
 */
template <typename P>
static inline binder_status_t AParcel_readParcelable(const AParcel* parcel, P* p) {
    int32_t null;
    binder_status_t status = AParcel_readInt32(parcel, &null);
    if (status != STATUS_OK) {
        return status;
    }
    if (null == 0) {
        return STATUS_UNEXPECTED_NULL;
    }
    return p->readFromParcel(parcel);
}

/**
 * Convenience API for writing a nullable parcelable.
 */
template <typename P>
static inline binder_status_t AParcel_writeNullableParcelable(AParcel* parcel,
                                                              const std::optional<P>& p) {
    if (p == std::nullopt) {
        return AParcel_writeInt32(parcel, 0);  // null
    }
    binder_status_t status = AParcel_writeInt32(parcel, 1);  // non-null
    if (status != STATUS_OK) {
        return status;
    }
    return p->writeToParcel(parcel);
}

/**
 * Convenience API for reading a nullable parcelable.
 */
template <typename P>
static inline binder_status_t AParcel_readNullableParcelable(const AParcel* parcel,
                                                             std::optional<P>* p) {
    int32_t null;
    binder_status_t status = AParcel_readInt32(parcel, &null);
    if (status != STATUS_OK) {
        return status;
    }
    if (null == 0) {
        *p = std::nullopt;
        return STATUS_OK;
    }
    *p = std::optional<P>(P{});
    return (*p)->readFromParcel(parcel);
}

/**
 * Writes a parcelable object of type P inside a std::vector<P> at index 'index' to 'parcel'.
 */
template <typename P>
binder_status_t AParcel_writeStdVectorParcelableElement(AParcel* parcel, const void* vectorData,
                                                        size_t index) {
    const std::vector<P>* vector = static_cast<const std::vector<P>*>(vectorData);
    return AParcel_writeParcelable(parcel, vector->at(index));
}

/**
 * Reads a parcelable object of type P inside a std::vector<P> at index 'index' from 'parcel'.
 */
template <typename P>
binder_status_t AParcel_readStdVectorParcelableElement(const AParcel* parcel, void* vectorData,
                                                       size_t index) {
    std::vector<P>* vector = static_cast<std::vector<P>*>(vectorData);
    return AParcel_readParcelable(parcel, &vector->at(index));
}

/**
 * Writes a ScopedFileDescriptor object inside a std::vector<ScopedFileDescriptor> at index 'index'
 * to 'parcel'.
 */
template <>
inline binder_status_t AParcel_writeStdVectorParcelableElement<ScopedFileDescriptor>(
        AParcel* parcel, const void* vectorData, size_t index) {
    const std::vector<ScopedFileDescriptor>* vector =
            static_cast<const std::vector<ScopedFileDescriptor>*>(vectorData);
    int writeFd = vector->at(index).get();
    if (writeFd < 0) {
        return STATUS_UNEXPECTED_NULL;
    }
    return AParcel_writeParcelFileDescriptor(parcel, writeFd);
}

/**
 * Reads a ScopedFileDescriptor object inside a std::vector<ScopedFileDescriptor> at index 'index'
 * from 'parcel'.
 */
template <>
inline binder_status_t AParcel_readStdVectorParcelableElement<ScopedFileDescriptor>(
        const AParcel* parcel, void* vectorData, size_t index) {
    std::vector<ScopedFileDescriptor>* vector =
            static_cast<std::vector<ScopedFileDescriptor>*>(vectorData);
    int readFd;
    binder_status_t status = AParcel_readParcelFileDescriptor(parcel, &readFd);
    if (status == STATUS_OK) {
        if (readFd < 0) {
            return STATUS_UNEXPECTED_NULL;
        }
        vector->at(index).set(readFd);
    }
    return status;
}

/**
 * Convenience API for writing a std::vector<P>
 */
template <typename P>
static inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<P>& vec) {
    const void* vectorData = static_cast<const void*>(&vec);
    return AParcel_writeParcelableArray(parcel, vectorData, vec.size(),
                                        AParcel_writeStdVectorParcelableElement<P>);
}

/**
 * Convenience API for reading a std::vector<P>
 */
template <typename P>
static inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<P>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readParcelableArray(parcel, vectorData, AParcel_stdVectorExternalAllocator<P>,
                                       AParcel_readStdVectorParcelableElement<P>);
}

// @START
/**
 * Writes a vector of int32_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<int32_t>& vec) {
    return AParcel_writeInt32Array(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of int32_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<int32_t>>& vec) {
    if (!vec) return AParcel_writeInt32Array(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of int32_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<int32_t>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readInt32Array(parcel, vectorData, AParcel_stdVectorAllocator<int32_t>);
}

/**
 * Reads an optional vector of int32_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<int32_t>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readInt32Array(parcel, vectorData, AParcel_nullableStdVectorAllocator<int32_t>);
}

/**
 * Writes a vector of uint32_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<uint32_t>& vec) {
    return AParcel_writeUint32Array(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of uint32_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<uint32_t>>& vec) {
    if (!vec) return AParcel_writeUint32Array(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of uint32_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<uint32_t>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readUint32Array(parcel, vectorData, AParcel_stdVectorAllocator<uint32_t>);
}

/**
 * Reads an optional vector of uint32_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<uint32_t>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readUint32Array(parcel, vectorData,
                                   AParcel_nullableStdVectorAllocator<uint32_t>);
}

/**
 * Writes a vector of int64_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<int64_t>& vec) {
    return AParcel_writeInt64Array(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of int64_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<int64_t>>& vec) {
    if (!vec) return AParcel_writeInt64Array(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of int64_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<int64_t>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readInt64Array(parcel, vectorData, AParcel_stdVectorAllocator<int64_t>);
}

/**
 * Reads an optional vector of int64_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<int64_t>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readInt64Array(parcel, vectorData, AParcel_nullableStdVectorAllocator<int64_t>);
}

/**
 * Writes a vector of uint64_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<uint64_t>& vec) {
    return AParcel_writeUint64Array(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of uint64_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<uint64_t>>& vec) {
    if (!vec) return AParcel_writeUint64Array(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of uint64_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<uint64_t>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readUint64Array(parcel, vectorData, AParcel_stdVectorAllocator<uint64_t>);
}

/**
 * Reads an optional vector of uint64_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<uint64_t>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readUint64Array(parcel, vectorData,
                                   AParcel_nullableStdVectorAllocator<uint64_t>);
}

/**
 * Writes a vector of float to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<float>& vec) {
    return AParcel_writeFloatArray(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of float to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<float>>& vec) {
    if (!vec) return AParcel_writeFloatArray(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of float from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<float>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readFloatArray(parcel, vectorData, AParcel_stdVectorAllocator<float>);
}

/**
 * Reads an optional vector of float from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<float>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readFloatArray(parcel, vectorData, AParcel_nullableStdVectorAllocator<float>);
}

/**
 * Writes a vector of double to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<double>& vec) {
    return AParcel_writeDoubleArray(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of double to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<double>>& vec) {
    if (!vec) return AParcel_writeDoubleArray(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of double from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<double>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readDoubleArray(parcel, vectorData, AParcel_stdVectorAllocator<double>);
}

/**
 * Reads an optional vector of double from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<double>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readDoubleArray(parcel, vectorData, AParcel_nullableStdVectorAllocator<double>);
}

/**
 * Writes a vector of bool to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<bool>& vec) {
    return AParcel_writeBoolArray(parcel, static_cast<const void*>(&vec), vec.size(),
                                  AParcel_stdVectorGetter<bool>);
}

/**
 * Writes an optional vector of bool to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<bool>>& vec) {
    if (!vec) return AParcel_writeBoolArray(parcel, nullptr, -1, AParcel_stdVectorGetter<bool>);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of bool from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<bool>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readBoolArray(parcel, vectorData, AParcel_stdVectorExternalAllocator<bool>,
                                 AParcel_stdVectorSetter<bool>);
}

/**
 * Reads an optional vector of bool from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<bool>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readBoolArray(parcel, vectorData,
                                 AParcel_nullableStdVectorExternalAllocator<bool>,
                                 AParcel_nullableStdVectorSetter<bool>);
}

/**
 * Writes a vector of char16_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<char16_t>& vec) {
    return AParcel_writeCharArray(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of char16_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<char16_t>>& vec) {
    if (!vec) return AParcel_writeCharArray(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of char16_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<char16_t>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readCharArray(parcel, vectorData, AParcel_stdVectorAllocator<char16_t>);
}

/**
 * Reads an optional vector of char16_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<char16_t>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readCharArray(parcel, vectorData, AParcel_nullableStdVectorAllocator<char16_t>);
}

/**
 * Writes a vector of int8_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel, const std::vector<int8_t>& vec) {
    return AParcel_writeByteArray(parcel, vec.data(), vec.size());
}

/**
 * Writes an optional vector of int8_t to the next location in a non-null parcel.
 */
inline binder_status_t AParcel_writeVector(AParcel* parcel,
                                           const std::optional<std::vector<int8_t>>& vec) {
    if (!vec) return AParcel_writeByteArray(parcel, nullptr, -1);
    return AParcel_writeVector(parcel, *vec);
}

/**
 * Reads a vector of int8_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel, std::vector<int8_t>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readByteArray(parcel, vectorData, AParcel_stdVectorAllocator<int8_t>);
}

/**
 * Reads an optional vector of int8_t from the next location in a non-null parcel.
 */
inline binder_status_t AParcel_readVector(const AParcel* parcel,
                                          std::optional<std::vector<int8_t>>* vec) {
    void* vectorData = static_cast<void*>(vec);
    return AParcel_readByteArray(parcel, vectorData, AParcel_nullableStdVectorAllocator<int8_t>);
}

// @END

/**
 * Convenience API for writing the size of a vector.
 */
template <typename T>
static inline binder_status_t AParcel_writeVectorSize(AParcel* parcel, const std::vector<T>& vec) {
    if (vec.size() > INT32_MAX) {
        return STATUS_BAD_VALUE;
    }

    return AParcel_writeInt32(parcel, static_cast<int32_t>(vec.size()));
}

/**
 * Convenience API for writing the size of a vector.
 */
template <typename T>
static inline binder_status_t AParcel_writeVectorSize(AParcel* parcel,
                                                      const std::optional<std::vector<T>>& vec) {
    if (!vec) {
        return AParcel_writeInt32(parcel, -1);
    }

    if (vec->size() > INT32_MAX) {
        return STATUS_BAD_VALUE;
    }

    return AParcel_writeInt32(parcel, static_cast<int32_t>(vec->size()));
}

/**
 * Convenience API for resizing a vector.
 */
template <typename T>
static inline binder_status_t AParcel_resizeVector(const AParcel* parcel, std::vector<T>* vec) {
    int32_t size;
    binder_status_t err = AParcel_readInt32(parcel, &size);

    if (err != STATUS_OK) return err;
    if (size < 0) return STATUS_UNEXPECTED_NULL;

    vec->resize(static_cast<size_t>(size));
    return STATUS_OK;
}

/**
 * Convenience API for resizing a vector.
 */
template <typename T>
static inline binder_status_t AParcel_resizeVector(const AParcel* parcel,
                                                   std::optional<std::vector<T>>* vec) {
    int32_t size;
    binder_status_t err = AParcel_readInt32(parcel, &size);

    if (err != STATUS_OK) return err;
    if (size < -1) return STATUS_UNEXPECTED_NULL;

    if (size == -1) {
        *vec = std::nullopt;
        return STATUS_OK;
    }

    *vec = std::optional<std::vector<T>>(std::vector<T>{});
    (*vec)->resize(static_cast<size_t>(size));
    return STATUS_OK;
}

}  // namespace ndk

/** @} */
