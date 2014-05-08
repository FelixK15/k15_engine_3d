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

#include "K15_WGL_GpuProgramImpl.h"

#include "K15_GpuProgramParameter.h"
#include "K15_LogManager.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { namespace WGL {
	/*********************************************************************************/
	const GLenum GpuProgramImpl::GLShaderStageConverter[GpuProgram::PS_COUNT] = {
		GL_VERTEX_SHADER,	//PS_VERTEX
		GL_GEOMETRY_SHADER, //PS_GEOMETRY
		GL_FRAGMENT_SHADER, //PS_FRAGMENT
		GL_COMPUTE_SHADER,  //PS_COMPUTE
	}; //GLShaderStageConverter
	/*********************************************************************************/
	GpuProgramImpl::GpuProgramImpl()
		: GpuProgramImplBase(),
		m_Program(0),
		m_Shader(0)

	{
	
	}
	/*********************************************************************************/
	GpuProgramImpl::~GpuProgramImpl()
	{
		if(m_Shader > 0)
		{
			glDetachShader(m_Program,m_Shader);
			glDeleteShader(m_Shader);
		}

		if(m_Program > 0)
		{
			glDeleteProgram(m_Program);
		}
	}
	/*********************************************************************************/
	void GpuProgramImpl::reflect()
	{
		if(m_GpuProgram->isCompiled())
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
			}
		}
	}
	/*********************************************************************************/
	bool GpuProgramImpl::compileShaderCode()
	{
		GLenum shaderStage = GLShaderStageConverter[m_GpuProgram->getStage()];
		bool compiledSuccessful = false;
		if(m_Program)
		{
			glDeleteProgram(m_Program);
			m_Program = 0;
		}
		
		const char* shaderCode[3] = {
			"#version 330\n",

			0,

			m_GpuProgram->getShaderCode().c_str()
		};
		
		m_Shader = glCreateShader(shaderStage);

		if(m_Shader) 
		{
			if(shaderStage == GL_VERTEX_SHADER)
			{
				shaderCode[1] = "out gl_PerVertex {\n"
								"vec4 gl_Position;\n"
								"float gl_PointSize;\n"
								"float gl_ClipDistance[];\n"
								"};\n";
			}
			else if(shaderStage == GL_FRAGMENT_SHADER)
			{
				shaderCode[1] = "";
			}
			
			glShaderSource(m_Shader, 3, shaderCode, NULL);
			glCompileShader(m_Shader);
			m_Program = glCreateProgram();
			if (m_Program) 
			{
				GLint compiled = GL_FALSE;
				glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &compiled);
				glProgramParameteri(m_Program, GL_PROGRAM_SEPARABLE, GL_TRUE);
				glProgramParameteri(m_Program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
			
				if(compiled) 
				{
					compiledSuccessful = true;
					glAttachShader(m_Program, m_Shader);
					glLinkProgram(m_Program);
					
					GLint linkStatus = GL_FALSE;
					glGetProgramiv(m_Program,GL_LINK_STATUS,&linkStatus);

					if(linkStatus == GL_FALSE)
					{
						GLint infoLogLength = 0;
						glGetProgramiv(m_Program,GL_INFO_LOG_LENGTH,&infoLogLength);
						if(infoLogLength == 0)
						{
							compiledSuccessful = false;
							String errorMsg = "gpu program linking and info log retrieval failed.\n" + g_Application->getRenderer()->getLastError();
							g_Application->getRenderer()->setLastError(errorMsg);
						}
						else
						{
							char* infoLogBuffer = (char*)alloca(infoLogLength);
							compiledSuccessful = false;
							String errorMsg = infoLogBuffer;
							errorMsg += "\n" + g_Application->getRenderer()->getLastError();
							g_Application->getRenderer()->setLastError(errorMsg);
						}
					}
				}
			}
		}

		return compiledSuccessful;
	}
	/*********************************************************************************/
	void GpuProgramImpl::loadBinaryCode()
	{
		const RawData* binaryShader = m_GpuProgram->getBinaryCode();
		GLenum binaryFormat = GL_NONE;

		//retrieve format
		memcpy(&binaryFormat,binaryShader->data,sizeof(GLenum));

		glProgramBinary(m_Program,binaryFormat,binaryShader->data + sizeof(GLenum),binaryShader->size - sizeof(GLenum));
	}
	/*********************************************************************************/
	void GpuProgramImpl::getBinaryCode(RawData* p_Buffer)
	{
		GLint sizeShader = 0;
		GLenum binaryFormat = GL_NONE;
		glGetProgramiv(m_Program,GL_PROGRAM_BINARY_LENGTH,&sizeShader);
		
		K15_ASSERT(sizeShader,StringUtil::format("Size of binary for shader %s is 0.",m_GpuProgram->getName().c_str()));

		sizeShader += sizeof(GLenum);

		p_Buffer->size = sizeShader;
		p_Buffer->data = new byte[sizeShader];

		glGetProgramBinary(m_Program,sizeShader - sizeof(GLenum),0,&binaryFormat,p_Buffer->data + sizeof(GLenum));

		//store format at the beginning of the buffer
		memcpy(p_Buffer->data,&binaryFormat,sizeof(GLenum));
	}
	/*********************************************************************************/
	GLuint GpuProgramImpl::getProgramGL() const
	{
		return m_Program;
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
		else if(p_GLType == GL_SAMPLER_1D)
		{
			return GpuProgramParameter::VT_SAMPLER_1D;
		}
		else if(p_GLType == GL_SAMPLER_2D)
		{
			return GpuProgramParameter::VT_SAMPLER_2D;
		}
		else if(p_GLType == GL_SAMPLER_3D)
		{
			return GpuProgramParameter::VT_SAMPLER_3D;
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
		else if(p_ProgramStage == GpuProgram::PS_COMPUTE)
		{
			return ".comp";
		}
		else if(p_ProgramStage == GpuProgram::PS_GEOMETRY)
		{
			return ".geom";
		}

		return "";
	}	
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::OGL namespace