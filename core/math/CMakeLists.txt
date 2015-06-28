project(math)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#"${CMAKE_CURRENT_LIST_DIR}/..")

#include(common)
include(math_core_src)
include(math_compiler_settings)

include_directories(./include)
include_directories(./src)

set(MATH_LIB_SRC ${MATH_SRC} ${MATH_INC})

source_group("include" FILES ${MATH_INC})
source_group("src" FILES ${MATH_SRC})

add_library(math STATIC ${MATH_LIB_SRC})
