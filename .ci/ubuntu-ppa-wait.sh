#!/bin/bash

### This script waits until the new release is available in the PPA for Ubuntu
VERSION=`git describe --tags --abbrev=0`
VERSION=${VERSION//v}
while true; do
	./.ci/common/get-build-status.py $VERSION
	code=$?
	echo "Build state: $code (0 == success)"
	if (( code == 0 )); then
		exit
	fi
	sleep 5
done
