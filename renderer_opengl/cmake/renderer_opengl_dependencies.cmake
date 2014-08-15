find_library(GLEW_LIB NAMES glew libglew)

if("${GLEW_LIB}" STREQUAL "GLEW_LIB-NOTFOUND")
	message(SEND_ERROR "Could not find library 'glew' ... Missing DEPENDENCY_PATH?")
endif()

set(OGL_DEPENDENCIES ${GLEW_LIB} "k15core")

if(WIN32)
	set(OGL_DEPENDENCIES ${OGL_DEPENDENCIES} "opengl32.dll")
	set(OGL_DEPENDENCIES ${OGL_DEPENDENCIES} "k15math")
endif()