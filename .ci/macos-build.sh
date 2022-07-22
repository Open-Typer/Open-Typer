#!/bin/bash

export CPATH=$CPATH:`echo /usr/local/Cellar/openssl@1.1/1.1.1*/include`
export LIBRARY_PATH=`echo /usr/local/Cellar/openssl@1.1/1.1.1*/lib`
. .ci/common/build.sh macos

macdeployqt ${app_name}.app -dmg
