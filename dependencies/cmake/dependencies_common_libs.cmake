set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/libs")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(assimp)
add_subdirectory(zlib)
add_subdirectory(squish)

set_target_properties(assimp PROPERTIES FOLDER dependencies)
set_target_properties(zlib PROPERTIES FOLDER dependencies)
set_target_properties(squish PROPERTIES FOLDER dependencies)
