include_directories("./include")

file(GLOB ENET_SRC "src/*.cpp"
"src/*.c"
"src/*.cc")

file(GLOB ENET_INC "include/*.h"
"include/*.hpp"
"include/*.inl"
"include/enet/*.*")

source_group("include" FILES ${ENET_INC})
source_group("src" FILES ${ENET_SRC})

#we want to build enet statically
add_definitions("-DENET_BUILDING_LIB")

add_library(enet STATIC ${ENET_SRC} ${ENET_INC})