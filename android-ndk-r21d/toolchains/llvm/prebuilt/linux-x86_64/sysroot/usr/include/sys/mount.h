/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

/**
 * @file sys/mount.h
 * @brief Mounting and unmounting filesystems.
 */

#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

__BEGIN_DECLS

/** The umount2() flag to force unmounting. */
#define MNT_FORCE 1
/** The umount2() flag to lazy unmount. */
#define MNT_DETACH 2
/** The umount2() flag to mark a mount point as expired. */
#define MNT_EXPIRE 4

/** The umount2() flag to not dereference the mount point path if it's a symbolic link. */
#define UMOUNT_NOFOLLOW 8

/**
 * [mount(2)](http://man7.org/linux/man-pages/man2/mount.2.html) mounts the filesystem `source` at
 * the mount point `target`.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int mount(const char* __source, const char* __target, const char* __fs_type, unsigned long __flags, const void* __data);

/**
 * [umount(2)](http://man7.org/linux/man-pages/man2/umount.2.html) unmounts the filesystem at
 * the given mount point.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int umount(const char* __target);

/**
 * [umount2(2)](http://man7.org/linux/man-pages/man2/umount2.2.html) unmounts the filesystem at
 * the given mount point, according to the supplied flags.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int umount2(const char* __target, int __flags);

__END_DECLS
