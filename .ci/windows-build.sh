#!/bin/bash

if [[ "$1" != "1" ]]; then
	curl -o openssl.zip -LJO https://github.com/CristiFati/Prebuilt-Binaries/raw/master/OpenSSL/v1.1.1/OpenSSL-1.1.1q-Win-pc064.zip
	mkdir ../openssl
	unzip openssl.zip -d ../openssl
	mv ../openssl/OpenSSL/OpenSSL/* C:/OpenSSL
	rm openssl.zip
	find C:/OpenSSL/lib -depth -name "*.lib" -exec sh -c 'f="{}"; mv -- "$f" "${f%.lib}.a"' \;
else
	cd release
	rm -f *.o *.cpp *.h
	cp C:/OpenSSL/bin/*.dll ./
	cd ..
	mv release ../
fi
