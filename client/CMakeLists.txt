cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(puzzlebox_client C CXX)

find_package(Protobuf REQUIRED)

add_executable(main
	main.cpp
	../proto/puzbusv1.proto
	)

set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")
set(CMAKE_STATIC_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")

protobuf_generate(
	APPEND_PATH
	TARGET main
	IMPORT_DIRS ../proto/
	)
target_link_libraries(main PRIVATE protobuf::libprotobuf)
target_include_directories(main PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

