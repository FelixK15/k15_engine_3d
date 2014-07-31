/**
 * @file K15_RendererOGLPrerequisites.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_RendererOGL_Prerequisites_h_
#define _K15Engine_RendererOGL_Prerequisites_h_

//include order is important here!
#include "K15_Prerequisites.h"

#if defined K15_OS_WINDOWS
	#include "WGL/K15_OpenGL_WGL.h"
    #define K15_RENDERER_API __declspec(dllexport)
	#define K15_DESKTOP_GL
#elif defined K15_OS_LINUX
	#include "GLX/K15_OpenGL_GLX.h"
    #define K15_RENDERER_API
	#define K15_DESKTOP_GL
#elif defined K15_OS_ANDROID || defined K15_OS_IOS
	#include "GLES2/K15_OpenGL_GLES2.h"
    #define K15_RENDERER_API
	#define K15_EMBEDDED_GL
#endif //K15_OS_WINDOWS

#ifdef K15_DESKTOP_GL
    #define K15_MIN_GL_VERSION_MAJOR 3
    #define K15_MIN_GL_VERSION_MINOR 0
    #define K15_MAX_GL_VERSION_MAJOR 4
    #define K15_MAX_GL_VERSION_MINOR 4
#elif defined K15_EMBEDDED_GL
    #define K15_MIN_GL_VERSION_MAJOR 2
    #define K15_MIN_GL_VERSION_MINOR 0
#endif
#include "K15_OpenGL_Extensions.h"


//undef to force the use of emulated extension functions
//#define K15_GL_FORCE_EMULATED_EXTENSIONS

#define K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

#ifdef K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
#	define K15_GL_EXPERIMENT_MAP_BUFFER_WITH_UNSYNCHRONIZED
#	define K15_GL_EXPERIMENT_MAP_BUFFER_WITH_INVALIDATE_RANGE
#endif //K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

#endif //_K15Engine_RendererOGL_Prerequisites_h_
