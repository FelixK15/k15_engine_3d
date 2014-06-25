#Set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(engine_core_src)
include(engine_rendering_src)
include(engine_resource_system_src)
include(engine_components_src)
include(engine_serialization_src)

set(ENGINE_SRC ${CORE_SRC} ${CORE_INC} ${RENDERING_SRC} ${RENDERING_INC} ${RESOURCE_SYSTEM_SRC} ${RESOURCE_SYSTEM_INC} ${COMPONENTS_SRC} ${COMPONENTS_INC} ${SERIALIZATION_SRC} ${SERIALIZATION_INC})

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
include_directories(../dependencies/tinyobjloader/include)
include_directories(../dependencies/glm/include)

if(CPP11_IS_AVAILABLE)
	include_directories(../dependencies/tinythread/include)
endif()

source_group("core\\include" FILES ${CORE_INC})
source_group("core\\src" FILES ${CORE_SRC})

source_group("resource_system\\include" FILES ${RESOURCE_SYSTEM_INC})
source_group("resource_system\\src" FILES ${RESOURCE_SYSTEM_SRC})

source_group("rendering\\include" FILES ${RENDERING_INC})
source_group("rendering\\src" FILES ${RENDERING_SRC})

source_group("components\\include" FILES ${COMPONENTS_INC})
source_group("components\\src" FILES ${COMPONENTS_SRC})

source_group("serialization\\include" FILES ${SERIALIZATION_INC})
source_group("serialization\\src" FILES ${SERIALIZATION_SRC})

if(BUILD_ANDROID) #When building for android, the core will be statically linked
	add_library(core STATIC ${ENGINE_SRC})
else()
	add_library(core SHARED ${ENGINE_SRC})
endif() 
