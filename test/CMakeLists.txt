#src files
link_directories("${DEPENDENCY_PATH}")

include_directories(../engine/core/include)
include_directories(../engine/rendering/include)
include_directories(../engine/resource_system/include)
include_directories(../engine/serialization/include)
include_directories(../dependencies/glm/include)

set(TEST_SRC "./src/main.cpp")

message(STATUS "CXX_FLAGS for engine_test: ${CMAKE_CXX_FLAGS}")

add_executable(engine_test ${TEST_SRC})
target_link_libraries(engine_test k15core)
