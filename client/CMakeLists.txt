cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(../proto/include.cmake)

project(puzzlebox_client C CXX)

add_executable(main
	main.cpp
	)

