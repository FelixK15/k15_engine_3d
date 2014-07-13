include_directories("./include")
include_directories("../zlib/include")

file(GLOB MINIZIP_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB MINIZIP_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${MINIZIP_INC})
source_group("src" FILES ${MINIZIP_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

if(COMPILER_IS_GCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

message(STATUS "minizip CMAKE_CXX_FLAGS: ${CMAKE_C_FLAGS}")


add_library(minizip STATIC ${MINIZIP_SRC} ${MINIZIP_INC})
