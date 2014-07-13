include_directories("./include")

file(GLOB LIBTIFF_SRC "src/*.cpp"
"src/*.c"
"src/*.cc"
"src/*.cxx")

file(GLOB LIBTIFF_INC "include/*.h"
"include/*.hpp"
"include/*.inl"
"include/*.hxx")

source_group("include" FILES ${LIBTIFF_INC})
source_group("src" FILES ${LIBTIFF_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

if(COMPILER_IS_GCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

message(STATUS "libtiff CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")


add_library(libtiff STATIC ${LIBTIFF_SRC} ${LIBTIFF_INC})
