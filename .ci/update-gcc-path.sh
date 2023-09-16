#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 9
sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-9 9
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 9
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-9 9
sudo update-alternatives --set gcc /usr/bin/gcc-9
sudo update-alternatives --set cc /usr/bin/gcc-9
sudo update-alternatives --set g++ /usr/bin/g++-9
sudo update-alternatives --set c++ /usr/bin/g++-9
