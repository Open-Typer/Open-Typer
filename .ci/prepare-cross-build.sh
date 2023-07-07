#!/bin/bash

target_arch="$1"

echo "$(pwd)/qt-cross/bin:$PATH" >> $GITHUB_PATH
echo "LD_LIBRARY_PATH=$(pwd)/qt-cross/lib:$(pwd)/qt-host/lib" >> "${GITHUB_ENV}"
.ci/install-cross-compiler.sh "$target_arch"
if [[ "$target_arch" == "armv7" ]]; then
    echo "QMAKE_CC=arm-linux-gnueabihf-gcc
    QMAKE_CXX=arm-linux-gnueabihf-g++
    QMAKE_LINK=arm-linux-gnueabihf-g++
    QMAKE_LINK_SHLIB=arm-linux-gnueabihf-g++
    QMAKE_AR=arm-linux-gnueabihf-ar cqs
    QMAKE_OBJCOPY=arm-linux-gnueabihf-objcopy
    QMAKE_NM=arm-linux-gnueabihf-nm -P
    QMAKE_STRIP=arm-linux-gnueabihf-strip" >> .qmake.conf
fi
