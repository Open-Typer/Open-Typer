#!/bin/bash

PLATFORM=$1

# exit codes:
# 1: unsupported platform
# 2: lrelease failed
# 3: qmake failed
# 4: build (make) failed
lrelease *.pro || exit 2
lrelease app/*.pro || exit 2
lrelease libcore/*.pro || exit 2
qmake || exit 3
if [[ "$PLATFORM" == "win32" ]]; then
	mingw32-make.exe -j4 || exit 4
elif [[ "$PLATFORM" == "win64" ]]; then
	mingw32-make.exe -j4 || exit 4
elif [[ "$PLATFORM" == "macos" ]]; then
	make -j6 || exit 4
else
	make -j$(nproc --all) || exit 4
fi
