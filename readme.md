# Puzzle box

This repository contains the source code for the puzzle framework designed and
implemented during the 2023-2024 run of the Puzzlebox project. This year's run
of the project consists of only software students, and was developed using the
hardware from the 21-22 run of the project.

The software in this repository should be easily portable to various other
microcontrollers, and a recommendation is made in the [design
document](docs/design.adoc). Please see the [handover
document](docs/handover.adoc) first for more details.

> [!NOTE]
> This project was mostly developed on Linux. All subfolders/modules/libraries
> use CMake, and *should* build cross-platform when using CMake+Ninja. This has
> not yet been verified.

## Documentation

If you are viewing this page from Doxygen, please take a look at the
[components](topics.html) tab for a comprehensive list of components within
this project.

Project documentation is available in the [docs](docs/) folder, and all code is
documented using Doxygen. To generate HTML docs, run

```
$ make doxygen
```

and open [the generated HTML files](doxygen/html/index.html) in a browser. A
rendered copy of this documentation is also hosted unofficially at
[pipeframe](https://media.pipeframe.xyz/puzzlebox/23-24/doxygen).

## Tidyness

Please keep this repository tidy by being aware of the following conventions:

- An `.editorconfig` file is provided in this repository. Please install the
  [EditorConfig](https://editorconfig.org/) plugin for your text editor of
  choice to automatically use these.
- There are also `.clang-tidy` and `.clang-format` files, which can be enforced
  by running

  ```
  $ make format
  ```

## Libraries

Libraries are stored in the [lib](lib/) folder, and this folder is symlinked to
from each subfolder in the project for convenience (allows CMake to include
out-of-tree modules). The lib folder contains a mix of direct Git submodules
and custom libraries specific to this project. (Most) external dependencies are
tracked as git submodules, exceptions are in the [puzzle](puzzle/) folder.

TL;DR: If something is complaining about missing files

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

