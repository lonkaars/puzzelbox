cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# enable debug features
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG)

project(pbc C CXX)

add_subdirectory(lib/i2ctcp)
add_subdirectory(lib/pbdrv)
include(lib/pbdrv/ext/stdlib/include.cmake)

add_executable(pbc
	main.cpp
	rl.cpp
	sock.cpp
	cmd.cpp
	parse.cpp
	xxd.c
	i2c.cpp
	mod.c
)

target_link_libraries(pbc
	i2ctcp

	# this is such a common library that I did not bother adding it as a submodule
	readline

	# pbdrv-mod is used instead of pbdrv because the client generates messages
	# that are blindly forwarded by the main controller. The message routing
	# logic from pbdrv-mod is not used as the client is not a puzzle module, but
	# it still uses the same PB_MOD_ADDR as the main controller to impersonate
	# the main controller.
	pbdrv-mod
)

