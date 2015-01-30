#CMAKE Version 2.6 required
#cmake_minimum_required(VERSION 2.8.10)

#project(k15_tools)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lua_standalone)
add_subdirectory(resource_compiler)
add_subdirectory(launcher)

set_target_properties(lua_standalone PROPERTIES FOLDER tools)
set_target_properties(resourcecompiler PROPERTIES FOLDER tools)
