#!/bin/bash

curl -o openssl.zip -LJO https://github.com/CristiFati/Prebuilt-Binaries/raw/master/OpenSSL/v1.1.1/OpenSSL-1.1.1f-Win-pc032.zip
mkdir ../openssl
unzip openssl.zip -d ../openssl
mv ../openssl/* C:/OpenSSL
rm openssl.zip
find C:/OpenSSL/lib -depth -name "*.lib" -exec sh -c 'f="{}"; mv -- "$f" "${f%.lib}.a"' \;
source .ci/common/build.sh win32
cd release
rm -f *.o *.cpp *.h
windeployqt Open-Typer.exe
cp C:/OpenSSL/bin/*.dll ./

# Installer - does not work in CI right now...
#cd ../installer
#cp -r ../release ./packages/com.adazem009.Open-Typer/data
#binarycreator -c config/config.xml -p packages/ Open-Typer-installer.exe --include com.adazem009.Open-Typer
