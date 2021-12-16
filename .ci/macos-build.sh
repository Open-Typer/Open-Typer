#!/bin/bash

.ci/common/build.sh macos

cd Open-Typer.app
zip -r ../Open-Typer-mac.zip .
