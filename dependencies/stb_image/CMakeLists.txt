include_directories("./include")

file(GLOB STB_IMAGE_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${STB_IMAGE_INC})

if(COMPILER_IS_GCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

message(STATUS "stb_image CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

add_library(stb_image STATIC ${STB_IMAGE_INC})
