# Open-Typer

A simple typing tutor program, which aims for **customizability** (see *Configuring*), open source code and ease of use.

## Features

- Cross-platform - runs on Linux and Windows.
- Customizability - coming soon.
- Quick navigation between lessons and exercises.
- Support for various languages and keyboards - coming soon - please let me know if you can help :)
- No installation needed - just put it on your desktop and it'll work.

## How to use

Download a release [here](https://github.com/adazem009/Open-Typer/releases/latest).

### GNU/Linux
Make the file executable:

`chmod +x Open-Typer`

Start the program:

`./Open-Typer`

**Note:** You might need some Qt dependencies (which should be installed by default on most modern distributions).

## Configuring

All lessons, sublessons and their levels are saved in config files.

**Some config files are in this repository (see the `res/configs` directory).**

Each level is saved in one line. Here are some examples:

- String:

`1.1.1:0,0 Some text`

Output:

`Some text`

- Repeating string:

`1.1.2:1,s repeating`

Output:

`repeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeatingrepeating`

- Repeating word(s):

`1.1.3:1,w words to repeat`

Output:

`words to repeat words to repeat words to repeat words to repeat words to repeat words to repeat words to repeat words to repeat`

- Random sequence of words:

`1.1.4:1,rw word1 word2 word3`

Output:

`word2 word1 word1 word3 word2 word3 word1 ...`

### Config explanation

`1.1.1` - Lesson 1, sublesson 1, level 1

`:0,0` - string (don't repeat anything)

`:1,s` - repeating string

`:1,w` - repeating word(s)

`:1,rw` - random sequence of words

It's possible to create a level, which is more than 1 line long, for example:

`:1,w;3` - 3 lines long level

# Building 

**Building on Windows is not supported.**

Install dependencies (Debian, Ubuntu):

`sudo apt-get install gcc make build-essential qt5-qmake qtbase5-dev`

## Building for UNIX-like operating systems

```
qmake
make
./Open-Typer
```

## Building for Windows

**Cross-compiling for Windows is coming soon.**

# Known issues

- *none*
