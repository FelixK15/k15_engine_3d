project(materialtool)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(include/)
include_directories(../../core/oslayer/include)
include_directories(../../core/runtime/include)
include_directories(../../core/guilayer/include)
include_directories(../../core/logging/include)
include_directories(../../core/math/include)

set (K15_MATERIALTOOL_SRC "src/K15_MaterialTool.cpp")

source_group(src FILES ${K15_MATERIALTOOL_SRC})

add_executable(materialtool ${K15_MATERIALTOOL_SRC})
target_link_libraries(materialtool guilayer)
