project(resourcecompiler)

set(RESOURCECOMPILER_SRC "./src/main.cpp")

set(RESOURCECOMPILER_LIBS libresourcecompiler oslayer)

include_directories(./../)
include_directories(./include)
include_directories(./src)
include_directories(../../core/libresourcecompiler/include/)
include_directories(../../core/oslayer/include/)
include_directories(../../core/logging/include/)
include_directories(../../core/string/include/)
include_directories(../../core/runtime/include/)
include_directories(../../core/rendering/include/)
include_directories(../../core/memory/include/)
include_directories(../../core/math/include/)
include_directories(../../dependencies/assimp/include/)
include_directories(../../dependencies/stb/)
include_directories(../../dependencies/squish/)

source_group("include" FILES ${RESOURCECOMPILER_INC})
source_group("src" FILES ${RESOURCECOMPILER_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
endif()

add_executable(resourcecompiler ${RESOURCECOMPILER_SRC})
target_link_libraries(resourcecompiler ${RESOURCECOMPILER_LIBS})