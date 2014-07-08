#CMAKE Version 2.6 required
cmake_minimum_required(VERSION 2.8.10)

#Configuration Parameters	
option(BUILD_ANDROID OFF "Build Android Binaries")
option(BUILD_STATIC_CORE OFF "Build Static Libraries")
set(DEPENDENCY_PATH CACHE STRING "Dependency Directory")

if("${DEPENDENCY_PATH}" STREQUAL "")
	set(DEPENDENCY_PATH "${CMAKE_CURRENT_LIST_DIR}/build_dependencies/")
endif()

message(STATUS "Dependency Path: ${DEPENDENCY_PATH}")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${DEPENDENCY_PATH})

#If build for android has been selected,
#set BUILD_STATIC_CORE to 1 
#(android doesn't work well with shared libs)
if(BUILD_ANDROID)
	set(BUILD_STATIC_CORE 1)
endif()

#Project Name
project(k15_engine_3d)

#Check Which Compiler will be used
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	#GCC
	set(COMPILER_IS_GCC 1)
        if(NOT BUILD_STATIC)
            #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC")
        endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	#Clang
	set(COMPILER_IS_CLANG 1)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	#MSVC
	set(COMPILER_IS_MSVC 1)
	add_definitions(/DK15_BUILD /MP)
endif()

#Check compiler version
if(COMPILER_IS_GCC AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -fPIC -lc -lstdc++")
	set(CPP11_IS_AVAILABLE 1)
elseif(COMPILER_IS_MSVC AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
	set(CPP11_IS_AVAILABLE 1)
elseif(COMPILER_IS_CLANG AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.0)
	set(CPP11_IS_AVAILABLE 1)
endif()

#message whether c++11 is supported or not.
if(CPP11_IS_AVAILABLE)
	message(STATUS "C++11 is available")
else()
	message(STATUS "C++11 is not available")
endif()


message(STATUS "Using Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiling with CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#Define Subdirectories
add_subdirectory(engine)

if(BUILD_ANDROID)
	#opengl es for android
	add_subdirectory(renderer_opengl_es)
else()
	#opengl for the rest
	add_subdirectory(renderer_opengl)
endif()
#add_subdirectory(plugins)
