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

#include "K15_LogManager.h"
#include "K15_RenderTask.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	const GLenum GpuProgramImplOGL::GLShaderStageConverter[GpuProgram::PS_COUNT] = {
		GL_VERTEX_SHADER,	//PS_VERTEX
		GL_GEOMETRY_SHADER, //PS_GEOMETRY
		GL_FRAGMENT_SHADER, //PS_FRAGMENT
		GL_COMPUTE_SHADER,  //PS_COMPUTE
	}; //GLShaderStageConverter
	/*********************************************************************************/
	GpuProgramImplOGL::GpuProgramImplOGL()
		: GpuProgramImplBase(),
		m_Program(0)
	{
	
	}
	/*********************************************************************************/
	GpuProgramImplOGL::~GpuProgramImplOGL()
	{
		if(m_Program > 0)
		{
			glDeleteProgram(m_Program);
		}
	}
	/*********************************************************************************/
	bool GpuProgramImplOGL::compileShaderCode()
	{
		GLenum shaderStage = GLShaderStageConverter[m_GpuProgram->getStage()];

		if(m_Program)
		{
			glDeleteProgram(m_Program);
			m_Program = 0;
		}

		m_Program = glCreateShaderProgramv(shaderStage,1,(const GLchar**)m_GpuProgram->getShaderCode().c_str());

		if(glGetError() != GL_NO_ERROR)
		{
			m_GpuProgram->setError(g_Application->getRenderTask()->getRenderer()->getLastError());
			return false;
		}

		return true;
	}
	/*********************************************************************************/
	bool GpuProgramImplOGL::loadBinaryCode()
	{
		const RawData* binaryShader = m_GpuProgram->getBinaryCode();
		GLenum binaryFormat = GL_NONE;

		//retrieve format
		memcpy(&binaryFormat,binaryShader->data,sizeof(GLenum));

		glProgramBinary(m_Program,binaryFormat,binaryShader->data + sizeof(GLenum),binaryShader->size - sizeof(GLenum));

		if(glGetError() != GL_NO_ERROR)
		{
			_LogError("Could not load compiled shader-binary from shader %s. %s",m_GpuProgram->getAssetName().c_str(),
				g_Application->getRenderTask()->getRenderer()->getLastError().c_str());

			return false;
		}

		return true;
	}
	/*********************************************************************************/
	bool GpuProgramImplOGL::getBinaryCode(RawData* p_Buffer)
	{
		GLint sizeShader = 0;
		GLenum binaryFormat = GL_NONE;
		glGetProgramiv(m_Program,GL_PROGRAM_BINARY_LENGTH,&sizeShader);
		sizeShader += sizeof(GLenum);

		p_Buffer->size = sizeShader;
		p_Buffer->data = new byte[sizeShader];

		glGetProgramBinary(m_Program,sizeShader - sizeof(GLenum),0,&binaryFormat,p_Buffer->data + sizeof(GLenum));

		//store format at the beginning of the buffer
		memcpy(p_Buffer->data,&binaryFormat,sizeof(GLenum));

		if(glGetError() != GL_NO_ERROR)
		{
			_LogError("Could not get compiled shader-binary from shader %s. %s",m_GpuProgram->getAssetName().c_str(),
				g_Application->getRenderTask()->getRenderer()->getLastError().c_str());

			return false;
		}

		return true;
	}
	/*********************************************************************************/
	GLuint GpuProgramImplOGL::getProgramGL() const
	{
		return m_Program;
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::OGL namespace