#src files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

link_directories("${DEPENDENCY_PATH}")

include_directories(../engine/core/include)
include_directories(../engine/rendering/include)
include_directories(../engine/resource_system/include)
include_directories(../engine/serialization/include)
include_directories(../dependencies/glew/include/)
include_directories(../dependencies/glm/include/)
include_directories(./include/)

include(renderer_opengl_compiler_settings)
include(renderer_opengl_src)
include(renderer_opengl_dependencies)

set(RENDERER_OGL_SRC ${RENDERER_OGL_SRC} ${OGL_INC} ${OGL_SRC} ${OGL_WGL_SRC} ${OGL_WGL_INC} ${OGL_GLX_INC} ${OGL_GLX_SRC} ${OGL_GLES2_SRC} ${OGL_GLES2_INC})

#organize source files
source_group("include" FILES ${OGL_INC})
source_group("include\\GLX" FILES ${OGL_GLX_INC})
source_group("include\\WGL" FILES ${OGL_WGL_INC})
source_group("include\\GLES2" FILES ${OGL_GLES2_INC})

source_group("src" FILES ${OGL_SRC})
source_group("src\\GLX" FILES ${OGL_GLX_SRC})
source_group("src\\WGL" FILES ${OGL_WGL_SRC})
source_group("src\\GLES2" FILES ${OGL_GLES2_SRC})


add_library(renderer_opengl SHARED ${RENDERER_OGL_SRC})
target_link_libraries(renderer_opengl ${OGL_DEPENDENCIES})
#target_link_libraries(renderer_opengl LINK_INTERFACE_LIBRARIES)