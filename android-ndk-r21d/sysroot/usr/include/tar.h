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
 * @file tar.h
 * @brief Constants for reading/writing `.tar` files.
 */

#include <sys/cdefs.h>

/** `.tar` file magic. (Includes the NUL.) */
#define TMAGIC "ustar"
/** `.tar` file magic length in bytes. */
#define TMAGLEN 6
/** `.tar` file version. (Does not include the NUL.) */
#define TVERSION "00"
/** `.tar` file version length in bytes. */
#define TVERSLEN 2

/** Regular file type flag. */
#define REGTYPE '0'
/** Alternate regular file type flag. */
#define AREGTYPE '\0'
/** Link type flag. */
#define LNKTYPE '1'
/** Symbolic link type flag. */
#define SYMTYPE '2'
/** Character special file type flag. */
#define CHRTYPE '3'
/** Block special file type flag. */
#define BLKTYPE '4'
/** Directory type flag. */
#define DIRTYPE '5'
/** FIFO special file type flag. */
#define FIFOTYPE '6'
/** Reserved type flag. */
#define CONTTYPE '7'

/** Set-UID mode field bit. */
#define TSUID 04000
/** Set-GID mode field bit. */
#define TSGID 02000
/** Directory restricted deletion mode field bit. */
#define TSVTX 01000
/** Readable by user mode field bit. */
#define TUREAD 00400
/** Writable by user mode field bit. */
#define TUWRITE 00200
/** Executable by user mode field bit. */
#define TUEXEC 00100
/** Readable by group mode field bit. */
#define TGREAD 00040
/** Writable by group mode field bit. */
#define TGWRITE 00020
/** Executable by group mode field bit. */
#define TGEXEC 00010
/** Readable by other mode field bit. */
#define TOREAD 00004
/** Writable by other mode field bit. */
#define TOWRITE 00002
/** Executable by other mode field bit. */
#define TOEXEC 00001
