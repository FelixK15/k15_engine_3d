include_directories("./include")

file(GLOB DETOUR_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB DETOUR_INC "include/*.h"
"include/*.hpp"
"include/*.inl")

source_group("include" FILES ${DETOUR_INC})
source_group("src" FILES ${DETOUR_SRC})

add_library(detour STATIC ${DETOUR_SRC} ${DETOUR_INC})