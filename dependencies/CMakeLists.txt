cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

#new project for the dependencies
project(k15_engine_dependencies)

include(dependencies_common_libs)

if(WIN)
	include(dependencies_win32_libs)
elseif(LINUX)
	include(dependencies_linux_libs)
elseif(ANDROID)
	include(dependencies_android_libs)
endif()