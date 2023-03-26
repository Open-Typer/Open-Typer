#!/bin/bash

export CPATH=$CPATH:`echo /usr/local/Cellar/openssl@1.1/1.1.1*/include`
export LIBRARY_PATH=`echo /usr/local/Cellar/openssl@1.1/1.1.1*/lib`
VERSION=$(head -n 1 .qmake.conf)
VERSION=${VERSION:8}
VERSION_MAJOR=$(echo $VERSION | sed 's/\..*//')
. .ci/common/build.sh macos

mkdir -p ${app_name}.app/Contents/Frameworks
mv *.dylib ${app_name}.app/Contents/Frameworks/
install_name_tool -change \
	libopentyper-*.${VERSION_MAJOR}.dylib \
	@rpath/libopentyper-*.${VERSION_MAJOR}.dylib \
	${app_name}.app/Contents/MacOS/${executable_name}
macdeployqt ${app_name}.app -qmldir=src

npm install -g appdmg
mv ${app_name}.app res/macos-release/
appdmg res/macos-release/open-typer.json ${app_name}.dmg
