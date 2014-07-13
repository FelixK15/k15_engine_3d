include_directories("./include")

file(GLOB GLEW_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB ENET_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${ENET_INC})
source_group("src" FILES ${GLEW_SRC})

#we want to build glew statically
add_definitions("-DGLEW_STATIC")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

if(COMPILER_IS_GCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

message(STATUS "glew CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")


add_library(glew STATIC ${GLEW_SRC} ${ENET_INC})
