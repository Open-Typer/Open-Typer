#!/bin/bash

BUILD=$1
OUT=$2

cd $BUILD

git checkout build
git add $OUT
git commit -m "Build $OUT"
