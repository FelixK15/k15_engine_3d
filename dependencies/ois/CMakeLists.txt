include_directories("./include")

file(GLOB OIS_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB OIS_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

file(GLOB OIS_WIN_SRC "src/win32/*.cpp")
file(GLOB OIS_WIN_INC "include/win32/*.h")

file(GLOB OIS_LINUX_SRC "src/linux/*.cpp")
file(GLOB OIS_LINUX_INC "include/linux/*.h")

source_group("include" FILES ${OIS_INC})
source_group("src" FILES ${OIS_SRC})

source_group("include\\win32" FILES ${OIS_WIN_INC})
source_group("src\\win32" FILES ${OIS_WIN_SRC})

source_group("include\\linux" FILES ${OIS_LINUX_INC})
source_group("src\\linux" FILES ${OIS_LINUX_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

if(COMPILER_IS_GCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

message(STATUS "ois CMAKE_CXX_FLAGS: ${CMAKE_C_FLAGS}")


add_library(ois STATIC ${OIS_SRC} ${OIS_INC} ${OIS_WIN_INC} ${OIS_WIN_SRC} ${OIS_LINUX_INC} ${OIS_LINUX_SRC})
