#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

target_arch="$1"

sudo apt install -y sed flex bison gzip gettext texinfo libelf-dev libgomp1 make tar libgmp-dev libmpfr-dev libmpc-dev libisl-dev || exit $?
# TODO: Remove this after dropping Ubuntu 18.04
.ci/update-gcc-path.sh

echo "Preparing sysroot..."
sysroot_path=$(.ci/prepare-sysroot.sh "$target_arch")

#git clone https://github.com/Open-Typer/gcc-cross-compiler || exit $?
git init gcc-cross-compiler || exit $?
cd gcc-cross-compiler
git remote add origin https://github.com/Open-Typer/gcc-cross-compiler || exit $?
git fetch || exit $?
git checkout master || exit $?

case "$target_arch" in
    aarch64)
        sudo ./toolchain.py --arch aarch64 --install yes --cores $(nproc --all) --sysroot "$sysroot_path" || exit $?
        update-alternatives --install /usr/bin/aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc /usr/local/cross/aarch64/bin/aarch64-linux-gnu-gcc 10 || exit $?
        update-alternatives --install /usr/bin/aarch64-linux-gnu-g++ aarch64-linux-gnu-g++ /usr/local/cross/aarch64/bin/aarch64-linux-gnu-g++ 10 || exit $?
        ;;
    armv7)
        sudo ./toolchain.py --arch armhf --install yes --cores $(nproc --all) --sysroot "$sysroot_path" || exit $?
        update-alternatives --install /usr/bin/arm-linux-gnueabihf-gcc arm-linux-gnueabihf-gcc /usr/local/cross/armhf/bin/arm-linux-gnueabihf-gcc 10 || exit $?
        update-alternatives --install /usr/bin/arm-linux-gnueabihf-g++ arm-linux-gnueabihf-g++ /usr/local/cross/armhf/bin/arm-linux-gnueabihf-g++ 10 || exit $?
        ;;
esac
