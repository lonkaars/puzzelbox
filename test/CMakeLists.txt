cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(pbtest C CXX ASM)

include(../i2ctcp/include.cmake)
include(../shared/pb.cmake)

add_executable(test
	ExampleTest.cpp
	i2ctcp/main.cpp
	)

# enable_testing()
add_subdirectory(lib/googletest)

# target_include_directories(tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(test
	gtest_main
	i2ctcp
	mpack
	)

