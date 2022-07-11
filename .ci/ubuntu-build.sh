#!/bin/bash

# Install Qt (use build dependencies from debian/control)
sudo apt install -y devscripts equivs &&
sudo mk-build-deps --install debian/control &&

# Build
export QT_SELECT=qt5
.ci/common/build.sh linux
