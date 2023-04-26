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
 * @file binder_auto_utils.h
 * @brief These objects provide a more C++-like thin interface to the binder.
 */

#pragma once

#include <android/binder_ibinder.h>
#include <android/binder_parcel.h>
#include <android/binder_status.h>

#include <assert.h>

#include <unistd.h>
#include <cstddef>
#include <string>

namespace ndk {

/**
 * Represents one strong pointer to an AIBinder object.
 */
class SpAIBinder {
   public:
    /**
     * Takes ownership of one strong refcount of binder.
     */
    explicit SpAIBinder(AIBinder* binder = nullptr) : mBinder(binder) {}

    /**
     * Convenience operator for implicitly constructing an SpAIBinder from nullptr. This is not
     * explicit because it is not taking ownership of anything.
     */
    SpAIBinder(std::nullptr_t) : SpAIBinder() {}  // NOLINT(google-explicit-constructor)

    /**
     * This will delete the underlying object if it exists. See operator=.
     */
    SpAIBinder(const SpAIBinder& other) { *this = other; }

    /**
     * This deletes the underlying object if it exists. See set.
     */
    ~SpAIBinder() { set(nullptr); }

    /**
     * This takes ownership of a binder from another AIBinder object but it does not affect the
     * ownership of that other object.
     */
    SpAIBinder& operator=(const SpAIBinder& other) {
        AIBinder_incStrong(other.mBinder);
        set(other.mBinder);
        return *this;
    }

    /**
     * Takes ownership of one strong refcount of binder
     */
    void set(AIBinder* binder) {
        AIBinder* old = *const_cast<AIBinder* volatile*>(&mBinder);
        if (old != nullptr) AIBinder_decStrong(old);
        if (old != *const_cast<AIBinder* volatile*>(&mBinder)) {
            __assert(__FILE__, __LINE__, "Race detected.");
        }
        mBinder = binder;
    }

    /**
     * This returns the underlying binder object for transactions. If it is used to create another
     * SpAIBinder object, it should first be incremented.
     */
    AIBinder* get() const { return mBinder; }

    /**
     * This allows the value in this class to be set from beneath it. If you call this method and
     * then change the value of T*, you must take ownership of the value you are replacing and add
     * ownership to the object that is put in here.
     *
     * Recommended use is like this:
     *   SpAIBinder a;  // will be nullptr
     *   SomeInitFunction(a.getR());  // value is initialized with refcount
     *
     * Other usecases are discouraged.
     *
     */
    AIBinder** getR() { return &mBinder; }

   private:
    AIBinder* mBinder = nullptr;
};

namespace impl {

/**
 * This baseclass owns a single object, used to make various classes RAII.
 */
template <typename T, typename R, R (*Destroy)(T), T DEFAULT>
class ScopedAResource {
   public:
    /**
     * Takes ownership of t.
     */
    explicit ScopedAResource(T t = DEFAULT) : mT(t) {}

    /**
     * This deletes the underlying object if it exists. See set.
     */
    ~ScopedAResource() { set(DEFAULT); }

    /**
     * Takes ownership of t.
     */
    void set(T t) {
        Destroy(mT);
        mT = t;
    }

    /**
     * This returns the underlying object to be modified but does not affect ownership.
     */
    T get() { return mT; }

    /**
     * This returns the const underlying object but does not affect ownership.
     */
    const T get() const { return mT; }

    /**
     * This allows the value in this class to be set from beneath it. If you call this method and
     * then change the value of T*, you must take ownership of the value you are replacing and add
     * ownership to the object that is put in here.
     *
     * Recommended use is like this:
     *   ScopedAResource<T> a; // will be nullptr
     *   SomeInitFunction(a.getR()); // value is initialized with refcount
     *
     * Other usecases are discouraged.
     *
     */
    T* getR() { return &mT; }

    // copy-constructing/assignment is disallowed
    ScopedAResource(const ScopedAResource&) = delete;
    ScopedAResource& operator=(const ScopedAResource&) = delete;

    // move-constructing/assignment is okay
    ScopedAResource(ScopedAResource&& other) : mT(std::move(other.mT)) { other.mT = DEFAULT; }
    ScopedAResource& operator=(ScopedAResource&& other) {
        set(other.mT);
        other.mT = DEFAULT;
        return *this;
    }

   private:
    T mT;
};

}  // namespace impl

/**
 * Convenience wrapper. See AParcel.
 */
class ScopedAParcel : public impl::ScopedAResource<AParcel*, void, AParcel_delete, nullptr> {
   public:
    /**
     * Takes ownership of a.
     */
    explicit ScopedAParcel(AParcel* a = nullptr) : ScopedAResource(a) {}
    ~ScopedAParcel() {}
    ScopedAParcel(ScopedAParcel&&) = default;
};

/**
 * Convenience wrapper. See AStatus.
 */
class ScopedAStatus : public impl::ScopedAResource<AStatus*, void, AStatus_delete, nullptr> {
   public:
    /**
     * Takes ownership of a.
     */
    explicit ScopedAStatus(AStatus* a = nullptr) : ScopedAResource(a) {}
    ~ScopedAStatus() {}
    ScopedAStatus(ScopedAStatus&&) = default;
    ScopedAStatus& operator=(ScopedAStatus&&) = default;

    /**
     * See AStatus_isOk.
     */
    bool isOk() const { return get() != nullptr && AStatus_isOk(get()); }

    /**
     * See AStatus_getExceptionCode
     */
    binder_exception_t getExceptionCode() const { return AStatus_getExceptionCode(get()); }

    /**
     * See AStatus_getServiceSpecificError
     */
    int32_t getServiceSpecificError() const { return AStatus_getServiceSpecificError(get()); }

    /**
     * See AStatus_getStatus
     */
    binder_status_t getStatus() const { return AStatus_getStatus(get()); }

    /**
     * See AStatus_getMessage
     */
    const char* getMessage() const { return AStatus_getMessage(get()); }

    std::string getDescription() const {
        const char* cStr = AStatus_getDescription(get());
        std::string ret = cStr;
        AStatus_deleteDescription(cStr);
        return ret;
    }

    /**
     * Convenience methods for creating scoped statuses.
     */
    static ScopedAStatus ok() { return ScopedAStatus(AStatus_newOk()); }
    static ScopedAStatus fromExceptionCode(binder_exception_t exception) {
        return ScopedAStatus(AStatus_fromExceptionCode(exception));
    }
    static ScopedAStatus fromExceptionCodeWithMessage(binder_exception_t exception,
                                                      const char* message) {
        return ScopedAStatus(AStatus_fromExceptionCodeWithMessage(exception, message));
    }
    static ScopedAStatus fromServiceSpecificError(int32_t serviceSpecific) {
        return ScopedAStatus(AStatus_fromServiceSpecificError(serviceSpecific));
    }
    static ScopedAStatus fromServiceSpecificErrorWithMessage(int32_t serviceSpecific,
                                                             const char* message) {
        return ScopedAStatus(AStatus_fromServiceSpecificErrorWithMessage(serviceSpecific, message));
    }
    static ScopedAStatus fromStatus(binder_status_t status) {
        return ScopedAStatus(AStatus_fromStatus(status));
    }
};

/**
 * Convenience wrapper. See AIBinder_DeathRecipient.
 */
class ScopedAIBinder_DeathRecipient
    : public impl::ScopedAResource<AIBinder_DeathRecipient*, void, AIBinder_DeathRecipient_delete,
                                   nullptr> {
   public:
    /**
     * Takes ownership of a.
     */
    explicit ScopedAIBinder_DeathRecipient(AIBinder_DeathRecipient* a = nullptr)
        : ScopedAResource(a) {}
    ~ScopedAIBinder_DeathRecipient() {}
    ScopedAIBinder_DeathRecipient(ScopedAIBinder_DeathRecipient&&) = default;
};

/**
 * Convenience wrapper. See AIBinder_Weak.
 */
class ScopedAIBinder_Weak
    : public impl::ScopedAResource<AIBinder_Weak*, void, AIBinder_Weak_delete, nullptr> {
   public:
    /**
     * Takes ownership of a.
     */
    explicit ScopedAIBinder_Weak(AIBinder_Weak* a = nullptr) : ScopedAResource(a) {}
    ~ScopedAIBinder_Weak() {}
    ScopedAIBinder_Weak(ScopedAIBinder_Weak&&) = default;

    /**
     * See AIBinder_Weak_promote.
     */
    SpAIBinder promote() { return SpAIBinder(AIBinder_Weak_promote(get())); }
};

/**
 * Convenience wrapper for a file descriptor.
 */
class ScopedFileDescriptor : public impl::ScopedAResource<int, int, close, -1> {
   public:
    /**
     * Takes ownership of a.
     */
    explicit ScopedFileDescriptor(int a = -1) : ScopedAResource(a) {}
    ~ScopedFileDescriptor() {}
    ScopedFileDescriptor(ScopedFileDescriptor&&) = default;
};

}  // namespace ndk

/** @} */
