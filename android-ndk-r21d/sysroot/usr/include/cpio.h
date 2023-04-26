/*
 * Copyright (C) 2016 The Android Open Source Project
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
 * @file cpio.h
 * @brief Constants for reading/writing cpio files.
 */

#include <sys/cdefs.h>

/** Readable by user mode field bit. */
#define C_IRUSR 0000400
/** Writable by user mode field bit. */
#define C_IWUSR 0000200
/** Executable by user mode field bit. */
#define C_IXUSR 0000100
/** Readable by group mode field bit. */
#define C_IRGRP 0000040
/** Writable by group mode field bit. */
#define C_IWGRP 0000020
/** Executable by group mode field bit. */
#define C_IXGRP 0000010
/** Readable by other mode field bit. */
#define C_IROTH 0000004
/** Writable by other mode field bit. */
#define C_IWOTH 0000002
/** Executable by other mode field bit. */
#define C_IXOTH 0000001
/** Set-UID mode field bit. */
#define C_ISUID 0004000
/** Set-GID mode field bit. */
#define C_ISGID 0002000
/** Directory restricted deletion mode field bit. */
#define C_ISVTX 0001000
/** Directory mode field type. */
#define C_ISDIR 0040000
/** FIFO mode field type. */
#define C_ISFIFO 0010000
/** Regular file mode field type. */
#define C_ISREG 0100000
/** Block special file mode field type. */
#define C_ISBLK 0060000
/** Character special file mode field type. */
#define C_ISCHR 0020000
/** Reserved. */
#define C_ISCTG 0110000
/** Symbolic link mode field type. */
#define C_ISLNK 0120000
/** Socket mode field type. */
#define C_ISSOCK 0140000

/** cpio file magic. */
#define MAGIC "070707"
