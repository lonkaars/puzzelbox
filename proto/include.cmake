include_directories(${CMAKE_CURRENT_LIST_DIR})

# mpack
include_directories(${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack)
add_library(mpack STATIC
	${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack/mpack-common.c
	${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack/mpack-expect.c
	${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack/mpack-node.c
	${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack/mpack-platform.c
	${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack/mpack-reader.c
	${CMAKE_CURRENT_LIST_DIR}/lib/mpack/src/mpack/mpack-writer.c
	)

