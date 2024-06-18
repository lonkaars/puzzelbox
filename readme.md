# Puzzle box

This repository contains the source code for the puzzle framework designed and
implemented during the 2023-2024 run of the Puzzlebox project. This year's run
of the project consists of only software students, and was developed using the
hardware from the 21-22 run of the project.

Improved hardware was designed but not realised during the 22-23 run of the
project. This hardware is recommended for future groups participating in the
project. The software in this repository should be easily portable to various
other microcontrollers, and a recommendation is made in the [design
document](docs/design.adoc).

## Tidyness

Please keep this repository tidy by being aware of the following conventions!

### Folder structure

```
/client           desktop PC application for controlling the puzzle box
/docs             project documentation in AsciiDoc(tor) format
/lib              custom libraries and submodules
├───/i2ctcp       I2C over TCP protocol functions (used by main and client)
├───/mpack        MsgPack CMake configuration and extension
└───/pbdrv        puzzle bus driver (module driver + (de)serializing functions)
/main             main controller (RPi pico) software
/puzzle/<name>    puzzle sources, each puzzle has its own subdirectory
/shared           (unused) shared code
/test             unit tests
```

### Code style

An `.editorconfig` file is provided in this repository. Please install the
[EditorConfig](https://editorconfig.org/) plugin for your text editor of choice
to automatically use these.

Currently, no linter/formatter is configured for maintaining consistent code
style.

## Submodules

This repository tracks (most) dependencies via git submodules.

If something is complaining about missing files

```
git submodule update --init --recursive --depth 1
```

until your problems go away.

## lazy\.mk

[`lazy.mk`](./lazy.mk) is a file made by Loek, and includes some rules for
forwarding `make` calls to `cmake` and `ninja`. **This is purely for
convenience, and should not become an essential part of the build system**.
This file should be included at the end of a regular makefile. Any targets
defined in a makefile can be used as-is, while targets that would otherwise be
unknown will be forwarded to Ninja.

