cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

project(pbdrv C CXX)

add_subdirectory(../mpack ${CMAKE_CURRENT_BINARY_DIR}/mpack)

if(DEFINED ARDUINO)
	set(PBDRV_ARDUINO true)
endif()

include_directories(.)
add_library(pbdrv STATIC
	pb-read.c
	pb-write.c
	)
target_link_libraries(pbdrv
	mpack
	)
target_include_directories(pbdrv SYSTEM INTERFACE .)

list(APPEND PBDRV_SRCS pb-mod.c)

if(PBDRV_ARDUINO)
	list(APPEND PBDRV_SRCS drv/arduino/mod.cpp)
endif()

add_library(pbdrv-mod STATIC ${PBDRV_SRCS})
target_link_libraries(pbdrv-mod pbdrv)
# add_dependencies(pbdrv-mod pbdrv)

if(PBDRV_ARDUINO)
	target_link_arduino_libraries(pbdrv-mod
		core
		Wire
		)
endif()

