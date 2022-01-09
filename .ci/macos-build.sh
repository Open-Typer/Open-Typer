#!/bin/bash

.ci/common/build.sh macos

macdeployqt Open-Typer.app -dmg
