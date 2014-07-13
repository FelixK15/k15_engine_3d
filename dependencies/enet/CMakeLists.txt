include_directories("./include")

file(GLOB ENET_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB ENET_INC "include/*.h"
"include/*.hpp"
"include/*.inl"
"include/enet/*.*")

source_group("include" FILES ${ENET_INC})
source_group("src" FILES ${ENET_SRC})

#we want to build enet statically
add_definitions("-DENET_BUILDING_LIB")

if(COMPILER_IS_GCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

message(STATUS "enet CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")


add_library(enet STATIC ${ENET_SRC} ${ENET_INC})
