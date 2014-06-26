include_directories("./include")

file(GLOB TINYTHREAD_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB TINYTHREAD_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${TINYTHREAD_INC})
source_group("src" FILES ${TINYTHREAD_SRC})

add_library(tinythread STATIC ${TINYTHREAD_SRC} ${TINYTHREAD_INC})