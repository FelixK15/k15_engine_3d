set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(./../)
include_directories(./include)
include_directories(./src)

set(K15_MEMORY_SRC "./src/K15_Memory.cpp")
set(K15_MEMORY_INC 	"./include/K15_MemoryBuffer.h"
					"./include/K15_MemoryPrerequisites.h"
					"./include/K15_CustomMemoryAllocator.h")

source_group(src FILES ${K15_MEMORY_SRC})
source_group(include FILES ${K15_MEMORY_INC})

add_library(memory STATIC ${K15_MEMORY_SRC} ${K15_MEMORY_INC})