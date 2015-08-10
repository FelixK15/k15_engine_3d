set(K15_UNIT_TEST_SRC "./src/K15_UnitTest.cpp")

source_group(src FILES ${K15_UNIT_TEST_SRC})

include_directories(./src)
include_directories(./../)
include_directories(./../core/logging/include)
include_directories(./../core/string/include)
include_directories(./../core/runtime/include)
include_directories(./../core/oslayer/include)
include_directories(./../core/math/include)

add_executable(unittest ${K15_UNIT_TEST_SRC})

target_link_libraries(unittest logging string oslayer runtime math)