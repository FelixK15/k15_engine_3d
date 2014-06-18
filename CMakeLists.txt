#CMAKE Version 2.6 required
cmake_minimum_required(VERSION 2.8.10)

#Configuration Parameters
set(BUILD_ANDROID OFF CACHE BOOL "Create Android make files")

#Project Name
project(k15_engine_3d)

#Check Which Compiler will be used
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	#GCC
	set(COMPILER_IS_GCC 1)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	#Clang
	set(COMPILER_IS_CLANG 1)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	#MSVC
	set(COMPILER_IS_MSVC 1)
endif()

#Check compiler version
if(COMPILER_IS_GCC AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
	set(CMAKE_CXX_FLAGS "-std=c++11") 
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


#Define Subdirectories
add_subdirectory(engine)
add_subdirectory(dependencies)

if(BUILD_ANDROID)
	#opengl es for android
	add_subdirectory(renderer_opengl_es)
else()
	#opengl for the rest
	add_subdirectory(renderer_opengl)
endif()
#add_subdirectory(plugins)
