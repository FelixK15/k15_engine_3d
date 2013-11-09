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
#include "GL\glew.h"

using namespace K15_Engine::Rendering;

#if defined K15_OS_WINDOWS
#	if defined K15_OGL_RENDERER_BUILD
#		define K15_OGL_RENDERER_API __declspec(dllexport)
#	else
#		define K15_OGL_RENDERER_API __declspec(dllimport)
#	endif //K15_OGL_RENDERER_BUILD
#endif //K15_OS_WINDOWS


#endif //_K15Engine_RendererOGL_Prerequisites_h_