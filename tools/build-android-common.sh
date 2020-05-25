#!/bin/bash
#
# Copyright 2016 leenjewel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# read -n1 -p "Press any key to continue..."

set -u

if [[ -z ${ANDROID_NDK_ROOT} ]]; then
  echo "ANDROID_NDK_ROOT not defined"
  exit 1
fi

if [[ -z ${ANDROID_HOME} ]]; then
  echo "ANDROID_HOME not defined"
  exit 1
fi

function set_android_toolchain() {

}