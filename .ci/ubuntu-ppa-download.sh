#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

### This script downloads new Ubuntu packages from the PPA
sudo apt install -y python3-launchpadlib
VERSION=$(head -n 1 .qmake.conf)
VERSION=${VERSION:8}
(./.ci/common/get-package-urls.py $launchpad_name $ppa_name $VERSION | xargs wget) ||
(sleep 10 && rm -f *.deb && exec "$0")
