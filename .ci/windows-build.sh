#!/bin/bash

source .ci/common/build.sh win32

cd release
rm -f *.o *.cpp *.h
windeployqt Open-Typer.exe
curl -o openssl.zip https://deac-fra.dl.sourceforge.net/project/openssl-for-windows/OpenSSL-1.1.1h_win32.zip
mkdir openssl
unzip openssl.zip -d ../openssl
rm openssl.zip
cp ../openssl/*/*.dll ./
