set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(include/)
include_directories(src/)
include_directories(../oslayer/include/)
include_directories(../logging/include/)

set(K15_RUNTIME_SRC 	"src/K15_ConfigFile.cpp"
			"src/K15_String.cpp"
			"src/K15_GameInit.cpp")

set(K15_RUNTIME_INCLUDE "include/K15_ConfigFile.h"
			"include/K15_String.h"
			"include/K15_GameInit.h")

set(K15_RUNTIME_GENERATED_INC "include/generated/K15_ConfigValueBuffer.h")

set(K15_RUNTIME_COMPLETE_SRC ${K15_RUNTIME_SRC} ${K15_RUNTIME_INCLUDE} ${K15_RUNTIME_GENERATED_INC})

source_group(include FILES ${K15_RUNTIME_INCLUDE})
source_group(src FILES ${K15_RUNTIME_SRC})
source_group(include\\generated FILES ${K15_RUNTIME_GENERATED_INC})
#source_group(src\\generated FILES ${K15_RUNTIME_GENERATED_SRC})

add_library(runtime STATIC ${K15_RUNTIME_COMPLETE_SRC})
