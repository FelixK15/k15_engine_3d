include_directories("./include")

file(GLOB OBJLOADER_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB OBJLOADER_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${OBJLOADER_INC})
source_group("src" FILES ${OBJLOADER_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(tinyobjloader STATIC ${OBJLOADER_SRC} ${OBJLOADER_INC})