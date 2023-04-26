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

#ifndef _NDK_CAMERA_WINDOW_TYPE_H
#define _NDK_CAMERA_WINDOW_TYPE_H

/**
 * @addtogroup Camera
 * @{
 */

/**
 * @file NdkCameraWindowType.h
 */

/*
 * This file defines an NDK API.
 * Do not remove methods.
 * Do not change method signatures.
 * Do not change the value of constants.
 * Do not change the size of any of the classes defined in here.
 * Do not reference types that are not part of the NDK.
 * Do not #include files that aren't part of the NDK.
 */

/**
 * This file defines the window type used by NDK and the VNDK variants of the
 * camera2 NDK. This enables us to share the api definition headers and avoid
 * code duplication (since the VNDK variant doesn't use ANativeWindow unlike the
 * NDK variant).
 */
#ifdef __ANDROID_VNDK__
#include <cutils/native_handle.h>
typedef native_handle_t ACameraWindowType;
#else
#include <android/native_window.h>
typedef ANativeWindow ACameraWindowType;
#endif

#endif //_NDK_CAMERA_WINDOW_TYPE_H
