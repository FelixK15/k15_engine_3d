project(binary_cpp_generator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(include/)
include_directories(../../core/)
include_directories(../../core/oslayer/include)
include_directories(../../core/logging/include)
include_directories(../../core/string/include)

set (K15_BINARY_CPP_GENERATOR_SRC "src/main.cpp")

source_group(src FILES ${K15_BINARY_CPP_GENERATOR_SRC})

add_executable(binary_cpp_generator ${K15_BINARY_CPP_GENERATOR_SRC})
target_link_libraries(binary_cpp_generator logging string oslayer)