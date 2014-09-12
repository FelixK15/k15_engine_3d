#src files
link_directories("${DEPENDENCY_PATH}")

include_directories(./include)
include_directories(../engine/core/include)
include_directories(../engine/rendering/include)
include_directories(../engine/resource_system/include)
include_directories(../engine/serialization/include)
include_directories(../dependencies/glm/include)

set(TEST_SRC "./src/main.cpp"
"./src/K15_EngineTestModule.cpp")

set(TEST_INC "./include/K15_EngineTestModule.h")

source_group("include" FILES ${TEST_INC})
source_group("src" FILES ${TEST_SRC})

add_executable(engine_test ${TEST_SRC} ${TEST_INC})
target_link_libraries(engine_test k15core)
