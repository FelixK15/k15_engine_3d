include_directories("./include")

file(GLOB ZLIB_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB ZLIB_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${ZLIB_INC})
source_group("src" FILES ${ZLIB_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(zlib STATIC ${ZLIB_SRC} ${ZLIB_INC})