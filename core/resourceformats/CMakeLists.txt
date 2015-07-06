cmake_minimum_required(VERSION 2.8.10)

project(k15_resourceformats)

include_directories(./include)
include_directories(./../../dependencies/zlib/include)

set(RF_INC 		"./include/K15_MeshFormat.h"
				"./include/K15_MaterialFormat.h"
				"./include/K15_TextureFormat.h"
				"./include/K15_HeaderFormat.h"
				"./include/K15_FontFormat.h"
				"./include/K15_DataAccessHelper.h"
				"./include/K15_ResourceFormats_Prerequisites.h"
				"./include/K15_ResourceFormats_Config.h")

set(RF_ENUMS	"./include/enums/K15_FormatValues.h"
				"./include/enums/K15_ReturnValues.h")

set(RF_SRC 		"./src/K15_MeshFormat.cpp"
				"./src/K15_DataAccessHelper.cpp"
				"./src/K15_MaterialFormat.cpp"
				"./src/K15_FontFormat.cpp"
				"./src/K15_TextureFormat.cpp"
				"./src/K15_HeaderFormat.cpp")

source_group("include\\enums" FILES ${RF_ENUMS})
source_group("include" FILES ${RF_INC})
source_group("src" FILES ${RF_SRC})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(resourceformats ${RF_INC} ${RF_ENUMS} ${RF_SRC})