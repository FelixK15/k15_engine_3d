project(k15math)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(math_core_src)

include_directories(./include)

set(MATH_LIB_SRC ${MATH_SRC} ${MATH_INC})

source_group("include" FILES ${MATH_INC})
source_group("src" FILES ${MATH_SRC})

add_library(k15math STATIC ${MATH_LIB_SRC})