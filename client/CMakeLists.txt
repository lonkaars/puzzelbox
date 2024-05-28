cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

project(puzzlebox_client C CXX)

include(../i2ctcp/include.cmake)

add_executable(pbc
	main.cpp
	rl.cpp
	sock.cpp
	cmd.cpp
	parse.cpp
	xxd.c
	)

target_link_libraries(pbc
	i2ctcp
	mpack
	readline # this is such a common library that I did not bother adding it as a submodule
	)


