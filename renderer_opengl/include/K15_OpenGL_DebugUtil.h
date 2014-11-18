/**
 * @file K15_OpenGL_Debug.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/13
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

#include "K15_OpenGL_Prerequisites.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	class DebugUtil
	{
	public:
		static void GLAPIENTRY logError_ARB(GLenum p_Source, GLenum p_Type, GLuint p_ID, GLenum p_Severity, GLsizei p_Length, const GLchar* p_Message, GLvoid* p_UserParam);
		static void GLAPIENTRY logError_AMD(GLuint p_ID, GLenum p_Category, GLenum p_Severity, GLsizei p_Length, const GLchar* p_Message, GLvoid* p_UserParam);
	}; // end of DebugUtil class declaration
}}} // end of K15_Engine::Rendering::OpenGL namespace