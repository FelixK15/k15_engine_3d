#ifndef K15_Render_Prerequisties_h_
#define K15_Render_Prerequisties_h_

#include <K15_OSLayer_Prerequisites.h>

#ifdef K15_OS_WINDOWS
	#include <d3d.h>
	#include "WGL/K15_OpenGL_WGL.h"
	#define K15_DESKTOP_GL
#elif defined K15_OS_LINUX
	#include "XGL/K15_OpenGL_XGL.h"
	#define K15_DESKTOP_GL
#elif defined K15_OS_ANDROID || defined K15_OS_IOS
	#include "EGL/K15_OpenGL_EGL.h"
	#define K15_EMBEDDED_GL
#endif //K15_OS_WINDOWS

#ifdef K15_DESKTOP_GL
	#define K15_MIN_GL_VERSION_MAJOR 3
	#define K15_MIN_GL_VERSION_MINOR 3
#elif defined K15_EMBEDDED_GL
	#define K15_MIN_GL_VERSION_MAJOR 2
	#define K15_MIN_GL_VERSION_MINOR 0
#endif

#include "K15_OpenGL_Extensions.h"

#endif //K15_Render_Prerequisties_h_