include_directories("./include")

file(GLOB TINYTHREAD_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB TINYTHREAD_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${TINYTHREAD_INC})
source_group("src" FILES ${TINYTHREAD_SRC})

if(COMPILER_IS_GCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

message(STATUS "tinythread CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")


add_library(tinythread STATIC ${TINYTHREAD_SRC} ${TINYTHREAD_INC})
