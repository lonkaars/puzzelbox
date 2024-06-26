cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

# arduino
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/lib/Arduino-CMake-Toolchain/Arduino-toolchain.cmake)
set(ARDUINO_BOARD "Arduino Mega or Mega 2560 [avr.mega]")

# freertos
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE .)
set(FREERTOS_PORT GCC_ATMEGA)
set(FREERTOS_HEAP 4)

project(pb_mod_neo C CXX)

add_subdirectory(lib/pbdrv)
add_subdirectory(lib/FreeRTOS-Kernel)

add_executable(main
	main.cpp
	mod.c
)

target_link_libraries(main PUBLIC
	pbdrv-mod
)
target_link_arduino_libraries(main PUBLIC
	core
	Wire
	Adafruit_seesaw
)

# fugly workaround
target_link_arduino_libraries(_arduino_lib_Adafruit_seesaw_Library PUBLIC "Adafruit BusIO")

target_enable_arduino_upload(main)

