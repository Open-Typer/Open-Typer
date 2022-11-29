#!/bin/bash

sudo apt install -y devscripts dput debhelper python3-launchpadlib

CHANNELS=( $(./.ci/common/get-supported-releases.py) )
VERSION=`git describe --tags --abbrev=0`
VERSION=${VERSION//v}
CURRENT_DIR=`pwd`
rm -rf AppDir *.AppImage

for CHANNEL in "${CHANNELS[@]}"; do
echo "${ppa_package_name} (${VERSION}~${CHANNEL}) ${CHANNEL}; urgency=emergency

  * New upstream release.

 -- ${gpg_name} <${gpg_email}>  $(date +'%a, %d %b %Y %T %z')
" > debian/changelog
debuild -S -d
cd ..
dput ppa:${launchpad_name}/${ppa_name} *${CHANNEL}_source.changes
cd "${CURRENT_DIR}"
done
