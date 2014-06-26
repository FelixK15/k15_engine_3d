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

#include "K15_Prerequisites.h"

using namespace K15_Engine::Rendering::OpenGL;

#include "glew.h"

#if defined K15_OS_WINDOWS
    #define K15_RENDERER_API __declspec(dllexport)
#endif //K15_OS_WINDOWS

// custom gl functions (per platform)
typedef GLboolean (GLAPIENTRY * PFNK15GLINIT)(GLuint, GLuint, GLuint);
typedef GLboolean (GLAPIENTRY * PFNK15GLSHUTDOWN)(void);
typedef GLboolean (GLAPIENTRY * PFNK15GLSWAPBUFFERS)(void);
typedef GLvoid*   (GLAPIENTRY * PFNGLGETPROCADDRESS)(GLchar*);

#ifdef K15_OS_WINDOWS
    #include "wglew.h"
    #include "WGL/K15_OpenGL_WGL.h"
    PFNK15GLINIT        kglInit             = _wglInit;
    PFNK15GLSWAPBUFFERS kglSwapBuffers      = _wglSwapBuffers;
    PFNK15GLSHUTDOWN    kglShutdown         = _wglShutdown;
    PFNGLGETPROCADDRESS kglGetProcAddress   = _wglGetProcAddress;
#else ifdef K15_OS_LINUX
    #include "glxew.h"
    #include "GLX/K15_OpenGL_GLX.h"
    PFNK15GLINIT        kglInit             = _glxInit;
    PFNK15GLSWAPBUFFERS kglSwapBuffers      = _glxSwapBuffers;
    PFNK15GLSHUTDOWN    kglShutdown         = _glxShutdown;
    PFNGLGETPROCADDRESS kglGetProcAddress   = _glxGetProcAddress;
#endif //K15_OS_WINDOWS

  //experimentals to test perf with different approaches.

#define K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

#ifndef K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
#	define K15_GL_EXPERIMENT_MAP_BUFFER_WITH_UNSYNCHRONIZED
#	define K15_GL_EXPERIMENT_MAP_BUFFER_WITH_INVALIDATE_RANGE
#endif //K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

#endif //_K15Engine_RendererOGL_Prerequisites_h_
