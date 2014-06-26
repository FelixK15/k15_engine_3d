include_directories("./include")

file(GLOB JSONCPP_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB JSONCPP_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${JSONCPP_INC})
source_group("src" FILES ${JSONCPP_SRC})

#we want to build jsoncpp statically
add_definitions("-D_LIB")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(jsoncpp STATIC ${JSONCPP_SRC} ${JSONCPP_INC})