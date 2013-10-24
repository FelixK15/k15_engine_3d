/**
 * @file K15_ShaderImplOGL.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_GpuProgramImplOGL.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	GLenum GpuProgramImplOGL::GLGpuProgramTypeConverter[GpuProgram::GPT_COUNT] = {
		GL_VERTEX_SHADER,	//GPT_VERTEX_PROGRAM
		GL_FRAGMENT_SHADER,	//GPT_PIXEL_PROGRAM
		GL_GEOMETRY_SHADER,	//GPT_GEOMETRY_PROGRAM
		GL_COMPUTE_SHADER	//GPT_COMPUTE_PROGRAM
	};//GLGpuProgramTypeConverter
	/*********************************************************************************/
	GpuProgramImplOGL::GpuProgramImplOGL()
		: GpuProgramImplBase(),
		  m_GpuProgramHandle(K15_OGL_INVALID_HANDLE)
	{
	
	}
	/*********************************************************************************/
	GpuProgramImplOGL::~GpuProgramImplOGL()
	{

	}
	/*********************************************************************************/
	void GpuProgramImplOGL::init(Enum p_Type)
	{
		m_GpuProgramHandle = glCreateShader(GLGpuProgramTypeConverter[p_Type]);
	}
	/*********************************************************************************/
	void GpuProgramImplOGL::shutdown()
	{
		glDeleteShader(m_GpuProgramHandle);
		m_GpuProgramHandle = K15_OGL_INVALID_HANDLE;
	}
	/*********************************************************************************/
	bool GpuProgramImplOGL::compile()
	{
		glShaderSource(m_GpuProgramHandle,1,(GLchar**)m_GpuProgram->getProgramCode().c_str(),(GLint*)m_GpuProgram->getProgramCode().length());
		glCompileShader(m_GpuProgramHandle);
		
		GLuint status = 0;
		glGetShaderiv(m_GpuProgramHandle,GL_COMPILE_STATUS,&status);

		if(status != GL_TRUE)
		{
			_LogError("Could not compile shader.");
			return false;
		}

		return true;
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::OGL namespace