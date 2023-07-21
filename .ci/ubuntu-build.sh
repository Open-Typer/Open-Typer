#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

# Build
if [[ "$1" != "0" ]]; then
    .ci/common/build.sh linux || exit 1
fi

# Build linuxdeploy
sudo apt install -y build-essential g++ pkg-config curl wget libpng-dev libjpeg-dev zsync desktop-file-utils libxcb-cursor0 &&
git clone https://github.com/Open-Typer/linuxdeploy linuxdeploy-build --recurse-submodules &&
mkdir linuxdeploy-build/build
cd linuxdeploy-build/build &&
# Add src/core/copyright to include paths (see https://github.com/linuxdeploy/linuxdeploy/issues/212)
cmake -DCMAKE_CXX_FLAGS=-isystem\ $(pwd)/../src/core/copyright .. &&
make -j$(nproc --all) &&
mv bin/linuxdeploy ../.. &&
cd ../.. &&
rm -rf linuxdeploy-build &&

# Build linuxdeploy-plugin-qt
sudo apt install -y nlohmann-json3-dev &&
git clone https://github.com/Open-Typer/linuxdeploy-plugin-qt plugin-qt --recurse-submodules &&
mkdir plugin-qt/build &&
cd plugin-qt/build &&
cmake .. &&
make -j$(nproc --all) &&
mv bin/linuxdeploy-plugin-qt ../.. &&
cd ../.. &&
rm -rf plugin-qt &&

# Build AppImageKit
sudo apt install -y snapd squashfs-tools &&
sudo snap install docker &&
git clone https://github.com/AppImage/AppImageKit --recurse-submodules &&
cd AppImageKit &&
sudo env ARCH=$(arch) bash ci/build.sh
sudo cp out/appimagetool /usr/bin/ &&
sudo cp out/digest /usr/bin/ &&
sudo cp out/validate /usr/bin/ &&
cd .. &&
sudo mkdir -p /usr/lib/appimagekit &&
sudo ln -s /usr/bin/mksquashfs /usr/lib/appimagekit/mksquashfs &&

# Install patchelf from PyPI (see https://github.com/linuxdeploy/linuxdeploy-plugin-qt/issues/133#issuecomment-1608168363)
# TODO: Remove this after dropping Ubuntu 20.04
sudo apt install -y python3-pip
pip3 install patchelf
export PATH=$PATH:~/.local/bin

# Build AppImage
export QML_SOURCES_PATHS=src &&
export EXTRA_QT_PLUGINS="svg;" &&
export LDAI_UPDATE_INFORMATION="${appimage_zsync_prefix}${app_name}*-${APPIMAGE_ARCH-$(arch)}.AppImage.zsync"
echo "AppImage update information: ${LDAI_UPDATE_INFORMATION}"
./linuxdeploy --appdir AppDir -e open-typer -i snap/gui/open-typer.png -d res/linux-release/usr/share/applications/open-typer-appimage.desktop --plugin qt

case "$(qmake -query QMAKE_XSPEC)" in
    linux-g++)
        appimagetool AppDir
        ;;
    linux-arm-gnueabi-g++)
        wget https://github.com/AppImage/AppImageKit/releases/download/continuous/runtime-armhf
        ARCH=arm appimagetool --runtime-file runtime-armhf AppDir
        ;;
    linux-aarch64-gnu-g++)
        wget https://github.com/AppImage/AppImageKit/releases/download/continuous/runtime-aarch64
        ARCH=arm_aarch64 appimagetool --runtime-file runtime-aarch64 AppDir
        ;;
esac
