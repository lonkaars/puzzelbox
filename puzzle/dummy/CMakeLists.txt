cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

# arduino
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/lib/Arduino-CMake-Toolchain/Arduino-toolchain.cmake)
set(ARDUINO_BOARD "Arduino Uno [avr.uno]")

project(pb_mod_dummy C CXX)

include(../../shared/include.cmake)

add_executable(main
	main.cpp
	)

# target_link_libraries(main
# 	puzbus
# 	# pbdrv-mod-arduino
# 	)
target_link_arduino_libraries(main
	core
	Wire
	)

target_enable_arduino_upload(main)

