#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

target_arch="$1"
root_path="$(pwd)"
sysroot_path="${root_path}/sysroot"

if [ -d "$sysroot_path" ]; then
    echo "warning: sysroot already exists"
    exit 0
fi

sysroot_ubuntu_version="$(lsb_release -rs).5"
sysroot_ubuntu_codename="$(lsb_release -cs)"

case "$target_arch" in
    aarch64)
        target_arch_debian_name="arm64"
        ;;
    armv7)
        target_arch_debian_name="armhf"
        ;;
esac

sudo apt install -y qemu-user-static || exit 1
sudo apt install -y symlinks || exit 1

curl "https://cdimage.ubuntu.com/ubuntu-base/releases/${sysroot_ubuntu_codename}/release/ubuntu-base-${sysroot_ubuntu_version}-base-${target_arch_debian_name}.tar.gz" > ./ubuntu-base.tar.gz || exit 1
mkdir -p "$sysroot_path"
sudo tar -xvzf ubuntu-base.tar.gz -C "$sysroot_path" || exit 1
sudo update-binfmts --enable qemu-arm || exit 1
sudo update-binfmts --enable qemu-aarch64 || exit 1
sudo mount -o bind /dev "${sysroot_path}/dev" || exit 1
sudo cp /etc/resolv.conf "${sysroot_path}/etc" || exit 1
sudo chmod 1777 "${sysroot_path}/tmp" || exit 1
sudo cp /usr/bin/qemu-arm-static "${sysroot_path}/usr/bin/"
sudo cp /usr/bin/qemu-aarch64-static "${sysroot_path}/usr/bin/"

# TODO: Remove this after dropping Ubuntu 18.04
case "$target_arch" in
    aarch64)
        curl http://launchpadlibrarian.net/377890883/apt_1.7.0~alpha2_arm64.deb > "${sysroot_path}/apt.deb" || exit 1
        curl http://launchpadlibrarian.net/377890885/libapt-pkg5.0_1.7.0~alpha2_arm64.deb > "${sysroot_path}/libapt-pkg.deb" || exit 1
        ;;
    armv7)
        curl http://launchpadlibrarian.net/377891765/apt_1.7.0~alpha2_armhf.deb > "${sysroot_path}/apt.deb" || exit 1
        curl http://launchpadlibrarian.net/377891767/libapt-pkg5.0_1.7.0~alpha2_armhf.deb > "${sysroot_path}/libapt-pkg.deb" || exit 1
        ;;
esac
sudo chroot "$sysroot_path" /bin/bash -c "dpkg -i *.deb" || exit 1

sudo chroot "$sysroot_path" /bin/bash -c "apt-get clean && apt-get update && apt install -y symlinks && symlinks -rc /" || exit 1
