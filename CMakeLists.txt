#CMAKE Version 2.6 required
cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Configuration Parameters	
option(BUILD_STATIC_CORE OFF "Build Static Libraries")

#Project Name
project(k15_engine_3d)

include(common)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Define Subdirectories
add_subdirectory(core)
add_subdirectory(unittest)
add_subdirectory(dependencies)
add_subdirectory(testgame)
add_subdirectory(tools)

message(STATUS "Using Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiling with CXX_FLAGS: ${CMAKE_CXX_FLAGS}")