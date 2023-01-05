#!/bin/bash

export CPATH=$CPATH:`echo /usr/local/Cellar/openssl@1.1/1.1.1*/include`
export LIBRARY_PATH=`echo /usr/local/Cellar/openssl@1.1/1.1.1*/lib`
VERSION=`git describe --tags --abbrev=0`
VERSION=${VERSION//v}
VERSION_MAJOR=$(echo $VERSION | sed 's/\..*//')
. .ci/common/build.sh macos

mkdir -p ${app_name}.app/Contents/Frameworks
mv *.dylib ${app_name}.app/Contents/Frameworks/
install_name_tool -change \
	libopentyper-core.${VERSION_MAJOR}.dylib \
	@rpath/libopentyper-core.${VERSION_MAJOR}.dylib \
	${app_name}.app/Contents/MacOS/${executable_name}
macdeployqt ${app_name}.app -qmldir app/ui/qml -dmg
