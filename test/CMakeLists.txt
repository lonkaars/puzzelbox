cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(pbtest C CXX ASM)

add_executable(test
	i2ctcp/main.cpp
	pbdrv/main.cpp
	)

add_subdirectory(lib/googletest)
add_subdirectory(lib/pbdrv)
add_subdirectory(lib/i2ctcp)

target_link_libraries(test
	gtest_main
	i2ctcp
	mpack
	pbdrv
	)

