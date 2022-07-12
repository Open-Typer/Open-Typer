#!/bin/bash

### This script generates an installer and a Qt Installer Framework repository and uploads it
git config --global user.name "GitHub Actions Bot"
git config --global user.email "<>"
../add-credentials.sh ${1} ${2}

part1='/<Version>/c\\t<Version>'
VERSION=${3//v}
part2='<\/Version>'
part3='/<ReleaseDate>/c\\t<ReleaseDate>'
part4='<\/ReleaseDate>'
sed -i -e "${part1}${VERSION}${part2}" config/config.xml
sed -i -e "${part1}${VERSION}${part2}" packages/com.adazem009.opentyper/meta/package.xml
sed -i -e "${part3}$(date +'%Y-%m-%d')${part4}" packages/com.adazem009.opentyper/meta/package.xml
git add config/config.xml
git add packages/com.adazem009.opentyper/meta/package.xml
git diff --quiet HEAD || git commit -m "Release Open-Typer v${VERSION}"
git push

mv ../release/* packages/com.adazem009.opentyper/data/Open-Typer
./build.sh $(echo ../Tools/QtInstallerFramework/*/bin/binarycreator.exe) $(echo ../Tools/QtInstallerFramework/*/bin/repogen.exe)
git push -f --set-upstream origin repository
mv ../*.zip .
