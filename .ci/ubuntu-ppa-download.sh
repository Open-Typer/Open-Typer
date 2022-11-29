#!/bin/bash

### This script downloads new Ubuntu packages from the PPA
sudo apt install -y python3-launchpadlib
VERSION=`git describe --tags --abbrev=0`
VERSION=${VERSION//v}
(./.ci/common/get-package-urls.py $launchpad_name $ppa_name $VERSION | xargs wget) ||
(sleep 10 && rm -f *.deb && exec "$0")
