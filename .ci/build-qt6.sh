#!/bin/bash

qt_version="$1"
qt_modules="$(echo $2 | tr ' ' ',')"
target_arch="$3"
root_path="$(pwd)"
sysroot_path="${root_path}/sysroot"
sysroot_ubuntu_version="20.04.5"
host_prefix="${root_path}/qt-host"
cross_prefix="${root_path}/qt-cross"
target_prefix="/usr/local/qt"
toolchain_config="${root_path}/.ci/qt6-toolchain.cmake"

case "$target_arch" in
    aarch64)
        target_arch_name="armv8-a"
        toolchain_name="aarch64-linux-gnu"
        target_platform="linux-aarch64-gnu-g++"
        ;;
    armv7)
        target_arch_name="armv7-a"
        toolchain_name="arm-linux-gnueabihf"
        target_platform="linux-arm-gnueabi-g++"
        ;;
esac

toolchain_prefix="${toolchain_name}-"

case "$target_arch" in
    aarch64)
        target_arch_debian_name="arm64"
        ;;
    armv7)
        target_arch_debian_name="armhf"
        ;;
esac

echo "Qt version to build: ${qt_version}"
echo "Qt modules: ${qt_modules}"
echo "Target architecture: ${target_arch} (${target_arch_name})"

# Install dependencies
${root_path}/.ci/qt6-dependencies.sh || exit 1
${root_path}/.ci/install-cross-compiler.sh "${target_arch}" || exit 1
sudo apt install -y qemu-user-static || exit 1
sudo apt install -y symlinks || exit 1

# Clone Qt
git clone https://github.com/qt/qt5 qt || exit 1
cd qt
git checkout "v$qt_version" || exit 1
./init-repository --module-subset=qtbase,qttools,qtdeclarative,${qt_modules} || exit 1

# Build Qt (host)
mkdir host-build
cd host-build
echo "Building host Qt..."
../configure -release -nomake examples -nomake tests -opensource -confirm-license -prefix "$host_prefix" || exit 1
cmake --build . --parallel $(nproc --all) || exit 1
echo "Installing host Qt..."
cmake --install . || exit 1
cd ..
rm -rf host-build

# Prepare sysroot
echo "Preparing sysroot..."
curl "https://cdimage.ubuntu.com/ubuntu-base/releases/focal/release/ubuntu-base-${sysroot_ubuntu_version}-base-${target_arch_debian_name}.tar.gz" > ./ubuntu-base.tar.gz || exit 1
mkdir -p "$sysroot_path"
sudo tar -xvzf ubuntu-base.tar.gz -C "$sysroot_path" || exit 1
sudo update-binfmts --enable qemu-arm || exit 1
sudo mount -o bind /dev "${sysroot_path}/dev" || exit 1
sudo cp /etc/resolv.conf "${sysroot_path}/etc" || exit 1
sudo chmod 1777 "${sysroot_path}/tmp" || exit 1
sudo cp "${root_path}/.ci/qt6-dependencies.sh" "${sysroot_path}/"
sudo chroot "$sysroot_path" /bin/bash -c "/qt6-dependencies.sh" || exit 1
sudo chroot "$sysroot_path" /bin/bash -c "apt install -y symlinks && symlinks -rc /" || exit 1

# Build Qt (cross)
mkdir cross-build
cd cross-build
echo "Cross-compiling Qt..."
export BUILD_SYSROOT_PATH=${sysroot_path}
export BUILD_TOOLCHAIN_NAME=${toolchain_name}
export BUILD_TOOLCHAIN_PREFIX=${toolchain_prefix}
export BUILD_ARCH_NAME=${target_arch_name}
../configure -release -opengl es2 -nomake examples -nomake tests -qt-host-path "$host_prefix" -xplatform "$target_platform" \
             -device-option CROSS_COMPILE="$toolchain_prefix" -sysroot "$sysroot_path" -opensource -confirm-license \
             -prefix "$target_prefix" -extprefix "$cross_prefix" -- -DCMAKE_TOOLCHAIN_FILE="$toolchain_config" \
             -DQT_FEATURE_xcb=ON -DFEATURE_xcb_xlib=ON -DQT_FEATURE_xlib=ON || exit 1
cmake --build . --parallel $(nproc --all) || exit 1
echo "Installing cross-compiled Qt..."
cmake --install . || exit 1
cd ..
rm -rf cross-build

# Cleanup
sudo umount "${sysroot_path}/dev" || exit 1
sudo rm -rf "${sysroot_path}"
cd ..
rm -rf qt
