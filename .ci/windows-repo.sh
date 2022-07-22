#!/bin/bash

### This script generates an installer and a Qt Installer Framework repository and uploads it
git config --global user.name "GitHub Actions Bot"
git config --global user.email "<>"

part1='/<Version>/c\\t<Version>'
VERSION=${previous_tag//v}
part2='<\/Version>'
part3='/<ReleaseDate>/c\\t<ReleaseDate>'
part4='<\/ReleaseDate>'
sed -i -e "${part1}${VERSION}${part2}" config/config.xml
sed -i -e "${part1}${VERSION}${part2}" packages/${windows_app_name}/meta/package.xml
sed -i -e "${part3}$(date +'%Y-%m-%d')${part4}" packages/${windows_app_name}/meta/package.xml

if (( $update_windows_installer == 1 )); then
	git add config/config.xml
	git add packages/${windows_app_name}/meta/package.xml
	git diff --quiet HEAD || git commit -m "Release ${app_name} v${VERSION}"
	git push
fi

mv ../release/* packages/${windows_app_name}/data/${app_name}
./build.sh $(echo ../Tools/QtInstallerFramework/*/bin/binarycreator.exe) $(echo ../Tools/QtInstallerFramework/*/bin/repogen.exe)

if (( $update_windows_repository == 1 )); then
	git push -f --set-upstream origin repository
fi
