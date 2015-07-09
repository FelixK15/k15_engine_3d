project(guilayer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(include/)
include_directories(src/)
include_directories(./../)
include_directories(../oslayer/include/)
include_directories(../math/include/)
include_directories(../runtime/include/)
include_directories(../logging/include/)
include_directories(../memory/include/)
include_directories(../resourceformats/include/)
include_directories(../rendering/include/)

set(K15_GUI_LAYER_SRC 		"src/K15_GUIContext.cpp")
set(K15_GUI_LAYER_INCLUDE 	"include/K15_GUIContext.h")

source_group(src FILES ${K15_GUI_LAYER_SRC})
source_group(include FILES ${K15_GUI_LAYER_INCLUDE})

add_library(guilayer STATIC ${K15_GUI_LAYER_SRC} ${K15_GUI_LAYER_INCLUDE})
