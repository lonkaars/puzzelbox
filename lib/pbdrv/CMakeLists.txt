cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

project(pbdrv C CXX)

add_subdirectory(lib/mpack)
add_compile_definitions(MPACK_HAS_CONFIG=1)
target_include_directories(mpack INTERFACE .) # mpack-config.h

# generic puzzle bus message handling library functions
add_library(pbdrv STATIC
	pb-msg.c
	pb-serial.c
	pb-buf.c
	)
target_include_directories(pbdrv SYSTEM INTERFACE .)
target_compile_definitions(pbdrv PRIVATE PB_TARGET_STDLIB)
target_link_libraries(pbdrv mpack)

# puzzle bus *module* specific code
add_library(pbdrv-mod STATIC
	pb-msg.c
	pb-serial.c
	pb-buf.c
	pb-mod.c
	pb-send.c
	pb-route.c
	)
target_include_directories(pbdrv-mod SYSTEM INTERFACE .)
target_compile_definitions(pbdrv-mod PRIVATE PB_TARGET_FREERTOS)
target_link_libraries(pbdrv-mod
	mpack
	# freertos is used to defer the handling of i2c messages outside the receive
	# interrupt service routine
	freertos_kernel
	freertos_kernel_include
	freertos_config
	)

# supported puzzle bus drivers
include(drv/arduino/cfg.cmake)
# include(drv/rp2040/cfg.cmake) # please see /main/pbdrv.h

