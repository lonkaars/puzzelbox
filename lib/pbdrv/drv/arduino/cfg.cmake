if(NOT DEFINED ARDUINO)
	return()
endif()

target_sources(pbdrv-mod PRIVATE "${CMAKE_CURRENT_LIST_DIR}/mod.cpp")
target_link_arduino_libraries(pbdrv-mod core Wire)

