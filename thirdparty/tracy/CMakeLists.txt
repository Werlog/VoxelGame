cmake_minimum_required(VERSION 3.16)

project(tracy)

if(PROFILE_TRACY)
	add_library(tracy STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/TracyClient.cpp")
	target_compile_definitions(tracy PUBLIC TRACY_ENABLE)
	target_compile_definitions(tracy PUBLIC TRACY_NO_SYSTEM_TRACING)

	target_include_directories(tracy PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
else()
	add_library(tracy INTERFACE)

	target_include_directories(tracy INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/")
endif()
