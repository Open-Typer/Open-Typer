#!/bin/bash

export CPATH=$CPATH:`echo /usr/local/Cellar/openssl@1.1/1.1.1*/include`
.ci/common/build.sh macos

macdeployqt Open-Typer.app -dmg
