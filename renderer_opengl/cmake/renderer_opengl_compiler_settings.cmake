if(COMPILER_IS_MSVC)
	set(CMAKE_CXX_FLAGS "/DGLEW_STATIC /MP")
	#write export def
	set(MODULE_DEF_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/renderer_opengl/module.def")
	file(WRITE ${MODULE_DEF_PATH} "EXPORTS\npluginLoad\npluginUnload\ngetDescription")
	set(RENDERER_OGL_SRC ${MODULE_DEF_PATH})
elseif(COMPILER_IS_GCC)
	set(CMAKE_CXX_FLAGS "-DGLEW_STATIC")
endif()