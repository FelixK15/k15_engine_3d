#CMAKE Version 2.6 required
cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Configuration Parameters	
option(BUILD_ANDROID OFF "Build Android Binaries")
option(BUILD_STATIC_CORE OFF "Build Static Libraries")

#If build for android has been selected,
#set BUILD_STATIC_CORE to 1 
#(android doesn't work well with shared libs)
if(BUILD_ANDROID)
	set(BUILD_STATIC_CORE 1)
endif()

#Project Name
project(k15_engine_3d)

include(common)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Define Subdirectories
add_subdirectory(engine)
add_subdirectory(dependencies)
add_subdirectory(tools)
add_subdirectory(renderer_opengl)
add_subdirectory(test)

set_target_properties(k15core PROPERTIES FOLDER engine)
set_target_properties(renderer_opengl PROPERTIES FOLDER engine)

message(STATUS "Using Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiling with CXX_FLAGS: ${CMAKE_CXX_FLAGS}")