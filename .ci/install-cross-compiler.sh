#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

sudo apt update
case "$1" in
    aarch64)
        sudo apt install -y g++-aarch64-linux-gnu
        ;;
    armv7)
        sudo apt install -y g++-arm-linux-gnueabihf
esac
