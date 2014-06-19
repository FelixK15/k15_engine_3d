cmake_minimum_required(VERSION 2.8.10)

#new project for the dependencies
project(k15_engine_dependencies)

#if we're building for android, search the android directory for CMakeLists.txt
if(BUILD_ANDROID)
	add_subdirectory(android)
endif()

#create list of dependencies
set(DEPENDENCY_LIST 	"detour"
			"enet"
			"glew"
			"jsoncpp"
			"libtiff"
			"minizip"
			"zlib")

#add tinythread if no cpp11 is available
if(NOT CPP11_IS_AVAILABLE)
	set(DEPENDENCY_LIST ${DEPENDENCY_LIST} "tinythread")
endif()

#iterate through list of dependencies
foreach(DEPENDENCY ${DEPENDENCY_LIST})
	#gather src and includes files per dependency
	file(GLOB DEPENDENCY_SRC "${DEPENDENCY}/src/*.cpp"
				 "${DEPENDENCY}/src/*.c")

	file(GLOB DEPENDENCY_INC "${DEPENDENCY}/include/*.h"
				 "${DEPENDENCY}/include/*.hpp"
				 "${DEPENDENCY}/include/*.inl")

	#add include folder
	include_directories("${DEPENDENCY}/include/")

	#organizing dependency include and src files into groups
	if(DEPENDENCY_SRC)
		source_group("${DEPENDENCY}\\include" FILES ${DEPENDENCY_INC})
	endif()

	if(DEPENDENCY_INC)
		source_group("${DEPENDENCY}\\src" FILES ${DEPENDENCY_SRC})
	endif()

	#add dependency library to the project
	add_library(${DEPENDENCY} STATIC ${DEPENDENCY_SRC} ${DEPENDENCY_INC}) 
endforeach(DEPENDENCY)
