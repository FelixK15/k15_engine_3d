#cmake_minimum_required(VERSION 2.8.10)
#project(k15_engine_dependencies)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/..")

#include(common)
include(dependencies_common_libs)

if(WIN)
	include(dependencies_win32_libs)
elseif(LINUX)
	include(dependencies_linux_libs)
elseif(ANDROID)
	include(dependencies_android_libs)
endif()
