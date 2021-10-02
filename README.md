# Open-Typer

A simple typing tutor program, which aims for **customizability** (see *Configuring*), open source code and ease of use.

## Features

- Lessons consist of levels
- Designed for use in terminals (I'm going to make it possible to use as a cross-platform GUI program later).
- Customizability - custom lessons and levels (see *Configuring*).
- Quick navigation (arrow up (not supported in Windows) or asterisk for level menu).
- Cross-platform - can be compiled for most UNIX-like operating systems and Windows. Windows support is experimental, but most of the features should work.

## How to use

Download a release [here](https://github.com/adazem009/Open-Typer/releases/latest).

Make the file executable:

`chmod +x open-typer`

Start the program:

`./open-typer`

The entire program depends on a config file (see *Configuring*). If it doesn't exist, it'll take a sample config and save it (you can then edit it).

## Configuring

All lessons (and their levels) are saved in a config file called `typer.conf`

Each level is saved in one line. Here are some examples:

- String:

`1.1:0,0 Some text`

Output:

`Some text`

- Repeating string:

`1.2:1,s repeating`

Output:

`repeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeating`

- Repeating word(s):

`1.3:1,w words to repeat`

Output:

`words to repeat words to repeat words to repeat words to repeat words to repeat words to repeat words to repeat words to repeat`

### Config explanation

`1.1` - Lesson 1, level 1

`:0,0` - string (don't repeat anything)

`:1,s` - repeating string

`:1,w` - repeating word(s)

# Building 

**Building on Windows is not supported.**

Install `gcc` and `make` (Debian, Ubuntu):

`sudo apt-get install gcc make`

Optional: If you want to compile for Windows, install the mingw cross-compiler:

`sudo apt-get install gcc-mingw-w64-i686 gcc-mingw-w64-x86-64`

## Building for UNIX-like operating systems

Use `make` like this:

`make`

You can also use one of these commands, which are the same:

```
make PLATFORM=UNIX
make PLATFORM=LINUX
```

## Building for Windows

Use `make` like this:

`make PLATFORM=WIN32` (to build for 32-bit and 64-bit Windows)

`make PLATFORM=WIN64` (to build for 64-bit Windows)

# Known issues

- *none*
