set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(./include)

set(K15_STRING_SRC "./src/K15_String.cpp")
set(K15_STRING_INC "./include/K15_String.h")

source_group(src FILES ${K15_STRING_SRC})
source_group(include FILES ${K15_STRING_INC})

add_library(string STATIC ${K15_STRING_SRC} ${K15_STRING_INC})