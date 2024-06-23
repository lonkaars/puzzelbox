target_sources(pbdrv PRIVATE "${CMAKE_CURRENT_LIST_DIR}/pb-mem.c")
target_link_libraries(pbdrv PUBLIC
	freertos_kernel
	freertos_kernel_include
	freertos_config
	)

