**Note:** Building on Windows is not covered on this page, but the procedure is same as with any Qt project.

# Building for GNU/Linux

**Install dependencies (Ubuntu):**

`sudo apt-get install gcc make build-essential qt5-qmake qtbase5-dev`

## Building a standalone binary

```
qmake
make -j$(nproc --all)
```

**Installation:**

`sudo mv Open-Typer /usr/local/bin/`

## Building a DEB package:

`./build-deb.sh`

**Installation**:

`sudo dpkg -i open-typer*.deb`

# Building for Windows

Use the `winbuild.sh` script.

It will download M Cross Environment and build Qt, so the first build will take a while.

`./winbuild.sh`
