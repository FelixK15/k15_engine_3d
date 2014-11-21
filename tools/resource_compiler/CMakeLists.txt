cmake_minimum_required(VERSION 2.8.10)
project(k15-resourcecompiler)

set(CMAKE_MODULE_PATH 
"${CMAKE_CURRENT_LIST_DIR}/dependencies")

set(RESOURCECOMPILER_SRC "./src/K15_ResourceCompiler_Arguments.cpp"
"./src/K15_ResourceCompiler_Logging.cpp"
"./src/K15_ResourceCompiler_Help.cpp"
"./src/main.cpp")

set(RESOURCECOMPILER_INC "./include/enums/K15_ResourceCompiler_LogPriority.h"
"./include/K15_ResourceCompiler_Arguments.h"
"./include/K15_ResourceCompiler_Help.h"
"./include/K15_ResourceCompiler_Logging.h"
"./include/K15_ResourceCompiler_Prerequisites.h")

include_directories(./include)

source_group("include" FILES ${RESOURCECOMPILER_INC})
source_group("src" FILES ${RESOURCECOMPILER_SRC})

add_executable(k15resourcecompiler ${RESOURCECOMPILER_SRC} ${RESOURCECOMPILER_INC})