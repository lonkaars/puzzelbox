cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

project(pbdrv C CXX)

add_subdirectory(lib/mpack)

# generic puzzle bus message handling library functions
add_library(pbdrv STATIC
	pb-msg.c
	pb-serial.c
	pb-buf.c
	)
target_link_libraries(pbdrv mpack)
target_include_directories(pbdrv SYSTEM INTERFACE .)

# puzzle bus module specific code (superset of pbdrv)
add_library(pbdrv-mod STATIC
	pb-mod.c
	)
target_link_libraries(pbdrv-mod pbdrv)
target_include_directories(pbdrv-mod SYSTEM INTERFACE .)

# supported puzzle bus drivers
include(drv/arduino/cfg.cmake)
# include(drv/rp2040/cfg.cmake) # please see /main/pbdrv.h

