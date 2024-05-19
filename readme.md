# puzzle box

Avans University of Applied Sciences project puzzle box.

## tidyness

Please keep this repository tidy by being aware of the following conventions!

### folder structure

|folder|contains|
|-|-|
|`/client`|Desktop PC application for controlling the puzzle box
|`/docs`|Project documentation in AsciiDoc(tor) format
|`/lib`|Libraries (tracked as [submodules](#submodules))
|`/main`|Main controller (RPi pico) software
|`/proto`|Puzzle bus TCP protocol functions (used by main and client)
|`/puzzle/<name>`|Puzzle sources, each puzzle has its own subdirectory
|`/shared`|Auxiliary shared code
|`/test`|Unit test framework (currently unutilized)

### code style

An `.editorconfig` file is provided in this repository. Please install the
[EditorConfig](https://editorconfig.org/) plugin for your text editor of choice
to automatically use these.

Currently, no linter/formatter is configured for maintaining consistent code
style.

## submodules

This repository tracks (most) dependencies via git submodules.

If something is complaining about missing files

```
git submodule update --init --recursive --depth 1
```

until your problems go away.

## Tests

```
mkdir -p test/build
cd test/build
cmake ..
make
make test
```

## ESP
1. Install ESP-IDF extension in vscode
2. Install using 'express' option
3. Install ESP-IDF v5.2.1 (release version)
4. For windows: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html#get-started-windows-first-steps
5. For Linux: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps
