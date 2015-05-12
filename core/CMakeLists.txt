#CMAKE Version 2.6 required
cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

#Define Subdirectories
add_subdirectory(launcher)
add_subdirectory(logging)
add_subdirectory(math)
add_subdirectory(oslayer)
add_subdirectory(rendering)
add_subdirectory(resourcecompiler)
add_subdirectory(resourceformats)
add_subdirectory(runtime)

set_target_properties(launcher PROPERTIES FOLDER core)
set_target_properties(logging PROPERTIES FOLDER core)
set_target_properties(math PROPERTIES FOLDER core)
set_target_properties(oslayer PROPERTIES FOLDER core)
set_target_properties(rendering PROPERTIES FOLDER core)
set_target_properties(resourcecompiler PROPERTIES FOLDER core)
set_target_properties(resourceformats PROPERTIES FOLDER core)
set_target_properties(runtime PROPERTIES FOLDER core)
