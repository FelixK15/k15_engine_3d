include_directories(include)
include_directories(src)

include_directories(./../)
include_directories(../math/include)
include_directories(../oslayer/include)
include_directories(../guilayer/include)
include_directories(../logging/include)
include_directories(../string/include)
include_directories(../memory/include)
include_directories(../resourceformats/include)
include_directories(../libshadercompiler/include)
include_directories(../runtime/include)

set(RENDERER_SRC ./src/K15_RenderContext.cpp)
set(RENDERER_INC ./include/K15_RenderContext.h
		 ./include/K15_RenderBackEnd.h
		 ./include/K15_RenderPrerequisites.h
		 ./include/K15_RenderCommands.h
		 ./include/K15_RenderVertexFormatDesc.h
		 ./include/K15_RenderProgramDesc.h
		 ./include/K15_RenderBufferDesc.h
		 ./include/K15_RenderStateDesc.h
		 ./include/K15_RenderStateDesc.h
		 ./include/K15_RenderViewportDesc.h
		 ./include/K15_RenderCameraDesc.h
		 ./include/K15_RenderTextureDesc.h
		 ./include/K15_RenderSamplerDesc.h
		 ./include/K15_RenderTargetDesc.h
		 ./include/K15_RenderFormats.h
		 ./include/K15_RenderGeometryDesc.h
		 ./include/K15_RenderFontDesc.h
		 ./include/K15_RenderShaderSemantics.h)

set(RENDERER_LIBS math logging libshadercompiler runtime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

if (${K15_RENDERER} STREQUAL "OpenGL")
	set(RENDERER_API_INC ./include/OpenGL/K15_RenderGLConfig.h
			./include/OpenGL/K15_RenderGLContext.h
			./include/OpenGL/K15_RenderGLTypes.h)

	include(opengl)
	source_group("include\\gl" FILES ${RENDERER_API_INC})
else()
	message(ERROR "Unsupported renderer: ${K15_RENDERER}")
endif()

source_group("include" FILES ${RENDERER_INC})
source_group("src" FILES ${RENDERER_SRC})

if(${K15_PLATFORM} STREQUAL "Win32")
	source_group("include\\gl\\wgl" FILES ${PLATFORM_RENDERER_INC})
	source_group("src\\gl\\wgl" FILES ${PLATFORM_RENDERER_SRC})
elseif(${K15_PLATFORM} STREQUAL "Android")
	source_group("include\\gl\\egl" FILES ${PLATFORM_RENDERER_INC})
	source_group("src\\gl\\egl" FILES ${PLATFORM_RENDERER_SRC})
	set(RENDERER_LIBS ${RENDERER_LIBS} GLESv2 EGL oslayer)
elseif(${K15_PLATFORM STREQUAL "Linux")
	source_group("include\\gl\\xgl" FILES ${PLATFORM_RENDERER_INC})
	source_group("src\\gl\\xgl" FILES ${PLATFORM_RENDERER_SRC})
endif()


add_library(rendering STATIC ${PLATFORM_RENDERER_INC} ${PLATFORM_RENDERER_SRC} ${RENDERER_API_SRC} ${RENDERER_API_INC} ${RENDERER_INC} ${RENDERER_SRC})
target_link_libraries(rendering ${RENDERER_LIBS})
