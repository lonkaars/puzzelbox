# puzzle box

Avans University of Applied Sciences project puzzle box.

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
