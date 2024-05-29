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

Debug only:
- Send arbitrary messages

## Building

PBC is a standard CMake project, but a [makefile](./makefile) is provided for
convenience (still requires CMake and Ninja are installed).

## Send data

```
     ADDRESS  DATA
     v~~~     v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
send 0x39     68:65:6c:6c:6f  44      0x20    'world'    33
              ^~~~~~~~~~~~~~  ^~      ^~~~    ^~~~~~~    ^~
              HEXSTR          NUMBER  NUMBER  STRING     NUMBER
              (binary)        (dec)   (hex)   (literal)  (dec)
```

The data is concatenated, and may contain mixed types of literals

