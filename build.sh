#!/bin/bash

qmake &&
make -j$(nproc --all) &&
echo "Success, run ./open-typer.sh to start Open-Typer."
