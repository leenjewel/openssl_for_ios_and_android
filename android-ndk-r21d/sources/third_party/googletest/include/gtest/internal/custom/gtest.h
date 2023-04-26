// Copyright 2015, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Injection point for custom user configurations. See README for details
//
// ** Custom implementation starts here **

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_CUSTOM_GTEST_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_CUSTOM_GTEST_H_

#define GTEST_CUSTOM_TEMPDIR_FUNCTION_ GetAndroidTempDir
static inline std::string GetAndroidTempDir() {
  // Android doesn't have /tmp, and /sdcard is no longer accessible from
  // an app context starting from Android O. On Android, /data/local/tmp
  // is usually used as the temporary directory, so try that first...
  if (access("/data/local/tmp", R_OK | W_OK | X_OK) == 0) return "/data/local/tmp/";

  // Processes running in an app context can't write to /data/local/tmp,
  // so fall back to the current directory...
  std::string result = "./";
  char* cwd = getcwd(NULL, 0);
  if (cwd != NULL) {
    result = cwd;
    result += "/";
    free(cwd);
  }
  return result;
}

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_CUSTOM_GTEST_H_
