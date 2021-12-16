#!/bin/bash

PLATFORM=$1

qmake
if [[ "$PLATFORM" == "win32" ]]; then
	mingw32-make.exe -j4
elif [[ "$PLATFORM" == "win64" ]]; then
	echo "Error: Only win32 is supported right now."
	exit 1
elif [[ "$PLATFORM" == "macos" ]]; then
	make -j6
else
	make -j$(nproc --all)
fi
