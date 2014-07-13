#Set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(engine_core_compiler_settings)
include(engine_core_src)
include(engine_rendering_src)
include(engine_resource_system_src)
include(engine_components_src)
include(engine_serialization_src)
include(engine_core_libraries)

set(ENGINE_SRC "${CORE_SRC}"
"${CORE_WIN32_SRC}"
"${CORE_ANDROID_SRC}"
"${CORE_LINUX_SRC}"
"${CORE_INC}"
"${CORE_WIN32_INC}"
"${CORE_ANDROID_INC}"
"${CORE_LINUX_INC}"
"${RENDERING_SRC}"
"${RENDERING_INC}"
"${RESOURCE_SRC}"
"${RESOURCE_ANDROID_SRC}"
"${RESOURCE_INC}"
"${RESOURCE_ANDROID_INC}"
"${COMPONENTS_SRC}"
"${COMPONENTS_INC}"
"${SERIALIZATION_SRC}"
"${SERIALIZATION_INC}")

#Define Include Directories
include_directories(./core/include/)
include_directories(./rendering/include/)
include_directories(./resource_system/include/)
include_directories(./components/include)
include_directories(./serialization/include)

include_directories(../dependencies/enet/include)
include_directories(../dependencies/detour/include)
include_directories(../dependencies/jsoncpp/include)
include_directories(../dependencies/libtiff/include)
include_directories(../dependencies/minizip/include)
include_directories(../dependencies/zlib/include)
include_directories(../dependencies/tinyobjloader/include)
include_directories(../dependencies/glm/include)

#If we're building a shared library, we need to link all the libraries.
if(NOT BUILD_STATIC_CORE)
	include(engine_core_libraries)
endif()

#Add tinythread dependency if C++11 is not available
if(NOT CPP11_IS_AVAILABLE)
	include_directories(../dependencies/tinythread/include)
endif()

#set precompiler header support for msvc
if(COMPILER_IS_MSVC)
	foreach(SRC_FILE ${ENGINE_SRC})
		string(REGEX MATCH ".*cpp" SRC_CPP ${SRC_FILE})

		if(SRC_CPP)
			set_source_files_properties("${SRC_CPP}" PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} /YuK15_PrecompiledHeader.h")
		endif()

	endforeach()
	
	set_source_files_properties("./core/src/K15_PrecompiledHeader.cpp" PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} /YcK15_PrecompiledHeader.h")
endif()

source_group("core\\include" FILES ${CORE_INC})
source_group("core\\src" FILES ${CORE_SRC})

source_group("core\\include\\win32" FILES ${CORE_WIN32_INC})
source_group("core\\src\\win32" FILES ${CORE_WIN32_SRC})

source_group("core\\include\\android" FILES ${CORE_ANDROID_INC})
source_group("core\\src\\android" FILES ${CORE_ANDROID_SRC})

source_group("core\\include\\linux" FILES ${CORE_LINUX_INC})
source_group("core\\src\\linux" FILES ${CORE_LINUX_SRC})

source_group("resource_system\\include" FILES ${RESOURCE_INC})
source_group("resource_system\\src" FILES ${RESOURCE_SRC})

source_group("resource_system\\include\\android" FILES ${RESOURCE_ANDROID_INC})
source_group("resource_system\\src\\android" FILES ${RESOURCE_ANDROID_SRC})

source_group("rendering\\include" FILES ${RENDERING_INC})
source_group("rendering\\src" FILES ${RENDERING_SRC})

source_group("components\\include" FILES ${COMPONENTS_INC})
source_group("components\\src" FILES ${COMPONENTS_SRC})

source_group("serialization\\include" FILES ${SERIALIZATION_INC})
source_group("serialization\\src" FILES ${SERIALIZATION_SRC})

message(STATUS "CXX_FLAGS for k15core: ${CMAKE_CXX_FLAGS}")

if(BUILD_ANDROID) #When building for android, the core will be statically linked
	add_library(k15core STATIC ${ENGINE_SRC})
else(WIN32)
	add_library(k15core SHARED ${ENGINE_SRC})

	if(WIN32)
            target_link_libraries(k15core winmm.lib)
            target_link_libraries(k15core Ws2_32.lib)
	elseif(UNIX)
	    target_link_libraries(k15core X11)
	    target_link_libraries(k15core GL)
    	endif()
#	target_link_libraries(k15core)
	target_link_libraries(k15core ${ENGINE_LIBS})
	#cmake_policy(PUSH)
	#cmake_policy(SET CMP0022 OLD)
	set_target_properties(k15core PROPERTIES LINK_INTERFACE_LIBRARIES "")
	#cmake_policy(POP)
endif() 
