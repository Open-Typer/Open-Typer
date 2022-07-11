#!/bin/bash

sudo apt install -y devscripts dput equivs
sudo mk-build-deps --install debian/control

CHANNELS=( bionic focal jammy )
VERSION=`git describe --tags --abbrev=0`
VERSION=${VERSION//v}
CURRENT_DIR=`pwd`

for CHANNEL in "${CHANNELS[@]}"; do
echo "open-typer (${VERSION}~${CHANNEL}) ${CHANNEL}; urgency=medium

  * New upstream release.

 -- adazem009 <adazem.009.games@gmail.com>  $(date +'%a, %d %b %Y %T %z')
" > debian/changelog
debuild -S
cd ..
dput ppa:adazem009/open-typer *${CHANNEL}_source.changes
cd "${CURRENT_DIR}"
done
