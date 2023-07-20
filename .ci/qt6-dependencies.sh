#!/bin/bash

sudo ()
{
    [[ $EUID = 0 ]] || set -- command sudo "$@"
    "$@"
}

sudo apt-get clean
sudo apt-get update

sudo apt-get upgrade -y

DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata
dpkg-reconfigure --frontend noninteractive tzdata

sudo apt install -y libboost-all-dev libudev-dev libinput-dev libts-dev \
                    libmtdev-dev libjpeg-dev libfontconfig1-dev libssl-dev \
                    libdbus-1-dev libglib2.0-dev libxkbcommon-dev libegl1-mesa-dev \
                    libgbm-dev libgles2-mesa-dev mesa-common-dev libasound2-dev \
                    libpulse-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
                    gstreamer1.0-alsa libvpx-dev libsrtp2-dev libsnappy-dev \
                    libnss3-dev "^libxcb.*" flex bison libxslt-dev ruby gperf \
                    libbz2-dev libcups2-dev libatkmm-1.6-dev libxi6 libxcomposite1 \
                    libfreetype6-dev libicu-dev libsqlite3-dev libxslt1-dev \
                    libavcodec-dev libavformat-dev libswscale-dev libx11-dev \
                    freetds-dev libsqlite3-dev libpq-dev libiodbc2 libiodbc2-dev firebird-dev \
                    libgst-dev libxext-dev libxcb1 libxcb1-dev libx11-xcb1 libx11-xcb-dev \
                    libxcb-keysyms1 libxcb-keysyms1-dev libxcb-image0 libxcb-image0-dev \
                    libxcb-shm0 libxcb-shm0-dev libxcb-icccm4 libxcb-icccm4-dev \
                    libxcb-sync1 libxcb-sync-dev libxcb-render-util0 libxcb-render-util0-dev \
                    libxcb-xfixes0-dev libxrender-dev libxcb-shape0-dev libxcb-randr0-dev \
                    libxcb-glx0-dev libxi-dev libdrm-dev libxcb-xinerama0 libxcb-xinerama0-dev \
                    libatspi2.0-dev libxcursor-dev libxcomposite-dev libxdamage-dev libxss-dev \
                    libxtst-dev libpci-dev libcap-dev libxrandr-dev libdirectfb-dev libaudio-dev \
                    libxkbcommon-x11-dev libclang-dev libclang-12-dev || exit 1

#sudo apt install -y gstreamer1.0-omx || exit 1
