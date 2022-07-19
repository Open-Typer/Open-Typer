#!/bin/bash

# Install Qt (use build dependencies from debian/control)
#sudo apt install -y devscripts equivs &&
#sudo mk-build-deps --install debian/control &&
# TODO: Enable these commands after Ubuntu 18.04 reaches EOL

# Build
export QT_SELECT=qt5
.ci/common/build.sh linux &&

# Build AppImage
sudo apt install -y zsync
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage &&
chmod +x linuxdeployqt-continuous-x86_64.AppImage &&
mkdir -p AppDir/usr/bin &&
mkdir -p AppDir/usr/lib &&
cp -r res/linux-release/usr/share AppDir/usr/share &&
cp open-typer AppDir/usr/bin/
./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/open-typer.desktop -updateinformation="zsync|https://github.com/Open-Typer/Open-Typer/releases/latest/download/Open-Typer-x86_64.AppImage.zsync" -appimage &&
rm linuxdeployqt-continuous-x86_64.AppImage
