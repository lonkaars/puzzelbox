# i2ctcp (I<sup>2</sup>C over TCP)

This folder includes protocol (de)serialization functions for sending and
receiving I<sup>2</sup>C messages over TCP. These functions are used by the
[main controller](../main) and the [puzzle box client (pbc)](../client). This
folder does not include any puzzle bus specific code, and the headers for
puzbus are in the [shared](../shared) folder instead.

[MessagePack][msgpack] (specifically the [mpack][mpack] implementation) is used
for the actual serialization/deserializtion, and the functions in this folder
act as helpers for parsing from chunked data streams.

To use these functions, include the following statement in your CMakeLists.txt:
```cmake
include(../i2ctcp/include.cmake)
```

The functions are available by `#include`ing the `i2ctcpv1.h` header, and are
extensively documented using Doxygen-style comments.

[msgpack]: https://msgpack.org/
[mpack]: https://github.com/ludocode/mpack/



