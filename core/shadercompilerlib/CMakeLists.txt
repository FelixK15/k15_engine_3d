project(shadercompilerlib)

include_directories(./../)
include_directories(./include)
include_directories(./../string/include)
include_directories(./../../dependencies/zlib/include)

set(SC_INC 		"./include/K15_ShaderCompiler.h")
set(SC_SRC 		"./src/K15_ShaderCompiler.cpp")

source_group("include" FILES ${SC_INC})
source_group("src" FILES ${SC_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(shadercompilerlib STATIC ${SC_INC} ${SC_SRC})
