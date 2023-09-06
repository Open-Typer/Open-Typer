#!/bin/bash

### This script waits until the new release is available in the PPA for Ubuntu
sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

sudo apt install -y python3-launchpadlib
VERSION=$(head -n 1 .qmake.conf)
VERSION=${VERSION:8}
echo VERSION=$VERSION
echo launchpad_name=$launchpad_name
echo ppa_name=$ppa_name
echo ppa_package_name=$ppa_package_name
echo $(git for-each-ref --sort=-creatordate --count 1 --format="%(refname:short)" "refs/tags/*")
while true; do
	./.ci/common/get-build-status.py $launchpad_name $ppa_name $ppa_package_name $VERSION
	code=$?
	echo "Build state: $code (0 == success)"
	if (( code == 0 )); then
		exit
	fi
	sleep 5
done
