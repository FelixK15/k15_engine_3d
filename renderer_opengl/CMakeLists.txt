#src files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(../dependencies/glew/include/)
include_directories(./include/)

include(renderer_opengl_src)

set(RENDERER_OGL_SRC ${OGL_INC} ${OGL_SRC})

#organize source files
source_group("include" FILES ${OGL_INC})
source_group("src" FILES ${OGL_SRC})

#specify library
add_library("renderer_opengl" SHARED ${RENDERER_OGL_SRC})

