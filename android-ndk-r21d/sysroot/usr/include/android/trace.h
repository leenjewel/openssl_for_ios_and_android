/*
 * Copyright (C) 2015 The Android Open Source Project
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
 * @addtogroup Tracing
 * @{
 */

/**
 * @file trace.h
 * @brief Writes trace events to the system trace buffer.
 *
 * These trace events can be collected and visualized using the Systrace tool.
 * For information about using the Systrace tool, read <a href="https://developer.android.com/studio/profile/systrace.html">Analyzing UI Performance with Systrace</a>.
 *
 * Available since API level 23.
 */

#ifndef ANDROID_NATIVE_TRACE_H
#define ANDROID_NATIVE_TRACE_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __ANDROID_API__ >= 23

/**
 * Returns true if tracing is enabled. Use this to avoid expensive computation only necessary
 * when tracing is enabled.
 *
 * Available since API level 23.
 */
bool ATrace_isEnabled() __INTRODUCED_IN(23);

/**
 * Writes a tracing message to indicate that the given section of code has begun. This call must be
 * followed by a corresponding call to {@link ATrace_endSection} on the same thread.
 *
 * Note: At this time the vertical bar character '|' and newline character '\\n' are used internally
 * by the tracing mechanism. If \p sectionName contains these characters they will be replaced with a
 * space character in the trace.
 *
 * Available since API level 23.
 */
void ATrace_beginSection(const char* sectionName) __INTRODUCED_IN(23);

/**
 * Writes a tracing message to indicate that a given section of code has ended. This call must be
 * preceeded by a corresponding call to {@link ATrace_beginSection} on the same thread. Calling this method
 * will mark the end of the most recently begun section of code, so care must be taken to ensure
 * that {@link ATrace_beginSection}/{@link ATrace_endSection} pairs are properly nested and called from the same thread.
 *
 * Available since API level 23.
 */
void ATrace_endSection() __INTRODUCED_IN(23);

#endif /* __ANDROID_API__ >= 23 */

#if __ANDROID_API__ >= 29

/**
 * Writes a trace message to indicate that a given section of code has
 * begun. Must be followed by a call to {@link ATrace_endAsyncSection} with the same
 * methodName and cookie. Unlike {@link ATrace_beginSection} and {@link ATrace_endSection},
 * asynchronous events do not need to be nested. The name and cookie used to
 * begin an event must be used to end it.
 *
 * Available since API level 29.
 *
 * \param sectionName The method name to appear in the trace.
 * \param cookie Unique identifier for distinguishing simultaneous events
 */
void ATrace_beginAsyncSection(const char* sectionName, int32_t cookie) __INTRODUCED_IN(29);

/**
 * Writes a trace message to indicate that the current method has ended.
 * Must be called exactly once for each call to {@link ATrace_beginAsyncSection}
 * using the same name and cookie.
 *
 * Available since API level 29.
 *
 * \param methodName The method name to appear in the trace.
 * \param cookie Unique identifier for distinguishing simultaneous events
 */
void ATrace_endAsyncSection(const char* sectionName, int32_t cookie) __INTRODUCED_IN(29);

/**
 * Writes trace message to indicate the value of a given counter.
 *
 * Available since API level 29.
 *
 * \param counterName The counter name to appear in the trace.
 * \param counterValue The counter value.
 */
void ATrace_setCounter(const char* counterName, int64_t counterValue) __INTRODUCED_IN(29);

#endif /* __ANDROID_API__ >= 29 */

#ifdef __cplusplus
};
#endif

#endif // ANDROID_NATIVE_TRACE_H

/** @} */
