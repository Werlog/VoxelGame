#https://github.com/meemknight/SDL2OpenglCMakeSetup

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build" OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

if(MSVC) 
add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

project(VoxelGame)

set(SDL_STATIC ON)

add_subdirectory(thirdparty/SDL-release-2.30.5)		#window opener
add_subdirectory(thirdparty/glad)			#opengl loader
add_subdirectory(thirdparty/stb_image)			#loading images
add_subdirectory(thirdparty/glm)			#math
add_subdirectory(thirdparty/imgui-docking)		#ui
add_subdirectory(thirdparty/FastNoiseSIMD)
add_subdirectory(thirdparty/freetype) #loading ttf files
add_subdirectory(thirdparty/enet)


file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cpp")
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp")

# SHARED_SOURCES holds the source files that are used in both executables
file(GLOB_RECURSE SHARED_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")
add_executable(VoxelGameServer)

# To ensure the server is built aswell when trying to run the client
add_dependencies("${CMAKE_PROJECT_NAME}" VoxelGameServer)

# BUILD SERVER

set_property(TARGET VoxelGameServer PROPERTY CXX_STANDARD 17)

target_sources(VoxelGameServer PRIVATE ${SERVER_SOURCES})
target_sources(VoxelGameServer PRIVATE ${SHARED_SOURCES})

target_include_directories(VoxelGameServer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/server/")
target_include_directories(VoxelGameServer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/shared/")

target_link_libraries(VoxelGameServer PRIVATE glm enet fastNoiseSIMD)


# BUILD CLIENT

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 
 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

endif()
if(MSVC) # If using the VS compiler...

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	#add this line if you want to remove the console!
	#set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
	
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${CLIENT_SOURCES})
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${SHARED_SOURCES})

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/client/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/shared/")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm 
	glad stb_image imgui SDL2-static fastNoiseSIMD freetype enet)
