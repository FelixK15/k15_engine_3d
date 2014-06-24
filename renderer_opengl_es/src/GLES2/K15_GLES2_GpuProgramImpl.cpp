/**
 * @file K15_GpuProgramImpl.cpp
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

#include "K15_GLES2_GpuProgramImpl.h"

#include "K15_GpuProgramParameter.h"
#include "K15_LogManager.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { namespace GLES2 {
	/*********************************************************************************/
	const GLenum GpuProgramImpl::GLShaderStageConverter[GpuProgram::PS_COUNT] = {
		GL_VERTEX_SHADER,	//PS_VERTEX
		GL_NONE,			//PS_GEOMETRY		//not supported in GLES 2.0
		GL_FRAGMENT_SHADER, //PS_FRAGMENT
		GL_NONE,			//PS_COMPUTE		//not supported in GLES 2.0
	}; //GLShaderStageConverter
	/*********************************************************************************/
	GpuProgramImpl::GpuProgramImpl()
		: GpuProgramImplBase(),
		m_Shader(GL_NONE),
		m_Program(GL_NONE)

	{
	
	}
	/*********************************************************************************/
	GpuProgramImpl::~GpuProgramImpl()
	{
		if(m_Shader)
		{
			glDetachShader(m_Program,m_Shader);
			glDeleteShader(m_Shader);
		}
	}
	/*********************************************************************************/
	void GpuProgramImpl::reflect()
	{
		if(m_Shader != GL_NONE)
		{
			GLint countActiveAttribs = 0;
			GLint attribBufferSize = 0;
			GLint actualAttribBufferSize = 0;
			GLint attribSize = 0;
			GLint attribLocation = 0;
			GLenum attribType = GL_NONE;

			GLint countActiveUniforms = 0;
			GLint uniformBufferSize = 0;
			GLint actualUniformBufferSize = 0;
			GLint uniformSize = 0;
			GLint uniformLocation = 0;
			GLint uniformBlock = 0;
			GLenum uniformType = 0;

			char* buffer = 0;

			glGetProgramiv(m_Program,GL_ACTIVE_UNIFORMS,&countActiveUniforms);
			glGetProgramiv(m_Program,GL_ACTIVE_ATTRIBUTES,&countActiveAttribs);
			glGetProgramiv(m_Program,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&attribBufferSize);
			glGetProgramiv(m_Program,GL_ACTIVE_UNIFORM_MAX_LENGTH,&uniformBufferSize);

			m_GpuProgram->setAmountUniforms(countActiveUniforms);
			m_GpuProgram->setAmountAttributes(countActiveAttribs);

			K15_LOG_DEBUG("Found %i active attributes for shader \"%s\".",countActiveAttribs,m_GpuProgram->getName().c_str());
			K15_LOG_DEBUG("Found %i active uniforms for shader \"%s\".",countActiveUniforms,m_GpuProgram->getName().c_str());

			buffer = (char*)alloca(attribBufferSize);
			for(int i = 0;i < countActiveAttribs;++i)
			{
				glGetActiveAttrib(m_Program,i,attribBufferSize,&actualAttribBufferSize,&attribSize,&attribType,buffer);
				attribLocation = glGetAttribLocation(m_Program,buffer);

				GpuProgramParameter& currentAttrib = m_GpuProgram->getAttribute(i);

				currentAttrib.setRegisterIndex(attribLocation);
				currentAttrib.setName(buffer);
				currentAttrib.setSize(attribSize);
				currentAttrib.setType(_getParameterType(attribType));

				K15_LOG_DEBUG("Attribute info: name: \"%s\" - size: %ibyte - index: %i - type: %i",
					buffer,attribSize,attribLocation,attribType);
			}

			buffer = (char*)alloca(uniformBufferSize);
			for(int i = 0;i < countActiveUniforms;++i)
			{
				glGetActiveUniform(m_Program,i,uniformBufferSize,&actualAttribBufferSize,&uniformSize,&uniformType,buffer);
				uniformLocation = glGetUniformLocation(m_Program,buffer);
				
				GpuProgramParameter& currentUniform = m_GpuProgram->getUniform(i);

				currentUniform.setRegisterIndex(uniformLocation);
				currentUniform.setName(buffer);
				currentUniform.setSize(uniformSize);
				currentUniform.setType(_getParameterType(uniformType));

				K15_LOG_DEBUG("Uniform info: name: \"%s\" - size: %ibyte - index: %i - type: %i",
					buffer,uniformSize,uniformLocation,uniformType);
			}
		}
	}
	/*********************************************************************************/
	bool GpuProgramImpl::compileShaderCode()
	{
		GLint compiled = GL_FALSE;
		GLenum shaderStage = GLShaderStageConverter[m_GpuProgram->getStage()];
		GLint logLength = 0;
		bool compiledSuccessful = false;

 		const char* shaderCode = m_GpuProgram->getShaderCode().c_str();
 		
		if(m_Shader)
		{
			glDeleteShader(m_Shader);
		}

 		m_Shader = glCreateShader(shaderStage);
		glShaderSource(m_Shader, 1, &shaderCode, NULL);
		glCompileShader(m_Shader);
		
		glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled) 
		{

			glGetShaderiv(m_Shader,GL_INFO_LOG_LENGTH,&logLength);

			char* infoLogBuffer = (char*)alloca(logLength);
			glGetShaderInfoLog(m_Shader,logLength,&logLength,infoLogBuffer);

			K15_LOG_ERROR("Could not compile shader \"%s\". Error:\"%s\".",m_GpuProgram->getName().c_str(),infoLogBuffer);

			return false;
		}

		return true;
	}
	/*********************************************************************************/
	void GpuProgramImpl::loadBinaryCode()
	{

	}
	/*********************************************************************************/
	void GpuProgramImpl::getBinaryCode(RawData* p_Buffer)
	{

	}
	/*********************************************************************************/
	GLuint GpuProgramImpl::getShaderGL() const
	{
		return m_Shader;
	}
	/*********************************************************************************/
	Enum GpuProgramImpl::_getParameterType(GLenum p_GLType)
	{
		if(p_GLType == GL_FLOAT)
		{
			return GpuProgramParameter::VT_FLOAT;
		}
		else if(p_GLType == GL_FLOAT_VEC2)
		{
			return GpuProgramParameter::VT_VECTOR2;
		}
		else if(p_GLType == GL_FLOAT_VEC3)
		{
			return GpuProgramParameter::VT_VECTOR3;
		}
		else if(p_GLType == GL_FLOAT_VEC4)
		{
			return GpuProgramParameter::VT_VECTOR4;
		}
		else if(p_GLType == GL_BOOL)
		{
			return GpuProgramParameter::VT_BOOL;
		}
		else if(p_GLType == GL_FLOAT_MAT2)
		{
			return GpuProgramParameter::VT_MATRIX2;
		}
		else if(p_GLType == GL_FLOAT_MAT3)
		{
			return GpuProgramParameter::VT_MATRIX3;
		}
		else if(p_GLType == GL_FLOAT_MAT4)
		{
			return GpuProgramParameter::VT_MATRIX4;
		}
		else if(p_GLType == GL_SAMPLER_2D)
		{
			return GpuProgramParameter::VT_SAMPLER_2D;
		}

		return GpuProgramParameter::VT_UNKNOW;
	}
	/*********************************************************************************/
	String GpuProgramImpl::getShaderExtension(Enum p_ProgramStage)
	{
		if(p_ProgramStage == GpuProgram::PS_VERTEX)
		{
			return ".vert";
		}
		else if(p_ProgramStage == GpuProgram::PS_FRAGMENT)
		{
			return ".frag";
		}

		return "";
	}	
	/*********************************************************************************/
	GLuint GpuProgramImpl::getProgramGL() const
	{
		return m_Program;
	}
	/*********************************************************************************/
	void GpuProgramImpl::setShaderGL(GLuint p_Shader)
	{
		m_Shader = p_Shader;
	}
	/*********************************************************************************/
	void GpuProgramImpl::setProgramGL(GLuint p_Program)
	{
		m_Program = p_Program;
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::OGL namespace