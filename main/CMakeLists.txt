cmake_minimum_required(VERSION 3.29)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(puzzlebox_main C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(PICO_BOARD pico_w)

pico_sdk_init()

add_executable(main 
	main.cpp
	FSM.cpp
)

pico_enable_stdio_usb(main 1)
# pico_enable_stdio_uart(main 1)
pico_add_extra_outputs(main)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(main pico_cyw43_arch_lwip_threadsafe_background pico_stdlib)
