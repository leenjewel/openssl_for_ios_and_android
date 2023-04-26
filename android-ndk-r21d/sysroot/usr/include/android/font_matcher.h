/*
 * Copyright (C) 2019 The Android Open Source Project
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
 * @file font_matcher.h
 * @brief Provides the font matching logic with various inputs.
 *
 * You can use this class for deciding what font is to be used for drawing text.
 *
 * A matcher is created from text style, locales and UI compatibility. The match function for
 * matcher object can be called multiple times until close function is called.
 *
 * Even if no font can render the given text, the match function will return a non-null result for
 * drawing Tofu character.
 *
 * Examples:
 * \code{.cpp}
 *  // Simple font query for the ASCII character.
 *  std::vector<uint16_t> text = { 'A' };
 *  AFontMatcher* matcher = AFontMatcher_create("sans-serif");
 *  ASystemFont* font = AFontMatcher_match(text.data(), text.length(), &runLength);
 *  // runLength will be 1 and the font will points a valid font file.
 *  AFontMatcher_destroy(matcher);
 *
 *  // Querying font for CJK characters
 *  std::vector<uint16_t> text = { 0x9AA8 };
 *  AFontMatcher* matcher = AFontMatcher_create("sans-serif");
 *  AFontMatcher_setLocales(matcher, "zh-CN,ja-JP");
 *  ASystemFont* font = AFontMatcher_match(text.data(), text.length(), &runLength);
 *  // runLength will be 1 and the font will points a Simplified Chinese font.
 *  AFontMatcher_setLocales(matcher, "ja-JP,zh-CN");
 *  ASystemFont* font = AFontMatcher_match(text.data(), text.length(), &runLength);
 *  // runLength will be 1 and the font will points a Japanese font.
 *  AFontMatcher_destroy(matcher);
 *
 *  // Querying font for text/color emoji
 *  std::vector<uint16_t> text = { 0xD83D, 0xDC68, 0x200D, 0x2764, 0xFE0F, 0x200D, 0xD83D, 0xDC68 };
 *  AFontMatcher* matcher = AFontMatcher_create("sans-serif");
 *  ASystemFont* font = AFontMatcher_match(text.data(), text.length(), &runLength);
 *  // runLength will be 8 and the font will points a color emoji font.
 *  AFontMatcher_destroy(matcher);
 *
 *  // Mixture of multiple script of characters.
 *  // 0x05D0 is a Hebrew character and 0x0E01 is a Thai character.
 *  std::vector<uint16_t> text = { 0x05D0, 0x0E01 };
 *  AFontMatcher* matcher = AFontMatcher_create("sans-serif");
 *  ASystemFont* font = AFontMatcher_match(text.data(), text.length(), &runLength);
 *  // runLength will be 1 and the font will points a Hebrew font.
 *  AFontMatcher_destroy(matcher);
 * \endcode
 *
 * Available since API level 29.
 */

#ifndef ANDROID_FONT_MATCHER_H
#define ANDROID_FONT_MATCHER_H

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

enum {
    /** A family variant value for the system default variant. */
    AFAMILY_VARIANT_DEFAULT = 0,

    /**
     * A family variant value for the compact font family variant.
     *
     * The compact font family has Latin-based vertical metrics.
     */
    AFAMILY_VARIANT_COMPACT = 1,

    /**
     * A family variant value for the elegant font family variant.
     *
     * The elegant font family may have larger vertical metrics than Latin font.
     */
    AFAMILY_VARIANT_ELEGANT = 2,
};

/**
 * AFontMatcher performs match operation on given parameters and available font files.
 * This matcher is not a thread-safe object. Do not pass this matcher to other threads.
 */
struct AFontMatcher;

/**
 * Select the best font from given parameters.
 *
 */

/**
 * Creates a new AFontMatcher object.
 *
 * Available since API level 29.
 */
AFontMatcher* _Nonnull AFontMatcher_create() __INTRODUCED_IN(29);

/**
 * Destroy the matcher object.
 *
 * Available since API level 29.
 *
 * \param matcher a matcher object. Passing NULL is not allowed.
 */
void AFontMatcher_destroy(AFontMatcher* _Nonnull matcher) __INTRODUCED_IN(29);

/**
 * Set font style to matcher.
 *
 * If this function is not called, the matcher performs with {@link ASYSTEM_FONT_WEIGHT_NORMAL}
 * with non-italic style.
 *
 * Available since API level 29.
 *
 * \param matcher a matcher object. Passing NULL is not allowed.
 * \param weight a font weight value. Only from 0 to 1000 value is valid
 * \param italic true if italic, otherwise false.
 */
void AFontMatcher_setStyle(
        AFontMatcher* _Nonnull matcher,
        uint16_t weight,
        bool italic) __INTRODUCED_IN(29);

/**
 * Set font locales to matcher.
 *
 * If this function is not called, the matcher performs with empty locale list.
 *
 * Available since API level 29.
 *
 * \param matcher a matcher object. Passing NULL is not allowed.
 * \param languageTags a null character terminated comma separated IETF BCP47 compliant language
 *                     tags.
 */
void AFontMatcher_setLocales(
        AFontMatcher* _Nonnull matcher,
        const char* _Nonnull languageTags) __INTRODUCED_IN(29);

/**
 * Set family variant to matcher.
 *
 * If this function is not called, the matcher performs with {@link AFAMILY_VARIANT_DEFAULT}.
 *
 * Available since API level 29.
 *
 * \param matcher a matcher object. Passing NULL is not allowed.
 * \param familyVariant Must be one of {@link AFAMILY_VARIANT_DEFAULT},
 *                      {@link AFAMILY_VARIANT_COMPACT} or {@link AFAMILY_VARIANT_ELEGANT} is valid.
 */
void AFontMatcher_setFamilyVariant(
        AFontMatcher* _Nonnull matcher,
        uint32_t familyVariant) __INTRODUCED_IN(29);

/**
 * Performs the matching from the generic font family for the text and select one font.
 *
 * For more information about generic font families, read [W3C spec](https://www.w3.org/TR/css-fonts-4/#generic-font-families)
 *
 * Even if no font can render the given text, this function will return a non-null result for
 * drawing Tofu character.
 *
 * Available since API level 29.
 *
 * \param matcher a matcher object. Passing NULL is not allowed.
 * \param familyName a null character terminated font family name
 * \param text a UTF-16 encoded text buffer to be rendered. Do not pass empty string.
 * \param textLength a length of the given text buffer. This must not be zero.
 * \param runLengthOut if not null, the font run length will be filled.
 * \return a font to be used for given text and params. You need to release the returned font by
 *         ASystemFont_close when it is no longer needed.
 */
AFont* _Nonnull AFontMatcher_match(
        const AFontMatcher* _Nonnull matcher,
        const char* _Nonnull familyName,
        const uint16_t* _Nonnull text,
        const uint32_t textLength,
        uint32_t* _Nullable runLengthOut) __INTRODUCED_IN(29);

#endif // __ANDROID_API__ >= 29

__END_DECLS

#endif // ANDROID_FONT_MATCHER_H

/** @} */
