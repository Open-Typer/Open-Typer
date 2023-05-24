# Build instructions

As of writing, Open-Typer (officially) supports Qt 5 (from 5.12 to 5.15) and Qt 6 (6.4).

**Install dependencies (Ubuntu):**

```
sudo apt-get install -y git build-essential qtbase5-dev libqt5charts5-dev \
	qttools5-dev-tools libssl-dev qtdeclarative5-dev qtquickcontrols2-5-dev
```

Qt can be installed using the official installer as well.

**Install dependencies (Windows):**

Download and install Qt from https://www.qt.io/download-qt-installer
~~The online installer requires you to make an account, but the offline installer
is a large download.~~ Currently, the only option available is the online installer.
Select one of the Qt versions in the installer
that is compatable with Open-Typer. 

Finish installing Qt, while making sure to select in the
Qt 6.4.3 folder (at the time of writing) Qt 5 Compatability Module, Qt Shader Tools, MinGW
and in the Additional Libraries folder, Qt Charts on the "Select Components" page.

**Build (Windows)**

Open Qt Creator from the start menu, and on the welcome screen, click "Open Project..."
Select the "Open-Typer.pro" file in the window that pops up. Then click "Configure Project", then
wait for it to index the project. Use the shortcut Ctrl + B, click the build icon, or hit Build -> Build Project "Open-Typer"

**Build (Ubuntu)**

```
lrelease *.pro
lrelease app/*.pro
lrelease libcore/*.pro
qmake
make -j$(nproc --all)
```

You may need to use the platform-specifc deploy tool to add shared libraries, for example `windeployqt` for Windows and `macdeployqt` for MacOS.