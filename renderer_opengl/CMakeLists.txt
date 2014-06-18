if(WIN32)
	set(GL_IMPL "WGL")
endif()

#Throw error if no gl impl has been found
if(NOT GL_IMPL)
	message(ERROR "No GL Impl for this OS.")
endif()

#get src and include files
file(GLOB GL_SRC "./src/*.cpp")
file(GLOB GL_IMPL_SRC "./src/${GL_IMPL}/*.cpp")

file(GLOB GL_INC "./include/*.h")
file(GLOB GL_IMPL_INC "./include/${GL_IMPL}/*.h")

#organize source files
source_group("${GL_IMPL}\\include" FILES ${GL_IMPL_INC})
source_group("${GL_IMPL}\\src" FILES ${GL_IMPL_SRC})

source_group("include" FILES ${GL_INC})
source_group("src" FILES ${GL_SRC})

#specify library
add_library("renderer_opengl" SHARED ${GL_IMPL_INC} ${GL_IMPL_SRC} ${GL_INC} ${GL_SRC})

