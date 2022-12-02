# Build instructions

**Install dependencies (Ubuntu):**

`sudo apt-get install -y git build-essential qtbase5-dev libqt5charts-dev qttools5-dev-tools libssl-dev`

Qt can be installed using the official installer as well.

As of writing, Open-Typer (officially) supports Qt 5 (from 5.9.5 to 5.15.8) and Qt 6 (6.2.4).

```
lrelease *.pro
lrelease app/*.pro
lrelease libcore/*.pro
qmake
make -j$(nproc --all)
```

You may need to use the platform-specifc deploy tool to add shared libraries, for example `windeployqt` for Windows and `macdeployqt` for MacOS.
