include_directories("./include")

file(GLOB DETOUR_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB DETOUR_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${DETOUR_INC})
source_group("src" FILES ${DETOUR_SRC})

if(COMPILER_IS_GCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

message(STATUS "Detour CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

add_library(detour STATIC ${DETOUR_SRC} ${DETOUR_INC})
