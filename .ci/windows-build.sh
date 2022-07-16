#!/bin/bash

curl -o openssl.zip -LJO https://github.com/CristiFati/Prebuilt-Binaries/raw/master/OpenSSL/v1.1.1/OpenSSL-1.1.1f-Win-pc032.zip
mkdir ../openssl
unzip openssl.zip -d ../openssl
mv ../openssl/OpenSSL/OpenSSL/* C:/OpenSSL
rm openssl.zip
find C:/OpenSSL/lib -depth -name "*.lib" -exec sh -c 'f="{}"; mv -- "$f" "${f%.lib}.a"' \;
source .ci/common/build.sh win32
cd release
rm -f *.o *.cpp *.h
windeployqt Open-Typer.exe
cp C:/OpenSSL/bin/*.dll ./
cd ..
mv release ../
