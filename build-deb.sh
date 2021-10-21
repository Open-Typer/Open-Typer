#!/bin/bash

### Deb build script for Open-Typer
### 2021 - adazem009

name="Open-Typer"
bin_name="Open-Typer"
pkg_name="open-typer"
revision=1

# Dependencies
deps=\
"libc6 (>= 2.14), \
libgcc-s1 (>= 3.0), \
libqt5core5a (>= 5.12.2), \
libqt5gui5 (>= 5.8.0) | libqt5gui5-gles (>= 5.8.0), \
libqt5network5 (>= 5.0.2), \
libqt5widgets5 (>= 5.0.2), \
libstdc++6 (>= 4.1.1)"

# Build
qmake || exit $?
make clean || exit $?
make -j$(nproc --all) || exit $?

# Read version
version_h=`cat src/include/version.h`
version=''
i=18
while (( i < $((${#version_h}-1)) )); do
	version="${version}${version_h:$i:1}"
	i=$((i+1))
done
echo Version: $version

# Get architecture
arch=$(dpkg-architecture -q DEB_BUILD_ARCH)

# Get maintainer
maintainer=$(git config --get user.name) || exit $?
maintainer_email=$(git config --get user.email) || exit $?

# Create working directory
working_dir="${pkg_name}_${version}-${revision}_${arch}"
mkdir -p "$working_dir"

# Add binaries
mkdir -p "${working_dir}/usr/local/bin"
cp "${bin_name}" "${working_dir}/usr/local/bin/"

# Create control file
mkdir -p "${working_dir}/DEBIAN"
echo \
"Package: ${name}
Version: ${version}
Architecture: ${arch}
Maintainer: ${maintainer} ${maintainer_email}
Description: Free and open source typing tutor program
Depends: ${deps}" \
> "${working_dir}/DEBIAN/control"

# Build deb
dpkg-deb --build --root-owner-group "${pkg_name}_${version}-${revision}_${arch}"
