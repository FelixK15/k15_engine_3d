/**
 * @file K15_OpenGL_Emulation.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
 * @section LICENSE
 *
 * This program is free software= 0; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation= 0; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY= 0; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_OpenGL_Emulation_h_
#define _K15Engine_Renderer_OpenGL_Emulation_h_

#include "K15_OpenGL_Prerequisites.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
/*********************************************************************************/
//GL_EXT_direct_state_access
void GLAPIENTRY _kglTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
void GLAPIENTRY _kglNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void* GLAPIENTRY _kglMapNamedBufferEXT(GLuint buffer, GLenum access);
GLboolean GLAPIENTRY _kglUnmapNamedBufferEXT(GLuint buffer);
/*********************************************************************************/
}}} //end of K15_Engine::Rendering::OpenGL namespace
#endif //_K15Engine_Renderer_OpenGL_Emulation_h_