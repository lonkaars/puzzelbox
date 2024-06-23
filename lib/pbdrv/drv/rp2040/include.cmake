if(NOT PICO_PLATFORM STREQUAL "rp2040")
	return()
endif()

target_sources(pbdrv-mod PRIVATE "${CMAKE_CURRENT_LIST_DIR}/mod.c")
target_link_libraries(pbdrv-mod PRIVATE hardware_i2c_headers pico_i2c_slave_headers)
target_include_directories(pbdrv-mod INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

# pico-stdlib is compatible with C stdlib
include("${CMAKE_CURRENT_LIST_DIR}/../../ext/stdlib/include.cmake")

