#ifndef ANDROID_NDK_VERSION_H
#define ANDROID_NDK_VERSION_H

/**
 * Major version of this NDK.
 *
 * For example: 16 for r16.
 */
#define __NDK_MAJOR__ 21

/**
 * Minor version of this NDK.
 *
 * For example: 0 for r16 and 1 for r16b.
 */
#define __NDK_MINOR__ 3

/**
 * Set to 0 if this is a release build, or 1 for beta 1,
 * 2 for beta 2, and so on.
 */
#define __NDK_BETA__ 0

/**
 * Build number for this NDK.
 *
 * For a local development build of the NDK, this is -1.
 */
#define __NDK_BUILD__ 6528147

/**
 * Set to 1 if this is a canary build, 0 if not.
 */
#define __NDK_CANARY__ 0

#endif  /* ANDROID_NDK_VERSION_H */
