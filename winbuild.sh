#!/bin/bash

### Windows cross-compilation script
### Uses MXE (M cross environment).
### 2021 - adazem009

if ! [ -s mxe ]; then
	git clone https://github.com/mxe/mxe || exit 1
elif [ -f mxe ]; then
	echo "Error! 'mxe' file exists."
	exit 2
fi

if ! [ -f mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 ]; then
	cd mxe
	make qtbase -j$(nproc --all) || exit 3
	cd ..
fi

export PATH=$PATH:$PWD/mxe/usr/bin
i686-w64-mingw32.static-qmake-qt5
make -j$(nproc --all)
