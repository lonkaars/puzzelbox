\defgroup pbc pbc
\brief Puzzle box client

# puzzle box client

This folder contains the source code for the puzzle box client (pbc). This is a
desktop application that communicates with the main controller over TCP to
send/receive I<sup>2</sup>C messages. This application is not only used by a
game operator to control and monitor the state of a puzzle box, but is also a
useful debugging tool when developing puzzle modules, as it allows you to send
arbitrary data over the puzzle bus.

## Features

- List detected puzzle modules
- Reset puzzle modules (individually or all to reset the box)
- Skip puzzle modules (individually or all)
- Request puzzle box state
- Debug: send arbitrary messages

## Usage

See \ref pbc_cmd_usage "command usage" for individual command usage.

## WIP TODO

- add enum to string functions in CLIENT ONLY
- bug: tab completion for `dump` seems to print garbage sometimes

