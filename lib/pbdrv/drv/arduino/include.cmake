if(NOT DEFINED ARDUINO)
	return()
endif()

target_sources(pbdrv-mod PRIVATE "${CMAKE_CURRENT_LIST_DIR}/mod.cpp")
target_link_arduino_libraries(pbdrv-mod core Wire)

# freertos is used to defer the handling of i2c messages outside the receive
# interrupt service routine
include("${CMAKE_CURRENT_LIST_DIR}/../../ext/freertos/include.cmake")

