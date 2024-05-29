cmake_minimum_required(VERSION 3.29)

project(puzzlebox_test C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(tests
	ExampleTest.cpp
)

enable_testing()

add_subdirectory(lib/googletest)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(tests PRIVATE gtest_main)

add_test(
	NAME tests
	COMMAND tests
)
