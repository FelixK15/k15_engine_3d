/**
 * @file plugin.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
 */

#ifndef _K15Engine_RendererOGL_Plugin_h_
#define _K15Engine_RendererOGL_Plugin_h_

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_OpenGL_Prerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_ApplicationModuleDescription.h"

/*********************************************************************************/
K15_RENDERER_API void pluginLoad();
/*********************************************************************************/
K15_RENDERER_API void pluginUnload();
/*********************************************************************************/
K15_RENDERER_API K15_Engine::Core::ApplicationModuleDescription getDescription();
/*********************************************************************************/

#endif //_K15Engine_RendererOGL_Plugin_h_