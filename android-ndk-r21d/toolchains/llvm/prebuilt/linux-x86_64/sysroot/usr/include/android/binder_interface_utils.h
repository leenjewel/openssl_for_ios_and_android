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
 * @file binder_interface_utils.h
 * @brief This provides common C++ classes for common operations and as base classes for C++
 * interfaces.
 */

#pragma once

#include <android/binder_auto_utils.h>
#include <android/binder_ibinder.h>

#if __has_include(<android/binder_shell.h>)
#include <android/binder_shell.h>
#define HAS_BINDER_SHELL_COMMAND
#endif  //_has_include

#include <assert.h>

#include <memory>
#include <mutex>

namespace ndk {

/**
 * analog using std::shared_ptr for internally held refcount
 *
 * ref must be called at least one time during the lifetime of this object. The recommended way to
 * construct this object is with SharedRefBase::make.
 */
class SharedRefBase {
   public:
    SharedRefBase() {}
    virtual ~SharedRefBase() {
        std::call_once(mFlagThis, [&]() {
            __assert(__FILE__, __LINE__, "SharedRefBase: no ref created during lifetime");
        });
    }

    /**
     * A shared_ptr must be held to this object when this is called. This must be called once during
     * the lifetime of this object.
     */
    std::shared_ptr<SharedRefBase> ref() {
        std::shared_ptr<SharedRefBase> thiz = mThis.lock();

        std::call_once(mFlagThis, [&]() { mThis = thiz = std::shared_ptr<SharedRefBase>(this); });

        return thiz;
    }

    /**
     * Convenience method for a ref (see above) which automatically casts to the desired child type.
     */
    template <typename CHILD>
    std::shared_ptr<CHILD> ref() {
        return std::static_pointer_cast<CHILD>(ref());
    }

    /**
     * Convenience method for making an object directly with a reference.
     */
    template <class T, class... Args>
    static std::shared_ptr<T> make(Args&&... args) {
        T* t = new T(std::forward<Args>(args)...);
        return t->template ref<T>();
    }

    static void operator delete(void* p) { std::free(p); }

   private:
    std::once_flag mFlagThis;
    std::weak_ptr<SharedRefBase> mThis;

    // Use 'SharedRefBase::make<T>(...)' to make. SharedRefBase has implicit
    // ownership. Making this operator private to avoid double-ownership.
    static void* operator new(size_t s) { return std::malloc(s); }
};

/**
 * wrapper analog to IInterface
 */
class ICInterface : public SharedRefBase {
   public:
    ICInterface() {}
    virtual ~ICInterface() {}

    /**
     * This either returns the single existing implementation or creates a new implementation.
     */
    virtual SpAIBinder asBinder() = 0;

    /**
     * Returns whether this interface is in a remote process. If it cannot be determined locally,
     * this will be checked using AIBinder_isRemote.
     */
    virtual bool isRemote() = 0;

    /**
     * Dumps information about the interface. By default, dumps nothing.
     */
    virtual inline binder_status_t dump(int fd, const char** args, uint32_t numArgs);

#ifdef HAS_BINDER_SHELL_COMMAND
    /**
     * Process shell commands. By default, does nothing.
     */
    virtual inline binder_status_t handleShellCommand(int in, int out, int err, const char** argv,
                                                      uint32_t argc);
#endif

    /**
     * Interprets this binder as this underlying interface if this has stored an ICInterface in the
     * binder's user data.
     *
     * This does not do type checking and should only be used when the binder is known to originate
     * from ICInterface. Most likely, you want to use I*::fromBinder.
     */
    static inline std::shared_ptr<ICInterface> asInterface(AIBinder* binder);

    /**
     * Helper method to create a class
     */
    static inline AIBinder_Class* defineClass(const char* interfaceDescriptor,
                                              AIBinder_Class_onTransact onTransact);

   private:
    class ICInterfaceData {
       public:
        std::shared_ptr<ICInterface> interface;

        static inline std::shared_ptr<ICInterface> getInterface(AIBinder* binder);

        static inline void* onCreate(void* args);
        static inline void onDestroy(void* userData);
        static inline binder_status_t onDump(AIBinder* binder, int fd, const char** args,
                                             uint32_t numArgs);

#ifdef HAS_BINDER_SHELL_COMMAND
        static inline binder_status_t handleShellCommand(AIBinder* binder, int in, int out, int err,
                                                         const char** argv, uint32_t argc);
#endif
    };
};

/**
 * implementation of IInterface for server (n = native)
 */
template <typename INTERFACE>
class BnCInterface : public INTERFACE {
   public:
    BnCInterface() {}
    virtual ~BnCInterface() {}

    SpAIBinder asBinder() override;

    bool isRemote() override { return false; }

   protected:
    /**
     * This function should only be called by asBinder. Otherwise, there is a possibility of
     * multiple AIBinder* objects being created for the same instance of an object.
     */
    virtual SpAIBinder createBinder() = 0;

   private:
    std::mutex mMutex;  // for asBinder
    ScopedAIBinder_Weak mWeakBinder;
};

/**
 * implementation of IInterface for client (p = proxy)
 */
template <typename INTERFACE>
class BpCInterface : public INTERFACE {
   public:
    explicit BpCInterface(const SpAIBinder& binder) : mBinder(binder) {}
    virtual ~BpCInterface() {}

    SpAIBinder asBinder() override;

    bool isRemote() override { return AIBinder_isRemote(mBinder.get()); }

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override {
        return AIBinder_dump(asBinder().get(), fd, args, numArgs);
    }

   private:
    SpAIBinder mBinder;
};

// END OF CLASS DECLARATIONS

binder_status_t ICInterface::dump(int /*fd*/, const char** /*args*/, uint32_t /*numArgs*/) {
    return STATUS_OK;
}

#ifdef HAS_BINDER_SHELL_COMMAND
binder_status_t ICInterface::handleShellCommand(int /*in*/, int /*out*/, int /*err*/,
                                                const char** /*argv*/, uint32_t /*argc*/) {
    return STATUS_OK;
}
#endif

std::shared_ptr<ICInterface> ICInterface::asInterface(AIBinder* binder) {
    return ICInterfaceData::getInterface(binder);
}

AIBinder_Class* ICInterface::defineClass(const char* interfaceDescriptor,
                                         AIBinder_Class_onTransact onTransact) {
    AIBinder_Class* clazz = AIBinder_Class_define(interfaceDescriptor, ICInterfaceData::onCreate,
                                                  ICInterfaceData::onDestroy, onTransact);
    if (clazz == nullptr) {
        return nullptr;
    }

    // We can't know if these methods are overridden by a subclass interface, so we must register
    // ourselves. The defaults are harmless.
    AIBinder_Class_setOnDump(clazz, ICInterfaceData::onDump);
#ifdef HAS_BINDER_SHELL_COMMAND
    if (AIBinder_Class_setHandleShellCommand != nullptr) {
        AIBinder_Class_setHandleShellCommand(clazz, ICInterfaceData::handleShellCommand);
    }
#endif
    return clazz;
}

std::shared_ptr<ICInterface> ICInterface::ICInterfaceData::getInterface(AIBinder* binder) {
    if (binder == nullptr) return nullptr;

    void* userData = AIBinder_getUserData(binder);
    if (userData == nullptr) return nullptr;

    return static_cast<ICInterfaceData*>(userData)->interface;
}

void* ICInterface::ICInterfaceData::onCreate(void* args) {
    std::shared_ptr<ICInterface> interface = static_cast<ICInterface*>(args)->ref<ICInterface>();
    ICInterfaceData* data = new ICInterfaceData{interface};
    return static_cast<void*>(data);
}

void ICInterface::ICInterfaceData::onDestroy(void* userData) {
    delete static_cast<ICInterfaceData*>(userData);
}

binder_status_t ICInterface::ICInterfaceData::onDump(AIBinder* binder, int fd, const char** args,
                                                     uint32_t numArgs) {
    std::shared_ptr<ICInterface> interface = getInterface(binder);
    return interface->dump(fd, args, numArgs);
}

#ifdef HAS_BINDER_SHELL_COMMAND
binder_status_t ICInterface::ICInterfaceData::handleShellCommand(AIBinder* binder, int in, int out,
                                                                 int err, const char** argv,
                                                                 uint32_t argc) {
    std::shared_ptr<ICInterface> interface = getInterface(binder);
    return interface->handleShellCommand(in, out, err, argv, argc);
}
#endif

template <typename INTERFACE>
SpAIBinder BnCInterface<INTERFACE>::asBinder() {
    std::lock_guard<std::mutex> l(mMutex);

    SpAIBinder binder;
    if (mWeakBinder.get() != nullptr) {
        binder.set(AIBinder_Weak_promote(mWeakBinder.get()));
    }
    if (binder.get() == nullptr) {
        binder = createBinder();
        mWeakBinder.set(AIBinder_Weak_new(binder.get()));
    }

    return binder;
}

template <typename INTERFACE>
SpAIBinder BpCInterface<INTERFACE>::asBinder() {
    return mBinder;
}

}  // namespace ndk

/** @} */
