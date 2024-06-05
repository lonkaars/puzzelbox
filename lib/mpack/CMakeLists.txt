if(TARGET mpack)
	return()
endif()

cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(mpack C)

add_library(mpack STATIC
	src/src/mpack/mpack-common.c
	src/src/mpack/mpack-expect.c
	src/src/mpack/mpack-node.c
	src/src/mpack/mpack-platform.c
	src/src/mpack/mpack-reader.c
	src/src/mpack/mpack-writer.c
	)
target_include_directories(mpack SYSTEM INTERFACE
	src/src/mpack
	)

# causes some wild crashes, please leave off
add_compile_definitions(MPACK_READ_TRACKING=0)

