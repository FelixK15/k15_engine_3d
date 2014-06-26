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

/*
#include <GL/gl.h>
#include <GL/glx.h>
*/
#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
  /*********************************************************************************/
  GLboolean GLAPIENTRY _glxInit(GLuint, GLuint, GLuint);
  GLboolean GLAPIENTRY _glxSwapBuffers(void);
  GLboolean GLAPIENTRY _glxShutdown(void);
  GLvoid*   GLAPIENTRY _glxGetProcAddress(GLchar*);
  /*********************************************************************************/
}}} // end of K15_Engine::Rendering::OpenGL namespace

#endif //_K15Engine_OpenGL_GLX_h_
