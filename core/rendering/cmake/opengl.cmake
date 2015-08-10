if (${K15_RENDERER_VERSION} STREQUAL "3" OR ${K15_RENDERER_VERSION} STREQUAL "2")
	set(RENDERER_FOUND TRUE)
else()
	message(ERROR "Unsupported OpenGL Version: ${K15_RENDERER_VERSION}")
endif()


if (${RENDERER_FOUND})
	#Add Platform specific files
	if (${K15_PLATFORM} STREQUAL "Win32")
		set(PLATFORM_RENDERER_INC ./include/OpenGL/WGL/K15_Win32RenderWGLContext.h)
		set(PLATFORM_RENDERER_SRC ./src/OpenGL/WGL/K15_Win32RenderWGLContext.cpp)
	elseif(${K15_PLATFORM} STREQUAL "Android")
		set(PLATFORM_RENDERER_INC ./include/OpenGL/EGL/K15_AndroidRenderEGLContext.h)
		set(PLATFORM_RENDERER_SRC ./src/OpenGL/EGL/K15_AndroidRenderEGLContext.cpp)
	elseif(${K15_PLATFORM} STREQUAL "Linux")
		set(PLATFORM_RENDERER_INC ./include/OpenGL/GLX/K15_X11RenderXGLContext.h)
		set(PLATFORM_RENDERER_SRC ./src/OpenGL/GLX/K15_X11RenderXGLContext.cpp)
	endif()

	if (${K15_RENDERER} STREQUAL "OGL")
		set(RENDERER_API_SRC 	./src/OpenGL/K15_RenderGLContext.cpp
					./src/OpenGL/K15_RenderGLTypes.cpp)

		set(RENDERER_API_INC 	./include/OpenGL/K15_RenderGLContext.h
					./include/OpenGL/K15_RenderGLConfig.h
					./include/OpenGL/K15_RenderGLTypes.h)
	endif()
endif()

