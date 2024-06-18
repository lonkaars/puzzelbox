cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

project(pbdrv C CXX)

add_subdirectory(lib/mpack)

# mpack-config.h
target_compile_definitions(mpack PRIVATE MPACK_HAS_CONFIG=1)
target_include_directories(mpack PRIVATE .)

# generic puzzle bus message handling library functions
add_library(pbdrv OBJECT
	pb-msg.c
	pb-serial.c
	pb-buf.c
	)
target_include_directories(pbdrv SYSTEM INTERFACE .)
target_link_libraries(pbdrv mpack)

# puzzle bus *module* specific code
add_library(pbdrv-mod OBJECT
	pb-msg.c
	pb-serial.c
	pb-buf.c
	pb-mod.c
	pb-send.c
	pb-route.c
	)
target_include_directories(pbdrv-mod SYSTEM INTERFACE .)
target_link_libraries(pbdrv-mod mpack)

# puzzle bus drivers
include(drv/arduino/include.cmake)
# include(drv/rp2040/include.cmake) # please see /main/pbdrv.h

