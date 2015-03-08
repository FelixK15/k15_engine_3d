include_directories(./include)
include_directories(./src)
include_directories(./../os_layer/include)
include_directories(./../logging/include)

set(RENDERER_SRC ./src/K15_RenderContext.cpp)
set(RENDERER_INC ./include/K15_RenderContext.h
				 ./include/K15_RenderPrerequisites.h
				 ./include/K15_RenderCommands.h)

set(RENDERER_LIBS k15math logging)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

if (${K15_RENDERER} STREQUAL "OpenGL")
	include(opengl)
else()
	message(ERROR "Unsupported renderer: ${K15_RENDERER}")
endif()

source_group("include" FILES ${RENDERER_INC})
source_group("src" FILES ${RENDERER_SRC})

if(${K15_PLATFORM} STREQUAL "Win32")
	source_group("include\\wgl" FILES ${PLATFORM_RENDERER_INC})
	source_group("src\\wgl" FILES ${PLATFORM_RENDERER_SRC})
	set(RENDERER_LIBS ${RENDERER_LIBS} opengl32.dll)
elseif(${K15_PLATFORM} STREQUAL "Android")
	source_group("include\\egl" FILES ${PLATFORM_RENDERER_INC})
	source_group("src\\egl" FILES ${PLATFORM_RENDERER_SRC})
elseif(${K15_PLATFORM STREQUAL "Linux")
	source_group("include\\xgl" FILES ${PLATFORM_RENDERER_INC})
	source_group("src\\xgl" FILES ${PLATFORM_RENDERER_SRC})
endif()


add_library(rendering STATIC ${PLATFORM_RENDERER_INC} ${PLATFORM_RENDERER_SRC} ${RENDERER_INC} ${RENDERER_SRC})
target_link_libraries(rendering ${RENDERER_LIBS})