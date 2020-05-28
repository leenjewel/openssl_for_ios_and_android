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

export PLATFORM_TYPE=""
export PKG_CONFIG_PATH=$(which pkg-config)

if [[ -z ${PKG_CONFIG_PATH} ]]; then
  echo "PKG_CONFIG_PATH not defined"
  exit 1
fi

function get_cpu_count() {
    if [ "$(uname)" == "Darwin" ]; then
        echo $(sysctl -n hw.physicalcpu)
    else
        echo $(nproc)
    fi
}

function init_log_color() {
    if test -t 1 && which tput >/dev/null 2>&1; then
        ncolors=$(tput colors)
        if test -n "$ncolors" && test $ncolors -ge 8; then
            bold_color=$(tput bold)
            warn_color=$(tput setaf 3)
            error_color=$(tput setaf 1)
            reset_color=$(tput sgr0)
        fi
        # 72 used instead of 80 since that's the default of pr
        ncols=$(tput cols)
    fi
    : ${ncols:=72}
}

function log_info() {
    echo "$warn_color$@$reset_color"
}

function log_warning() {
    echo "$warn_color$bold_color$@$reset_color"
}

function log_error() {
    echo "$error_color$bold_color$@$reset_color"
}

# init_log_color
# log_info "info"
# log_warning "warning"
# log_error "error"
