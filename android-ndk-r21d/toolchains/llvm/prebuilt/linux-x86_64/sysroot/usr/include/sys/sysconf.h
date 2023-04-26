#pragma once

/**
 * @file sys/sysconf.h
 * @brief Historical synonym for `<sysconf.h>`.
 *
 * This file used to contain the declarations of sysconf and its associated constants.
 * No standard mentions a `<sys/sysconf.h>`, but there are enough users in vendor (and
 * potential NDK users) to warrant not breaking source compatibility.
 *
 * New code should use `<sysconf.h>` directly.
 */

#include <bits/sysconf.h>
