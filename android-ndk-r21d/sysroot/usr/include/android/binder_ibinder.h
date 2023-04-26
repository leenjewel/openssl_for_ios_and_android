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
 * @file binder_ibinder.h
 * @brief Object which can receive transactions and be sent across processes.
 */

#pragma once

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <android/binder_parcel.h>
#include <android/binder_status.h>

__BEGIN_DECLS

#ifndef __ANDROID_API__
#error Android builds must be compiled against a specific API. If this is an \
 android platform host build, you must use libbinder_ndk_host_user.
#endif

#if __ANDROID_API__ >= 29

// Also see TF_* in kernel's binder.h
typedef uint32_t binder_flags_t;
enum {
    /**
     * The transaction will be dispatched and then returned to the caller. The outgoing process
     * cannot block a call made by this, and execution of the call will not be waited on. An error
     * can still be returned if the call is unable to be processed by the binder driver. All oneway
     * calls are guaranteed to be ordered if they are sent on the same AIBinder object.
     */
    FLAG_ONEWAY = 0x01,
};

// Also see IBinder.h in libbinder
typedef uint32_t transaction_code_t;
enum {
    /**
     * The first transaction code available for user commands (inclusive).
     */
    FIRST_CALL_TRANSACTION = 0x00000001,
    /**
     * The last transaction code available for user commands (inclusive).
     */
    LAST_CALL_TRANSACTION = 0x00ffffff,
};

/**
 * Represents a type of AIBinder object which can be sent out.
 */
struct AIBinder_Class;
typedef struct AIBinder_Class AIBinder_Class;

/**
 * Represents a local or remote object which can be used for IPC or which can itself be sent.
 *
 * This object has a refcount associated with it and will be deleted when its refcount reaches zero.
 * How methods interactive with this refcount is described below. When using this API, it is
 * intended for a client of a service to hold a strong reference to that service. This also means
 * that user data typically should hold a strong reference to a local AIBinder object. A remote
 * AIBinder object automatically holds a strong reference to the AIBinder object in the server's
 * process. A typically memory layout looks like this:
 *
 * Key:
 *   --->         Ownership/a strong reference
 *   ...>         A weak reference
 *
 *                         (process boundary)
 *                                 |
 * MyInterface ---> AIBinder_Weak  |  ProxyForMyInterface
 *      ^                .         |          |
 *      |                .         |          |
 *      |                v         |          v
 *   UserData  <---   AIBinder   <-|-      AIBinder
 *                                 |
 *
 * In this way, you'll notice that a proxy for the interface holds a strong reference to the
 * implementation and that in the server process, the AIBinder object which was sent can be resent
 * so that the same AIBinder object always represents the same object. This allows, for instance, an
 * implementation (usually a callback) to transfer all ownership to a remote process and
 * automatically be deleted when the remote process is done with it or dies. Other memory models are
 * possible, but this is the standard one.
 *
 * If the process containing an AIBinder dies, it is possible to be holding a strong reference to
 * an object which does not exist. In this case, transactions to this binder will return
 * STATUS_DEAD_OBJECT. See also AIBinder_linkToDeath, AIBinder_unlinkToDeath, and AIBinder_isAlive.
 *
 * Once an AIBinder is created, anywhere it is passed (remotely or locally), there is a 1-1
 * correspondence between the address of an AIBinder and the object it represents. This means that
 * when two AIBinder pointers point to the same address, they represent the same object (whether
 * that object is local or remote). This correspondance can be broken accidentally if AIBinder_new
 * is erronesouly called to create the same object multiple times.
 */
struct AIBinder;
typedef struct AIBinder AIBinder;

/**
 * The AIBinder object associated with this can be retrieved if it is still alive so that it can be
 * re-used. The intention of this is to enable the same AIBinder object to always represent the same
 * object.
 */
struct AIBinder_Weak;
typedef struct AIBinder_Weak AIBinder_Weak;

/**
 * Represents a handle on a death notification. See AIBinder_linkToDeath/AIBinder_unlinkToDeath.
 */
struct AIBinder_DeathRecipient;
typedef struct AIBinder_DeathRecipient AIBinder_DeathRecipient;

/**
 * This is called whenever a new AIBinder object is needed of a specific class.
 *
 * \param args these can be used to construct a new class. These are passed from AIBinder_new.
 * \return this is the userdata representing the class. It can be retrieved using
 * AIBinder_getUserData.
 */
typedef void* (*AIBinder_Class_onCreate)(void* args);

/**
 * This is called whenever an AIBinder object is no longer referenced and needs destroyed.
 *
 * Typically, this just deletes whatever the implementation is.
 *
 * \param userData this is the same object returned by AIBinder_Class_onCreate
 */
typedef void (*AIBinder_Class_onDestroy)(void* userData);

/**
 * This is called whenever a transaction needs to be processed by a local implementation.
 *
 * \param binder the object being transacted on.
 * \param code implementation-specific code representing which transaction should be taken.
 * \param in the implementation-specific input data to this transaction.
 * \param out the implementation-specific output data to this transaction.
 *
 * \return the implementation-specific output code. This may be forwarded from another service, the
 * result of a parcel read or write, or another error as is applicable to the specific
 * implementation. Usually, implementation-specific error codes are written to the output parcel,
 * and the transaction code is reserved for kernel errors or error codes that have been repeated
 * from subsequent transactions.
 */
typedef binder_status_t (*AIBinder_Class_onTransact)(AIBinder* binder, transaction_code_t code,
                                                     const AParcel* in, AParcel* out);

/**
 * This creates a new instance of a class of binders which can be instantiated. This is called one
 * time during library initialization and cleaned up when the process exits or execs.
 *
 * None of these parameters can be null.
 *
 * Available since API level 29.
 *
 * \param interfaceDescriptor this is a unique identifier for the class. This is used internally for
 * sanity checks on transactions.
 * \param onCreate see AIBinder_Class_onCreate.
 * \param onDestroy see AIBinder_Class_onDestroy.
 * \param onTransact see AIBinder_Class_onTransact.
 *
 * \return the class object representing these parameters or null on error.
 */
__attribute__((warn_unused_result)) AIBinder_Class* AIBinder_Class_define(
        const char* interfaceDescriptor, AIBinder_Class_onCreate onCreate,
        AIBinder_Class_onDestroy onDestroy, AIBinder_Class_onTransact onTransact)
        __INTRODUCED_IN(29);

/**
 * Dump information about an AIBinder (usually for debugging).
 *
 * When no arguments are provided, a brief overview of the interview should be given.
 *
 * \param binder interface being dumped
 * \param fd file descriptor to be dumped to, should be flushed, ownership is not passed.
 * \param args array of null-terminated strings for dump (may be null if numArgs is 0)
 * \param numArgs number of args to be sent
 *
 * \return binder_status_t result of transaction (if remote, for instance)
 */
typedef binder_status_t (*AIBinder_onDump)(AIBinder* binder, int fd, const char** args,
                                           uint32_t numArgs);

/**
 * This sets the implementation of the dump method for a class.
 *
 * If this isn't set, nothing will be dumped when dump is called (for instance with
 * android.os.Binder#dump). Must be called before any instance of the class is created.
 *
 * Available since API level 29.
 *
 * \param dump function to call when an instance of this binder class is being dumped.
 */
void AIBinder_Class_setOnDump(AIBinder_Class* clazz, AIBinder_onDump onDump) __INTRODUCED_IN(29);

/**
 * Creates a new binder object of the appropriate class.
 *
 * Ownership of args is passed to this object. The lifecycle is implemented with AIBinder_incStrong
 * and AIBinder_decStrong. When the reference count reaches zero, onDestroy is called.
 *
 * When this is called, the refcount is implicitly 1. So, calling decStrong exactly one time is
 * required to delete this object.
 *
 * Once an AIBinder object is created using this API, re-creating that AIBinder for the same
 * instance of the same class will break pointer equality for that specific AIBinder object. For
 * instance, if someone erroneously created two AIBinder instances representing the same callback
 * object and passed one to a hypothetical addCallback function and then later another one to a
 * hypothetical removeCallback function, the remote process would have no way to determine that
 * these two objects are actually equal using the AIBinder pointer alone (which they should be able
 * to do). Also see the suggested memory ownership model suggested above.
 *
 * Available since API level 29.
 *
 * \param clazz the type of the object to be created.
 * \param args the args to pass to AIBinder_onCreate for that class.
 *
 * \return a binder object representing the newly instantiated object.
 */
__attribute__((warn_unused_result)) AIBinder* AIBinder_new(const AIBinder_Class* clazz, void* args)
        __INTRODUCED_IN(29);

/**
 * If this is hosted in a process other than the current one.
 *
 * Available since API level 29.
 *
 * \param binder the binder being queried.
 *
 * \return true if the AIBinder represents an object in another process.
 */
bool AIBinder_isRemote(const AIBinder* binder) __INTRODUCED_IN(29);

/**
 * If this binder is known to be alive. This will not send a transaction to a remote process and
 * returns a result based on the last known information. That is, whenever a transaction is made,
 * this is automatically updated to reflect the current alive status of this binder. This will be
 * updated as the result of a transaction made using AIBinder_transact, but it will also be updated
 * based on the results of bookkeeping or other transactions made internally.
 *
 * Available since API level 29.
 *
 * \param binder the binder being queried.
 *
 * \return true if the binder is alive.
 */
bool AIBinder_isAlive(const AIBinder* binder) __INTRODUCED_IN(29);

/**
 * Built-in transaction for all binder objects. This sends a transaction that will immediately
 * return. Usually this is used to make sure that a binder is alive, as a placeholder call, or as a
 * sanity check.
 *
 * Available since API level 29.
 *
 * \param binder the binder being queried.
 *
 * \return STATUS_OK if the ping succeeds.
 */
binder_status_t AIBinder_ping(AIBinder* binder) __INTRODUCED_IN(29);

/**
 * Built-in transaction for all binder objects. This dumps information about a given binder.
 *
 * See also AIBinder_Class_setOnDump, AIBinder_onDump.
 *
 * Available since API level 29.
 *
 * \param binder the binder to dump information about
 * \param fd where information should be dumped to
 * \param args null-terminated arguments to pass (may be null if numArgs is 0)
 * \param numArgs number of args to send
 *
 * \return STATUS_OK if dump succeeds (or if there is nothing to dump)
 */
binder_status_t AIBinder_dump(AIBinder* binder, int fd, const char** args, uint32_t numArgs)
        __INTRODUCED_IN(29);

/**
 * Registers for notifications that the associated binder is dead. The same death recipient may be
 * associated with multiple different binders. If the binder is local, then no death recipient will
 * be given (since if the local process dies, then no recipient will exist to recieve a
 * transaction). The cookie is passed to recipient in the case that this binder dies and can be
 * null. The exact cookie must also be used to unlink this transaction (see AIBinder_linkToDeath).
 * This function may return a binder transaction failure. The cookie can be used both for
 * identification and holding user data.
 *
 * If binder is local, this will return STATUS_INVALID_OPERATION.
 *
 * Available since API level 29.
 *
 * \param binder the binder object you want to receive death notifications from.
 * \param recipient the callback that will receive notifications when/if the binder dies.
 * \param cookie the value that will be passed to the death recipient on death.
 *
 * \return STATUS_OK on success.
 */
binder_status_t AIBinder_linkToDeath(AIBinder* binder, AIBinder_DeathRecipient* recipient,
                                     void* cookie) __INTRODUCED_IN(29);

/**
 * Stops registration for the associated binder dying. Does not delete the recipient. This function
 * may return a binder transaction failure and in case the death recipient cannot be found, it
 * returns STATUS_NAME_NOT_FOUND.
 *
 * This only ever needs to be called when the AIBinder_DeathRecipient remains for use with other
 * AIBinder objects. If the death recipient is deleted, all binders will automatically be unlinked.
 * If the binder dies, it will automatically unlink. If the binder is deleted, it will be
 * automatically unlinked.
 *
 * Available since API level 29.
 *
 * \param binder the binder object to remove a previously linked death recipient from.
 * \param recipient the callback to remove.
 * \param cookie the cookie used to link to death.
 *
 * \return STATUS_OK on success. STATUS_NAME_NOT_FOUND if the binder cannot be found to be unlinked.
 */
binder_status_t AIBinder_unlinkToDeath(AIBinder* binder, AIBinder_DeathRecipient* recipient,
                                       void* cookie) __INTRODUCED_IN(29);

/**
 * This returns the calling UID assuming that this thread is called from a thread that is processing
 * a binder transaction (for instance, in the implementation of AIBinder_Class_onTransact).
 *
 * This can be used with higher-level system services to determine the caller's identity and check
 * permissions.
 *
 * Available since API level 29.
 *
 * \return calling uid or the current process's UID if this thread isn't processing a transaction.
 */
uid_t AIBinder_getCallingUid() __INTRODUCED_IN(29);

/**
 * This returns the calling PID assuming that this thread is called from a thread that is processing
 * a binder transaction (for instance, in the implementation of AIBinder_Class_onTransact).
 *
 * This can be used with higher-level system services to determine the caller's identity and check
 * permissions. However, when doing this, one should be aware of possible TOCTOU problems when the
 * calling process dies and is replaced with another process with elevated permissions and the same
 * PID.
 *
 * Available since API level 29.
 *
 * \return calling pid or the current process's PID if this thread isn't processing a transaction.
 * If the transaction being processed is a oneway transaction, then this method will return 0.
 */
pid_t AIBinder_getCallingPid() __INTRODUCED_IN(29);

/**
 * This can only be called if a strong reference to this object already exists in process.
 *
 * Available since API level 29.
 *
 * \param binder the binder object to add a refcount to.
 */
void AIBinder_incStrong(AIBinder* binder) __INTRODUCED_IN(29);

/**
 * This will delete the object and call onDestroy once the refcount reaches zero.
 *
 * Available since API level 29.
 *
 * \param binder the binder object to remove a refcount from.
 */
void AIBinder_decStrong(AIBinder* binder) __INTRODUCED_IN(29);

/**
 * For debugging only!
 *
 * Available since API level 29.
 *
 * \param binder the binder object to retrieve the refcount of.
 *
 * \return the number of strong-refs on this binder in this process. If binder is null, this will be
 * -1.
 */
int32_t AIBinder_debugGetRefCount(AIBinder* binder) __INTRODUCED_IN(29);

/**
 * This sets the class of an AIBinder object. This checks to make sure the remote object is of
 * the expected class. A class must be set in order to use transactions on an AIBinder object.
 * However, if an object is just intended to be passed through to another process or used as a
 * handle this need not be called.
 *
 * This returns true if the class association succeeds. If it fails, no change is made to the
 * binder object.
 *
 * Available since API level 29.
 *
 * \param binder the object to attach the class to.
 * \param clazz the clazz to attach to binder.
 *
 * \return true if the binder has the class clazz and if the association was successful.
 */
bool AIBinder_associateClass(AIBinder* binder, const AIBinder_Class* clazz) __INTRODUCED_IN(29);

/**
 * Returns the class that this binder was constructed with or associated with.
 *
 * Available since API level 29.
 *
 * \param binder the object that is being queried.
 *
 * \return the class that this binder is associated with. If this binder wasn't created with
 * AIBinder_new, and AIBinder_associateClass hasn't been called, then this will return null.
 */
const AIBinder_Class* AIBinder_getClass(AIBinder* binder) __INTRODUCED_IN(29);

/**
 * Value returned by onCreate for a local binder. For stateless classes (if onCreate returns
 * null), this also returns null. For a remote binder, this will always return null.
 *
 * Available since API level 29.
 *
 * \param binder the object that is being queried.
 *
 * \return the userdata returned from AIBinder_onCreate when this object was created. This may be
 * null for stateless objects. For remote objects, this is always null.
 */
void* AIBinder_getUserData(AIBinder* binder) __INTRODUCED_IN(29);

/**
 * A transaction is a series of calls to these functions which looks this
 * - call AIBinder_prepareTransaction
 * - fill out the in parcel with parameters (lifetime of the 'in' variable)
 * - call AIBinder_transact
 * - read results from the out parcel (lifetime of the 'out' variable)
 */

/**
 * Creates a parcel to start filling out for a transaction. This may add data to the parcel for
 * security, debugging, or other purposes. This parcel is to be sent via AIBinder_transact and it
 * represents the input data to the transaction. It is recommended to check if the object is local
 * and call directly into its user data before calling this as the parceling and unparceling cost
 * can be avoided. This AIBinder must be either built with a class or associated with a class before
 * using this API.
 *
 * This does not affect the ownership of binder. When this function succeeds, the in parcel's
 * ownership is passed to the caller. At this point, the parcel can be filled out and passed to
 * AIBinder_transact. Alternatively, if there is an error while filling out the parcel, it can be
 * deleted with AParcel_delete.
 *
 * Available since API level 29.
 *
 * \param binder the binder object to start a transaction on.
 * \param in out parameter for input data to the transaction.
 *
 * \return STATUS_OK on success. This will return STATUS_INVALID_OPERATION if the binder has not yet
 * been associated with a class (see AIBinder_new and AIBinder_associateClass).
 */
binder_status_t AIBinder_prepareTransaction(AIBinder* binder, AParcel** in) __INTRODUCED_IN(29);

/**
 * Transact using a parcel created from AIBinder_prepareTransaction. This actually communicates with
 * the object representing this binder object. This also passes out a parcel to be used for the
 * return transaction. This takes ownership of the in parcel and automatically deletes it after it
 * is sent to the remote process. The output parcel is the result of the transaction. If the
 * transaction has FLAG_ONEWAY, the out parcel will be empty. Otherwise, this will block until the
 * remote process has processed the transaction, and the out parcel will contain the output data
 * from transaction.
 *
 * This does not affect the ownership of binder. The out parcel's ownership is passed to the caller
 * and must be released with AParcel_delete when finished reading.
 *
 * Available since API level 29.
 *
 * \param binder the binder object to transact on.
 * \param code the implementation-specific code representing which transaction should be taken.
 * \param in the implementation-specific input data to this transaction.
 * \param out the implementation-specific output data to this transaction.
 * \param flags possible flags to alter the way in which the transaction is conducted or 0.
 *
 * \return the result from the kernel or from the remote process. Usually, implementation-specific
 * error codes are written to the output parcel, and the transaction code is reserved for kernel
 * errors or error codes that have been repeated from subsequent transactions.
 */
binder_status_t AIBinder_transact(AIBinder* binder, transaction_code_t code, AParcel** in,
                                  AParcel** out, binder_flags_t flags) __INTRODUCED_IN(29);

/**
 * This does not take any ownership of the input binder, but it can be used to retrieve it if
 * something else in some process still holds a reference to it.
 *
 * Available since API level 29.
 *
 * \param binder object to create a weak pointer to.
 *
 * \return object representing a weak pointer to binder (or null if binder is null).
 */
__attribute__((warn_unused_result)) AIBinder_Weak* AIBinder_Weak_new(AIBinder* binder)
        __INTRODUCED_IN(29);

/**
 * Deletes the weak reference. This will have no impact on the lifetime of the binder.
 *
 * Available since API level 29.
 *
 * \param weakBinder object created with AIBinder_Weak_new.
 */
void AIBinder_Weak_delete(AIBinder_Weak* weakBinder) __INTRODUCED_IN(29);

/**
 * If promotion succeeds, result will have one strong refcount added to it. Otherwise, this returns
 * null.
 *
 * Available since API level 29.
 *
 * \param weakBinder weak pointer to attempt retrieving the original object from.
 *
 * \return an AIBinder object with one refcount given to the caller or null.
 */
__attribute__((warn_unused_result)) AIBinder* AIBinder_Weak_promote(AIBinder_Weak* weakBinder)
        __INTRODUCED_IN(29);

/**
 * This function is executed on death receipt. See AIBinder_linkToDeath/AIBinder_unlinkToDeath.
 *
 * Available since API level 29.
 *
 * \param cookie the cookie passed to AIBinder_linkToDeath.
 */
typedef void (*AIBinder_DeathRecipient_onBinderDied)(void* cookie) __INTRODUCED_IN(29);

/**
 * Creates a new binder death recipient. This can be attached to multiple different binder objects.
 *
 * Available since API level 29.
 *
 * \param onBinderDied the callback to call when this death recipient is invoked.
 *
 * \return the newly constructed object (or null if onBinderDied is null).
 */
__attribute__((warn_unused_result)) AIBinder_DeathRecipient* AIBinder_DeathRecipient_new(
        AIBinder_DeathRecipient_onBinderDied onBinderDied) __INTRODUCED_IN(29);

/**
 * Deletes a binder death recipient. It is not necessary to call AIBinder_unlinkToDeath before
 * calling this as these will all be automatically unlinked.
 *
 * Available since API level 29.
 *
 * \param recipient the binder to delete (previously created with AIBinder_DeathRecipient_new).
 */
void AIBinder_DeathRecipient_delete(AIBinder_DeathRecipient* recipient) __INTRODUCED_IN(29);

#endif  //__ANDROID_API__ >= 29

#if __ANDROID_API__ >= 30

/**
 * Gets the extension registered with AIBinder_setExtension.
 *
 * See AIBinder_setExtension.
 *
 * Available since API level 30.
 *
 * \param binder the object to get the extension of.
 * \param outExt the returned extension object. Will be null if there is no extension set or
 * non-null with one strong ref count.
 *
 * \return error of getting the interface (may be a transaction error if this is
 * remote binder). STATUS_UNEXPECTED_NULL if binder is null.
 */
binder_status_t AIBinder_getExtension(AIBinder* binder, AIBinder** outExt) __INTRODUCED_IN(30);

/**
 * Gets the extension of a binder interface. This allows a downstream developer to add
 * an extension to an interface without modifying its interface file. This should be
 * called immediately when the object is created before it is passed to another thread.
 * No thread safety is required.
 *
 * For instance, imagine if we have this interface:
 *     interface IFoo { void doFoo(); }
 *
 * A). Historical option that has proven to be BAD! Only the original
 *     author of an interface should change an interface. If someone
 *     downstream wants additional functionality, they should not ever
 *     change the interface or use this method.
 *
 *    BAD TO DO:  interface IFoo {                       BAD TO DO
 *    BAD TO DO:      void doFoo();                      BAD TO DO
 *    BAD TO DO: +    void doBar(); // adding a method   BAD TO DO
 *    BAD TO DO:  }                                      BAD TO DO
 *
 * B). Option that this method enables.
 *     Leave the original interface unchanged (do not change IFoo!).
 *     Instead, create a new interface in a downstream package:
 *
 *         package com.<name>; // new functionality in a new package
 *         interface IBar { void doBar(); }
 *
 *     When registering the interface, add:
 *         std::shared_ptr<MyFoo> foo = new MyFoo; // class in AOSP codebase
 *         std::shared_ptr<MyBar> bar = new MyBar; // custom extension class
 *         ... = AIBinder_setExtension(foo->asBinder().get(), bar->asBinder().get());
 *         // handle error
 *
 *     Then, clients of IFoo can get this extension:
 *         SpAIBinder binder = ...;
 *         std::shared_ptr<IFoo> foo = IFoo::fromBinder(binder); // handle if null
 *         SpAIBinder barBinder;
 *         ... = AIBinder_getExtension(barBinder.get());
 *         // handle error
 *         std::shared_ptr<IBar> bar = IBar::fromBinder(barBinder);
 *         // type is checked with AIBinder_associateClass
 *         // if bar is null, then there is no extension or a different
 *         // type of extension
 *
 * Available since API level 30.
 *
 * \param binder the object to get the extension on. Must be local.
 * \param ext the extension to set (binder will hold a strong reference to this)
 *
 * \return OK on success, STATUS_INVALID_OPERATION if binder is not local, STATUS_UNEXPECTED_NULL
 * if either binder is null.
 */
binder_status_t AIBinder_setExtension(AIBinder* binder, AIBinder* ext) __INTRODUCED_IN(30);

#endif  //__ANDROID_API__ >= 30

__END_DECLS

/** @} */
