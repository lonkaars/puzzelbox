cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(i2ctcp C CXX)

add_subdirectory(lib/mpack)

add_library(i2ctcp STATIC i2ctcpv1.c)
target_link_libraries(i2ctcp mpack)
target_include_directories(i2ctcp SYSTEM INTERFACE .)

