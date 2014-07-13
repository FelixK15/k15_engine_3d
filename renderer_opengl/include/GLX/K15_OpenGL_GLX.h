/**
 * @file K15_OpenGL_GLX.h
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

#ifndef _K15Engine_OpenGL_GLX_h_
#define _K15Engine_OpenGL_GLX_h_

#include "glew.h"
#include "glxew.h"

// custom gl functions
typedef GLboolean (*PFNK15GLINIT)(GLint, GLint, GLint);
typedef GLboolean (*PFNK15GLSHUTDOWN)();
typedef GLboolean (*PFNK15GLSWAPBUFFERS)();
typedef GLvoid*	  (*PFNGLGETPROCADDRESS)(GLchar*);

extern PFNK15GLINIT        kglInit;
extern PFNK15GLSWAPBUFFERS kglSwapBuffers;
extern PFNK15GLSHUTDOWN    kglShutdown;
extern PFNGLGETPROCADDRESS kglGetProcAddress;

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
    GLboolean _glxInit(GLint, GLint, GLint);
    GLboolean _glxSwapBuffers();
    GLboolean _glxShutdown();
	GLvoid*   _glxGetProcAddress(GLchar*);
	/*********************************************************************************/
}}} //end of K15_Engine::Rendering::OpenGL namespace

#endif //_K15Engine_OpenGL_GLX_h_
