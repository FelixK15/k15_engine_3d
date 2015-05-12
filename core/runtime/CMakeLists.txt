set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(include/)
include_directories(../oslayer/include/)

set(K15_RUNTIME_SRC 	"src/K15_ConfigFile.cpp")
set(K15_RUNTIME_INCLUDE "include/K15_ConfigFile.h")
set(K15_RUNTIME_COMPLETE_SRC ${K15_RUNTIME_SRC} ${K15_RUNTIME_INCLUDE})

source_group(include FILES ${K15_RUNTIME_INCLUDE})
source_group(src FILES ${K15_RUNTIME_SRC})

add_library(runtime STATIC ${K15_RUNTIME_COMPLETE_SRC})