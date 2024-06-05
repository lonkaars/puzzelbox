if(DEFINED ARDUINO)
	set(PBDRV_ARDUINO true)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR})

list(APPEND PBDRV_SRCS "${CMAKE_CURRENT_LIST_DIR}/pb/moddrv.c")

if(PBDRV_ARDUINO)
	list(APPEND PBDRV_SRCS "${CMAKE_CURRENT_LIST_DIR}/pb/drv/arduino/mod.cpp")
endif()

add_library(pbdrv-mod STATIC ${PBDRV_SRCS})

if(PBDRV_ARDUINO)
	target_link_arduino_libraries(pbdrv-mod
		core
		Wire
		)
endif()
