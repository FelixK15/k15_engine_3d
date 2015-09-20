project(resourcecompiler)

set(RESOURCECOMPILER_INC "./include/K15_ResourceCompilerShader.h")
set(RESOURCECOMPILER_SRC "./src/main.cpp")

include_directories(./../)
include_directories(./include)
include_directories(./src)
include_directories(../resourceformats/include/)
include_directories(../oslayer/include/)
include_directories(../logging/include/)
include_directories(../string/include/)
include_directories(../runtime/include/)
include_directories(../rendering/include/)
include_directories(../memory/include/)
include_directories(../../dependencies/assimp/include/)
include_directories(../../dependencies/stb/)
include_directories(../../dependencies/squish/)

source_group("include" FILES ${RESOURCECOMPILER_INC})
source_group("src" FILES ${RESOURCECOMPILER_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
endif()

add_executable(resourcecompiler ${RESOURCECOMPILER_SRC})

target_link_libraries(resourcecompiler zlib assimp squish resourceformats oslayer string logging runtime memory)
