cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_BUILD_TYPE Debug)

project(pbtest C CXX ASM)

add_executable(test
	i2ctcp/main.cpp
	pbdrv/mod.c
	pbdrv/send.cpp
	pbdrv/mem.cpp
)

add_subdirectory(lib/googletest)
add_subdirectory(lib/pbdrv)
include(lib/pbdrv/ext/stdlib/include.cmake)
add_subdirectory(lib/i2ctcp)

target_link_libraries(test
	gtest_main
	i2ctcp
	mpack
	pbdrv-mod
)

