include_directories(${CMAKE_CURRENT_LIST_DIR})
add_library(puzbus STATIC
	${CMAKE_CURRENT_LIST_DIR}/pb/driver.c
	)

