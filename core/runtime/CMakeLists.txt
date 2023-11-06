set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(./../)
include_directories(./include/)
include_directories(./src/)
include_directories(./../math/include/)
include_directories(./../rendering/include/)
include_directories(./../resourceformats/include/)
include_directories(./../oslayer/include/)
include_directories(./../memory/include/)
include_directories(./../logging/include/)
include_directories(./../string/include/)
include_directories(./../libresourcecompiler/include/)

set(K15_RUNTIME_SRC 	"src/K15_ConfigFile.cpp"
			"src/K15_ResourceContext.cpp"
			"src/K15_GameInit.cpp"
			"src/K15_FrameCounter.cpp"
			"src/K15_Mesh.cpp"
			"src/K15_AsyncOperation.cpp")

set(K15_RUNTIME_INCLUDE "include/K15_ConfigFile.h"
			"include/K15_ResourceContext.h"
			"include/K15_Config_RT.h"
			"include/K15_Mesh.h"
			"include/K15_RuntimePrerequisites.h"
			"include/K15_GameInit.h"
			"include/K15_FrameCounter.h"
			"include/K15_AsyncOperation.h")

set(K15_RUNTIME_GENERATED_INC "include/generated/K15_ConfigValueBuffer.h")

set(K15_RUNTIME_COMPLETE_SRC ${K15_RUNTIME_SRC} ${K15_RUNTIME_INCLUDE} ${K15_RUNTIME_GENERATED_INC})

source_group(include FILES ${K15_RUNTIME_INCLUDE})
source_group(src FILES ${K15_RUNTIME_SRC})
source_group(include\\generated FILES ${K15_RUNTIME_GENERATED_INC})
#source_group(src\\generated FILES ${K15_RUNTIME_GENERATED_SRC})

add_library(runtime STATIC ${K15_RUNTIME_COMPLETE_SRC})
target_link_libraries(runtime libresourcecompiler)