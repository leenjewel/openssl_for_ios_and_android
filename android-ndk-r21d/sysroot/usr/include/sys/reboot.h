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
 * @file sys/reboot.h
 * @brief The reboot() function.
 */

#include <sys/cdefs.h>
#include <linux/reboot.h>

__BEGIN_DECLS

/** The glibc name for the reboot() flag `LINUX_REBOOT_CMD_RESTART`. */
#define RB_AUTOBOOT LINUX_REBOOT_CMD_RESTART
/** The glibc name for the reboot() flag `LINUX_REBOOT_CMD_HALT`. */
#define RB_HALT_SYSTEM LINUX_REBOOT_CMD_HALT
/** The glibc name for the reboot() flag `LINUX_REBOOT_CMD_CAD_ON`. */
#define RB_ENABLE_CAD LINUX_REBOOT_CMD_CAD_ON
/** The glibc name for the reboot() flag `LINUX_REBOOT_CMD_CAD_OFF`. */
#define RB_DISABLE_CAD LINUX_REBOOT_CMD_CAD_OFF
/** The glibc name for the reboot() flag `LINUX_REBOOT_CMD_POWER_OFF`. */
#define RB_POWER_OFF LINUX_REBOOT_CMD_POWER_OFF

/**
 * [reboot(2)](http://man7.org/linux/man-pages/man2/reboot.2.html) reboots the device.
 *
 * Does not return on successful reboot, returns 0 if CAD was successfully enabled/disabled,
 * and returns -1 and sets `errno` on failure.
 */
int reboot(int __cmd);

__END_DECLS
