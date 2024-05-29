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

|folder|contains|
|-|-|
|`/client`|Desktop PC application for controlling the puzzle box
|`/docs`|Project documentation in AsciiDoc(tor) format
|`/i2ctcp`|I<sup>2</sup>C over TCP protocol functions (used by main and client)
|`/lib`|Libraries (tracked as [submodules](#submodules))
|`/main`|Main controller (RPi pico) software
|`/puzzle/<name>`|Puzzle sources, each puzzle has its own subdirectory
|`/shared`|Shared code
|`/test`|Unit test framework (currently unutilized)

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

<!--
## Tests

```
mkdir -p test/build
cd test/build
cmake ..
make
make test
```
-->

## ESP SDK setup

1. Install ESP-IDF extension in Visual Studio Code
2. Install using 'express' option
3. Install ESP-IDF v5.2.1 (release version)

   Additional help:
   - [For windows](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html#get-started-windows-first-steps)
   - [For Linux](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps)

