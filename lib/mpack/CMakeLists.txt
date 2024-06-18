if(TARGET mpack)
	return()
endif()

cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# set(CMAKE_BUILD_TYPE Debug)

project(mpack C)

add_library(mpack STATIC
	src/src/mpack/mpack-common.c
	src/src/mpack/mpack-expect.c
	src/src/mpack/mpack-node.c
	src/src/mpack/mpack-platform.c
	src/src/mpack/mpack-reader.c
	src/src/mpack/mpack-writer.c
	read-remaining.c
	)
target_include_directories(mpack SYSTEM INTERFACE
	.
	src/src/mpack
	)

# causes some wild crashes, please leave off
target_compile_definitions(mpack PRIVATE MPACK_READ_TRACKING=0)

