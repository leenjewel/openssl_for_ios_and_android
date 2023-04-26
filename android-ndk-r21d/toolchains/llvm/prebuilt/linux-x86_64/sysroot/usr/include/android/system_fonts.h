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
 * @addtogroup Font
 * @{
 */

/**
 * @file system_fonts.h
 * @brief Provides the system font configurations.
 *
 * These APIs provides the list of system installed font files with additional metadata about the
 * font.
 *
 * The ASystemFontIterator_open method will give you an iterator which can iterate all system
 * installed font files as shown in the following example.
 *
 * \code{.cpp}
 *   ASystemFontIterator* iterator = ASystemFontIterator_open();
 *   ASystemFont* font = NULL;
 *
 *   while ((font = ASystemFontIterator_next(iterator)) != nullptr) {
 *       // Look if the font is your desired one.
 *       if (ASystemFont_getWeight(font) == 400 && !ASystemFont_isItalic(font)
 *           && ASystemFont_getLocale(font) == NULL) {
 *           break;
 *       }
 *       ASystemFont_close(font);
 *   }
 *   ASystemFontIterator_close(iterator);
 *
 *   int fd = open(ASystemFont_getFontFilePath(font), O_RDONLY);
 *   int collectionIndex = ASystemFont_getCollectionINdex(font);
 *   std::vector<std::pair<uint32_t, float>> variationSettings;
 *   for (size_t i = 0; i < ASystemFont_getAxisCount(font); ++i) {
 *       variationSettings.push_back(std::make_pair(
 *           ASystemFont_getAxisTag(font, i),
 *           ASystemFont_getAxisValue(font, i)));
 *   }
 *   ASystemFont_close(font);
 *
 *   // Use this font for your text rendering engine.
 *
 * \endcode
 *
 * Available since API level 29.
 */

#ifndef ANDROID_SYSTEM_FONTS_H
#define ANDROID_SYSTEM_FONTS_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/cdefs.h>

#include <android/font.h>

/******************************************************************
 *
 * IMPORTANT NOTICE:
 *
 *   This file is part of Android's set of stable system headers
 *   exposed by the Android NDK (Native Development Kit).
 *
 *   Third-party source AND binary code relies on the definitions
 *   here to be FROZEN ON ALL UPCOMING PLATFORM RELEASES.
 *
 *   - DO NOT MODIFY ENUMS (EXCEPT IF YOU ADD NEW 32-BIT VALUES)
 *   - DO NOT MODIFY CONSTANTS OR FUNCTIONAL MACROS
 *   - DO NOT CHANGE THE SIGNATURE OF FUNCTIONS IN ANY WAY
 *   - DO NOT CHANGE THE LAYOUT OR SIZE OF STRUCTURES
 */

__BEGIN_DECLS

#if __ANDROID_API__ >= 29

/**
 * ASystemFontIterator provides access to the system font configuration.
 *
 * ASystemFontIterator is an iterator for all available system font settings.
 * This iterator is not a thread-safe object. Do not pass this iterator to other threads.
 */
struct ASystemFontIterator;

/**
 * Create a system font iterator.
 *
 * Use ASystemFont_close() to close the iterator.
 *
 * Available since API level 29.
 *
 * \return a pointer for a newly allocated iterator, nullptr on failure.
 */
ASystemFontIterator* _Nullable ASystemFontIterator_open() __INTRODUCED_IN(29);

/**
 * Close an opened system font iterator, freeing any related resources.
 *
 * Available since API level 29.
 *
 * \param iterator a pointer of an iterator for the system fonts. Do nothing if NULL is passed.
 */
void ASystemFontIterator_close(ASystemFontIterator* _Nullable iterator) __INTRODUCED_IN(29);

/**
 * Move to the next system font.
 *
 * Available since API level 29.
 *
 * \param iterator an iterator for the system fonts. Passing NULL is not allowed.
 * \return a font. If no more font is available, returns nullptr. You need to release the returned
 *         font by ASystemFont_close when it is no longer needed.
 */
AFont* _Nullable ASystemFontIterator_next(ASystemFontIterator* _Nonnull iterator) __INTRODUCED_IN(29);

#endif // __ANDROID_API__ >= 29

__END_DECLS

#endif // ANDROID_SYSTEM_FONTS_H

/** @} */
