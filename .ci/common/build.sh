#!/bin/bash

PLATFORM=$1

# exit codes:
# 1: unsupported platform
# 3: qmake failed
# 4: build (make) failed

if [[ "$PLATFORM" == "wasm" ]]; then
	$RUNNER_WORKSPACE/Qt/*/wasm_*/bin/qmake || exit 3
else
	qmake || exit 3
fi
if [[ "$PLATFORM" == "win32" ]]; then
	mingw32-make.exe -j4 || exit 4
elif [[ "$PLATFORM" == "win64" ]]; then
	mingw32-make.exe -j4 || exit 4
elif [[ "$PLATFORM" == "macos" ]]; then
	make -j6 || exit 4
else
	make -j$(nproc --all) || exit 4
fi
