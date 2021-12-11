#!/bin/bash

BUILD=$1
OUT=$2

cd $BUILD

git config --local user.email "action@github.com"
git config --local user.name "GitHub"

git add $OUT
git commit -m "Build $OUT"
