#!/bin/bash

BUILD = $1

cd $BUILD

qmake
make -j$(nproc --all)
