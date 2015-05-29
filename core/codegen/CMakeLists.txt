set(K15_CODEGEN_SRC "./src/main.cpp"
					"./src/K15_CodeGenerator.cpp"
					"./src/K15_ArgumentParser.cpp")

set(K15_CODEGEN_INC "./include/K15_CodeGenerator.h"
					"./include/K15_ArgumentParser.h")

set(K15_CODEGEN_CODE ${K15_CODEGEN_SRC} ${K15_CODEGEN_INC})

source_group(src FILES ${K15_CODEGEN_SRC})
source_group(include FILES ${K15_CODEGEN_INC})

include_directories(./include)
include_directories(./src)
include_directories(./../oslayer/include)
include_directories(./../logging/include)

add_executable(codegen ${K15_CODEGEN_CODE})

target_link_libraries(codegen logging oslayer)