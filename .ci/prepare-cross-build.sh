#!/bin/bash

target_arch="$1"

case "$target_arch" in
    aarch64)
        toolchain_prefix="aarch64-linux-gnu-"
        echo "APPIMAGE_ARCH=aarch64" >> "${GITHUB_ENV}"
        ;;
    armv7)
        toolchain_prefix="arm-linux-gnueabihf-"
        echo "APPIMAGE_ARCH=armhf" >> "${GITHUB_ENV}"
        ;;
esac

echo "$(pwd)/qt-cross/bin:$(pwd)/qt-host/libexec" >> $GITHUB_PATH
echo "LD_LIBRARY_PATH=$(pwd)/qt-cross/lib:$(pwd)/qt-host/lib" >> "${GITHUB_ENV}"
.ci/install-cross-compiler.sh "$target_arch"
.ci/qt6-dependencies.sh
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

# Prepare cross-tools for linuxdeploy
sudo cp /usr/bin/${toolchain_prefix}strip strip
sudo mv /usr/bin/ldd /usr/bin/ldd-amd64
sudo cp .ci/bin/xldd /usr/bin/${toolchain_prefix}ldd
sudo ln -s /usr/bin/${toolchain_prefix}ldd /usr/bin/ldd
echo "CT_XLDD_ROOT=$(pwd)/sysroot" >> "${GITHUB_ENV}"
