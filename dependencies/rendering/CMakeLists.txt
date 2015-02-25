include_directories(./include)
include_directories(./src)
include_directories(./../os_layer/include)
include_directories(./../logging/include)

set(RENDERER_SRC ./src/K15_RenderContext.cpp)
set(RENDERER_INC ./include/K15_RenderContext.h
				 ./include/K15_RenderPrerequisites.h
				 ./include/K15_RenderCommands.h)

set(GL_SRC ./src/GL/K15_RenderGLContext.cpp
		   ./src/GL/K15_RenderGLTypes.cpp)

set(GL_INC ./include/GL/K15_RenderGLContext.h
		   ./include/GL/K15_RenderGLTypes.h)

set(GL_WGL_SRC ./src/GL/WGL/K15_Win32RenderWGLContext.cpp)
set(GL_WGL_INC ./include/GL/WGL/K15_Win32RenderWGLContext.h)

set(GL_XGL_SRC ./src/K15_RenderContext.cpp)
set(GL_XGL_INC ./include/K15_RenderContext.h)

set(GL_EGL_SRC ./src/K15_RenderContext.cpp)
set(GL_EGL_INC ./include/K15_RenderContext.h)

source_group("include" FILES ${RENDERER_INC})
source_group("src" FILES ${RENDERER_SRC})

source_group("include\\gl" FILES ${GL_INC})
source_group("src\\gl" FILES ${GL_SRC})

source_group("include\\gl\\wgl" FILES ${GL_WGL_INC})
source_group("src\\gl\\wgl" FILES ${GL_WGL_SRC})

source_group("include\\gl\\xgl" FILES ${GL_XGL_INC})
source_group("src\\gl\\xgl" FILES ${GL_XGL_SRC})

source_group("include\\gl\\egl" FILES ${GL_EGL_INC})
source_group("src\\gl\\egl" FILES ${GL_EGL_SRC})

add_library(rendering STATIC ${RENDERER_SRC} ${GL_SRC} ${GL_WGL_SRC} ${GL_XGL_SRC} ${GL_EGL_SRC}
							 ${RENDERER_INC} ${GL_INC} ${GL_WGL_INC} ${GL_XGL_INC} ${GL_EGL_INC})

target_link_libraries(rendering k15math logging opengl32.dll)
