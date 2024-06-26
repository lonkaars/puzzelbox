\defgroup pbc pbc
\brief Puzzle box client

This folder contains the source code for the puzzle box client (pbc). This is a
desktop application that communicates with the main controller over TCP to
send/receive I<sup>2</sup>C messages. This application is not only used by a
game operator to control and monitor the state of a puzzle box, but is also a
useful debugging tool when developing puzzle modules, as it allows you to send
arbitrary data over the puzzle bus.

> [!NOTE]
> The client depends on the [GNU readline][readline] library, which is not
> included as a submodule because it is installed on most Linux distributions
> by default. Please ensure you have the readline development headers installed
> for your distribution of choice.

## Features

- Individually reset puzzle modules
- Individually skip puzzle modules
- Debug: send arbitrary messages

## Usage

See [command usage](#pbc_cmd_usage)

## Bugs

- tab completion for the `dump` command seems to print garbage (sometimes)

[readline]: https://www.gnu.org/software/readline
