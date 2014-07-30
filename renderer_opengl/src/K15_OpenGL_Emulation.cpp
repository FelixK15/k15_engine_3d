/**
 * @file K15_OpenGL_Emulation.cpp
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
 */

#include "K15_OpenGL_Emulation.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage)
	{
		//it doesn't care where we bind the buffer to (http://www.opengl.org/wiki/Buffer_Object)
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferData(GL_COPY_WRITE_BUFFER, size, data, usage);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data)
	{
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}
	/*********************************************************************************/
	void* GLAPIENTRY _kglMapNamedBufferEXT(GLuint buffer, GLenum access)
	{
		// we can undbind buffer after mapping it. (http://www.opengl.org/wiki/Buffer_Object#Mapping)
		void* bufferData = 0;
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		bufferData = glMapBuffer(GL_COPY_WRITE_BUFFER, access);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		return bufferData;
	}
	/*********************************************************************************/
	GLboolean GLAPIENTRY _kglUnmapNamedBufferEXT(GLuint buffer)
	{
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		GLboolean unmapSuccessfully = glUnmapBuffer(GL_COPY_WRITE_BUFFER);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		return unmapSuccessfully;
	}
	/*********************************************************************************/
}}} //end of K15_Engine::Rendering::OpenGL namespace