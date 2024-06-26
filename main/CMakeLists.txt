cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(PICO_BOARD pico_w)

include(lib/pico-sdk/pico_sdk_init.cmake)
include(lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
add_subdirectory(lib/mpack)
add_subdirectory(lib/i2ctcp)
add_subdirectory(lib/pbdrv)

project(puzzlebox_main C CXX ASM)

pico_sdk_init()

add_executable(main
	main.c
	init.c
	sock.c
	i2c.c
	mod.c
	tasks.c
	blink.c
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
pico_add_extra_outputs(main)

# include_directories(lib/pico-sdk/lib/lwip/contrib/ports/freertos/include)

target_include_directories(main PRIVATE .)
target_link_libraries(main PUBLIC
	pico_cyw43_arch_lwip_sys_freertos
	pico_stdlib
	pico_i2c_slave
	hardware_i2c
	FreeRTOS-Kernel
	FreeRTOS-Kernel-Heap4
	i2ctcp
	mpack
	pbdrv-mod
)

