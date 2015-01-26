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
 */

#include "K15_OpenGL_PrecompiledHeader.h"

#include "K15_OpenGL_DebugUtil.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
	void DebugUtil::logError_ARB(GLenum p_Source, GLenum p_Type, GLuint p_ID, GLenum p_Severity, GLsizei p_Length, const GLchar* p_Message, GLvoid* p_UserParam)
	{
		String msg;

		if(p_Type == GL_DEBUG_TYPE_ERROR_ARB &&
		   (p_Severity == GL_DEBUG_SEVERITY_HIGH/* ||
		    p_Severity == GL_DEBUG_SEVERITY_MEDIUM*/))
		{
			if(p_Source == GL_DEBUG_SOURCE_API)
			{
				msg = "API error:";
			}
			else if(p_Source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)
			{
				msg = "Window system error:";
			}
			else if(p_Source == GL_DEBUG_SOURCE_SHADER_COMPILER)
			{
				msg = "Shader compiler error:";
			}
			else if(p_Source == GL_DEBUG_SOURCE_THIRD_PARTY)
			{
				msg = "Third party error:";
			}
			else if(p_Source == GL_DEBUG_SOURCE_APPLICATION)
			{
				msg = "Application error:";
			}
			else
			{
				msg = "Undefined OGL error:";
			}

			msg += p_Message;

			RendererBase* renderer = (RendererBase*)p_UserParam;
			msg = renderer->getLastError() + "\n" + msg;

			if(renderer)
			{
				renderer->setLastError(msg);
			}
		}
	}
	/*********************************************************************************/
	void DebugUtil::logError_AMD(GLuint p_ID, GLenum p_Category, GLenum p_Severity, GLsizei p_Length, const GLchar* p_Message, GLvoid* p_UserParam)
	{
		String msg;

		if(p_Severity == GL_DEBUG_SEVERITY_HIGH_AMD ||
		   p_Severity == GL_DEBUG_SEVERITY_MEDIUM_AMD)
		{
			if(p_Category == GL_DEBUG_CATEGORY_API_ERROR_AMD)
			{
				msg = "API error:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD)
			{
				msg = "Window system error:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD)
			{
				msg = "Shader compiler error:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_OTHER_AMD)
			{
				msg = "Third party error:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_APPLICATION_AMD)
			{
				msg = "Application error:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_PERFORMANCE_AMD)
			{
				msg = "Performance warning:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD)
			{
				msg = "Undefined Behavior:";
			}
			else if(p_Category == GL_DEBUG_CATEGORY_DEPRECATION_AMD)
			{
				msg = "Deprecation warning:";
			}
			else
			{
				msg = "Undefined OGL error:";
			}

			msg += p_Message;

			RendererBase* renderer = (RendererBase*)p_UserParam;
			msg = renderer->getLastError() + "\n" + msg;

			if(renderer)
			{
				renderer->setLastError(msg);
			}
		}
	}
	/*********************************************************************************/
}}} // end of K15_Engine::Rendering::OpenGL namespace