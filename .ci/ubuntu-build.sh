#!/bin/bash

# Build
.ci/common/build.sh linux &&

# Build AppImage
sudo apt install -y zsync
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage &&
chmod +x linuxdeployqt-continuous-x86_64.AppImage &&
mkdir -p AppDir/usr/bin &&
mkdir -p AppDir/usr/lib &&
mkdir -p AppDir/usr/share/applications &&
cp -r app/res/linux-release/usr/share/pixmaps AppDir/usr/share/ &&
cp app/res/linux-release/usr/share/applications/open-typer-appimage.desktop AppDir/usr/share/applications/open-typer.desktop &&
cp "$executable_name" AppDir/usr/bin/
cp *.so* AppDir/usr/lib/
./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/*.desktop -qmldir=app/ui/qml -updateinformation="zsync|${appimage_zsync_url}" -appimage &&
rm linuxdeployqt-continuous-x86_64.AppImage
