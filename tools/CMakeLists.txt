#CMAKE Version 2.6 required
#cmake_minimum_required(VERSION 2.8.10)

#project(k15_tools)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(materialtool)
add_subdirectory(shadercompiler)
add_subdirectory(binary_cpp_generator)

set_target_properties(materialtool PROPERTIES FOLDER tools)
set_target_properties(shadercompiler PROPERTIES FOLDER tools)
set_target_properties(binary_cpp_generator PROPERTIES FOLDER tools)