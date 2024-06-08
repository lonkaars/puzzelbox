if(NOT PICO_PLATFORM STREQUAL "rp2040")
	return()
endif()

target_sources(pbdrv-mod PRIVATE "${CMAKE_CURRENT_LIST_DIR}/mod.c")
target_link_libraries(pbdrv-mod hardware_i2c pico_i2c_slave)

