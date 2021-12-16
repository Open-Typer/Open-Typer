#!/bin/bash

# TODO: Rewrite deb build script
#.ci/common/build.sh linux
git config --local user.name "adazem009"
git config --local user.email "68537469+adazem009@users.noreply.github.com"
./build-deb.sh
