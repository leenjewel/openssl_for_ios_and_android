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
 * @file font.h
 * @brief Provides some constants used in system_fonts.h or fonts_matcher.h
 *
 * Available since API level 29.
 */

#ifndef ANDROID_FONT_H
#define ANDROID_FONT_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/cdefs.h>

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
    /** The minimum value fot the font weight value. */
    AFONT_WEIGHT_MIN = 0,

    /** A font weight value for the thin weight. */
    AFONT_WEIGHT_THIN = 100,

    /** A font weight value for the extra-light weight. */
    AFONT_WEIGHT_EXTRA_LIGHT = 200,

    /** A font weight value for the light weight. */
    AFONT_WEIGHT_LIGHT = 300,

    /** A font weight value for the normal weight. */
    AFONT_WEIGHT_NORMAL = 400,

    /** A font weight value for the medium weight. */
    AFONT_WEIGHT_MEDIUM = 500,

    /** A font weight value for the semi-bold weight. */
    AFONT_WEIGHT_SEMI_BOLD = 600,

    /** A font weight value for the bold weight. */
    AFONT_WEIGHT_BOLD = 700,

    /** A font weight value for the extra-bold weight. */
    AFONT_WEIGHT_EXTRA_BOLD = 800,

    /** A font weight value for the black weight. */
    AFONT_WEIGHT_BLACK = 900,

    /** The maximum value for the font weight value. */
    AFONT_WEIGHT_MAX = 1000
};

/**
 * AFont provides information of the single font configuration.
 */
struct AFont;

/**
 * Close an AFont.
 *
 * Available since API level 29.
 *
 * \param font a font returned by ASystemFontIterator_next or AFontMatchert_match.
 *        Do nothing if NULL is passed.
 */
void AFont_close(AFont* _Nullable font) __INTRODUCED_IN(29);

/**
 * Return an absolute path to the current font file.
 *
 * Here is a list of font formats returned by this method:
 * <ul>
 *   <li>OpenType</li>
 *   <li>OpenType Font Collection</li>
 *   <li>TrueType</li>
 *   <li>TrueType Collection</li>
 * </ul>
 * The file extension could be one of *.otf, *.ttf, *.otc or *.ttc.
 *
 * The font file returned is guaranteed to be opend with O_RDONLY.
 * Note that the returned pointer is valid until AFont_close() is called for the given font.
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \return a string of the font file path.
 */
const char* _Nonnull AFont_getFontFilePath(const AFont* _Nonnull font) __INTRODUCED_IN(29);

/**
 * Return a weight value associated with the current font.
 *
 * The weight values are positive and less than or equal to 1000.
 * Here are pairs of the common names and their values.
 * <p>
 *  <table>
 *  <tr>
 *  <th align="center">Value</th>
 *  <th align="center">Name</th>
 *  <th align="center">NDK Definition</th>
 *  </tr>
 *  <tr>
 *  <td align="center">100</td>
 *  <td align="center">Thin</td>
 *  <td align="center">{@link AFONT_WEIGHT_THIN}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">200</td>
 *  <td align="center">Extra Light (Ultra Light)</td>
 *  <td align="center">{@link AFONT_WEIGHT_EXTRA_LIGHT}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">300</td>
 *  <td align="center">Light</td>
 *  <td align="center">{@link AFONT_WEIGHT_LIGHT}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">400</td>
 *  <td align="center">Normal (Regular)</td>
 *  <td align="center">{@link AFONT_WEIGHT_NORMAL}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">500</td>
 *  <td align="center">Medium</td>
 *  <td align="center">{@link AFONT_WEIGHT_MEDIUM}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">600</td>
 *  <td align="center">Semi Bold (Demi Bold)</td>
 *  <td align="center">{@link AFONT_WEIGHT_SEMI_BOLD}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">700</td>
 *  <td align="center">Bold</td>
 *  <td align="center">{@link AFONT_WEIGHT_BOLD}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">800</td>
 *  <td align="center">Extra Bold (Ultra Bold)</td>
 *  <td align="center">{@link AFONT_WEIGHT_EXTRA_BOLD}</td>
 *  </tr>
 *  <tr>
 *  <td align="center">900</td>
 *  <td align="center">Black (Heavy)</td>
 *  <td align="center">{@link AFONT_WEIGHT_BLACK}</td>
 *  </tr>
 *  </table>
 * </p>
 * Note that the weight value may fall in between above values, e.g. 250 weight.
 *
 * For more information about font weight, read [OpenType usWeightClass](https://docs.microsoft.com/en-us/typography/opentype/spec/os2#usweightclass)
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \return a positive integer less than or equal to {@link ASYSTEM_FONT_MAX_WEIGHT} is returned.
 */
uint16_t AFont_getWeight(const AFont* _Nonnull font) __INTRODUCED_IN(29);

/**
 * Return true if the current font is italic, otherwise returns false.
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \return true if italic, otherwise false.
 */
bool AFont_isItalic(const AFont* _Nonnull font) __INTRODUCED_IN(29);

/**
 * Return a IETF BCP47 compliant language tag associated with the current font.
 *
 * For information about IETF BCP47, read [Locale.forLanguageTag(java.lang.String)](https://developer.android.com/reference/java/util/Locale.html#forLanguageTag(java.lang.String)")
 *
 * Note that the returned pointer is valid until AFont_close() is called.
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \return a IETF BCP47 compliant language tag or nullptr if not available.
 */
const char* _Nullable AFont_getLocale(const AFont* _Nonnull font) __INTRODUCED_IN(29);

/**
 * Return a font collection index value associated with the current font.
 *
 * In case the target font file is a font collection (e.g. .ttc or .otc), this
 * returns a non-negative value as an font offset in the collection. This
 * always returns 0 if the target font file is a regular font.
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \return a font collection index.
 */
size_t AFont_getCollectionIndex(const AFont* _Nonnull font) __INTRODUCED_IN(29);

/**
 * Return a count of font variation settings associated with the current font
 *
 * The font variation settings are provided as multiple tag-values pairs.
 *
 * For example, bold italic font may have following font variation settings:
 *     'wght' 700, 'slnt' -12
 * In this case, AFont_getAxisCount returns 2 and AFont_getAxisTag
 * and AFont_getAxisValue will return following values.
 * \code{.cpp}
 *     AFont* font = AFontIterator_next(ite);
 *
 *     // Returns the number of axes
 *     AFont_getAxisCount(font);  // Returns 2
 *
 *     // Returns the tag-value pair for the first axis.
 *     AFont_getAxisTag(font, 0);  // Returns 'wght'(0x77676874)
 *     AFont_getAxisValue(font, 0);  // Returns 700.0
 *
 *     // Returns the tag-value pair for the second axis.
 *     AFont_getAxisTag(font, 1);  // Returns 'slnt'(0x736c6e74)
 *     AFont_getAxisValue(font, 1);  // Returns -12.0
 * \endcode
 *
 * For more information about font variation settings, read [Font Variations Table](https://docs.microsoft.com/en-us/typography/opentype/spec/fvar)
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \return a number of font variation settings.
 */
size_t AFont_getAxisCount(const AFont* _Nonnull font) __INTRODUCED_IN(29);


/**
 * Return an OpenType axis tag associated with the current font.
 *
 * See AFont_getAxisCount for more details.
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \param axisIndex an index to the font variation settings. Passing value larger than or
 *        equal to {@link AFont_getAxisCount} is not allowed.
 * \return an OpenType axis tag value for the given font variation setting.
 */
uint32_t AFont_getAxisTag(const AFont* _Nonnull font, uint32_t axisIndex)
      __INTRODUCED_IN(29);

/**
 * Return an OpenType axis value associated with the current font.
 *
 * See AFont_getAxisCount for more details.
 *
 * Available since API level 29.
 *
 * \param font a font object. Passing NULL is not allowed.
 * \param axisIndex an index to the font variation settings. Passing value larger than or
 *         equal to {@link ASYstemFont_getAxisCount} is not allwed.
 * \return a float value for the given font variation setting.
 */
float AFont_getAxisValue(const AFont* _Nonnull font, uint32_t axisIndex)
      __INTRODUCED_IN(29);

#endif // __ANDROID_API__ >= 29

__END_DECLS

#endif // ANDROID_FONT_H

/** @} */
