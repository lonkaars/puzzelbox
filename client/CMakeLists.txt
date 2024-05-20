cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(puzzlebox_client C CXX)

include(../proto/include.cmake)

add_executable(main
	main.cpp
	rl.cpp
	sock.cpp
	)

target_link_libraries(main
	puzbus
	mpack
	readline # this is such a common library that I did not bother adding it as a submodule
	)


