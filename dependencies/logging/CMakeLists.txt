set(LOGGING_INC "include/K15_Logging.h")
set(LOGGING_SRC "src/K15_Logging.cpp")

include_directories(include)

source_group(include FILES ${LOGGING_INC})
source_group(src FILES ${LOGGING_SRC})

add_definitions(-DK15_LOG_ADD_STDOUT_LOGGING)

if(CMAKE_COMPILER_IS_MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
endif()

add_library(logging STATIC ${LOGGING_INC} ${LOGGING_SRC})