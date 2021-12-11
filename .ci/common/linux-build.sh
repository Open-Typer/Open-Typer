#!/bin/bash

BUILD=$1

cd $BUILD

qmake
make -j$(nproc --all)

mkdir -p /tmp/release
mv Open-Typer /tmp/release/
