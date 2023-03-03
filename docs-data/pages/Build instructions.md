# Build instructions

As of writing, Open-Typer (officially) supports Qt 5 (from 5.12 to 5.15) and Qt 6 (6.4).

**Install dependencies (Ubuntu):**

```
sudo apt-get install -y git build-essential qtbase5-dev libqt5charts5-dev \
	qttools5-dev-tools libssl-dev qtdeclarative5-dev qtquickcontrols2-5-dev
```

Qt can be installed using the official installer as well.

**Install dependencies (Windows):**

Download Qt Installer from https://www.qt.io/download-qt-installer
The online installer needs you to make an account, but the offline installer
is a large download.

**Build (Windows)**

Begin the setup. Make sure to select "Custom Installation" in the "Installation Folder" page.
Open the "Qt" dropdown on the "Select Components" page when you get there. Expand the "Developer and Designer Tools" dropdown.
Select "Qt Installer Framework 4.5", then click Next >, and finish the install.
cd into the Qt\Tools\QtInstallerFramework\4.5\bin folder in the command prompt.
Download the https://github.com/Open-Typer/installer repository anywhere, and unzip it. (Or use Git)
Run 
```
set PATH=%PATH%;C:\Qt\Tools\QtInstallerFramework\4.5\bin
```
in the command prompt window, then cd into the folder where you cloned the repository.
Run the 
```
binarycreator --offline-only -c config/config.xml -p packages Open-Typer-installer
```
command to generate an installer. Then to generate the repository, run
```
repogen -p packages repository
```
Open a file explorer window, go into the directory of the installer folder. 
Move the contents of the repository directory (inside the installer folder) one folder up.


**Build (Ubuntu)**

```
lrelease *.pro
lrelease app/*.pro
lrelease libcore/*.pro
qmake
make -j$(nproc --all)
```

You may need to use the platform-specifc deploy tool to add shared libraries, for example `windeployqt` for Windows and `macdeployqt` for MacOS.