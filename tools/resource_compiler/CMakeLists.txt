project(k15-resourcecompiler)

set(RESOURCECOMPILER_SRC "./src/K15_ResourceCompiler_Arguments.cpp"
"./src/K15_ResourceCompiler_Logging.cpp"
"./src/K15_ResourceCompiler_Help.cpp"
"./src/K15_ResourceCompiler_CompilerEnvironment.cpp"
"./src/K15_ResourceCompiler_Compiler.cpp"
"./src/K15_ResourceCompiler_ResourceParameter.cpp"
"./src/K15_ResourceCompiler_ResourceProcessorBase.cpp"
"./src/K15_ResourceCompiler_ResourceProcessorMesh.cpp"
"./src/K15_ResourceCompiler_ResourceProcessorTexture.cpp"
"./src/K15_ResourceCompiler_ResourceProcessorFont.cpp"
"./src/K15_ResourceCompiler_ResourceProcessorEnvironment.cpp"
"./src/K15_ResourceCompiler_StringUtil.cpp"
"./src/K15_ResourceCompiler_SearchUtil.cpp"
"./src/main.cpp")

set(RESOURCECOMPILER_WIN32_SRC "./src/win32/K15_ResourceCompiler_Win32.cpp")
set(RESOURCECOMPILER_WIN32_INC "./include/win32/K15_ResourceCompiler_Win32.h")

set(RESOURCECOMPILER_INC "./include/enums/K15_ResourceCompiler_LogPriority.h"
"./include/enums/K15_ResourceCompiler_FileSearchPattern.h"
"./include/K15_ResourceCompiler_Arguments.h"
"./include/K15_ResourceCompiler_Help.h"
"./include/K15_ResourceCompiler_Logging.h"
"./include/K15_ResourceCompiler_CompilerEnvironment.h"
"./include/K15_ResourceCompiler_Compiler.h"
"./include/K15_ResourceCompiler_ResourceParameter.h"
"./include/K15_ResourceCompiler_ResourceProcessorBase.h"
"./include/K15_ResourceCompiler_ResourceProcessorMesh.h"
"./include/K15_ResourceCompiler_ResourceProcessorTexture.h"
"./include/K15_ResourceCompiler_ResourceProcessorFont.h"
"./include/K15_ResourceCompiler_ResourceProcessorEnvironment.h"
"./include/K15_ResourceCompiler_ResourceData.h"
"./include/K15_ResourceCompiler_StringUtil.h"
"./include/K15_ResourceCompiler_SearchUtil.h"
"./include/K15_ResourceCompiler_Prerequisites.h")

include_directories(./include)
include_directories(../../dependencies/resourceformats/include/)
include_directories(../../dependencies/assimp/include/)
include_directories(../../dependencies/stb/)
include_directories(../../dependencies/squish/)

include_directories(${K15_RESOURCEFORMAT_PROJECT_DIR}/include)

source_group("include\\win32" FILES ${RESOURCECOMPILER_WIN32_INC})
source_group("include" FILES ${RESOURCECOMPILER_INC})

source_group("src\\win32" FILES ${RESOURCECOMPILER_WIN32_SRC})
source_group("src" FILES ${RESOURCECOMPILER_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
endif()

add_executable(resourcecompiler ${RESOURCECOMPILER_SRC} ${RESOURCECOMPILER_INC}
${RESOURCECOMPILER_WIN32_SRC} ${RESOURCECOMPILER_WIN32_INC})

target_link_libraries(resourcecompiler zlib assimp squish resourceformats)