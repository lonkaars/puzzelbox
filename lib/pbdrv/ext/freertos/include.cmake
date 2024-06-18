target_sources(pbdrv-mod PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/pb-mem.c"
	"${CMAKE_CURRENT_LIST_DIR}/pb-mod.c"
	)
target_link_libraries(pbdrv-mod
	freertos_kernel
	freertos_kernel_include
	freertos_config
	)

