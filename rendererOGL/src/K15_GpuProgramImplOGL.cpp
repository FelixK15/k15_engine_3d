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

#include "K15_GpuProgramImplOGL.h"

#include "K15_GpuProgramParameter.h"
#include "K15_LogManager.h"
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
	void GpuProgramImplOGL::reflect()
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
				currentUniform.setType(_getParameterType(attribType));
			}
			
// 			for(int i = 0;i < countActiveUniforms;++i)
// 			{
// 				GLchar* uniformName = (GLchar*)alloca(uniformNameLength[i]);
// 				GLuint uniformLocation = 0;
// 
// 				glGetActiveUniformName(m_Program,activeUniforms[i],uniformNameLength[i],0,uniformName);
// 				uniformLocation = glGetUniformLocation(m_Program,uniformName);
//         
// 				GpuProgramParameter& parameter = m_GpuProgram->getUniform(i);
// 				uniformString = String(uniformName);
// 
// 				GpuProgramParameter::DefaultVariableNameMap::const_iterator defaultIter = GpuProgramParameter::DefaultVariableNames.end();
// 
// 				//check if this parameter is an auto parameter
// 				for(GpuProgramParameter::DefaultVariableNameMap::const_iterator iter = GpuProgramParameter::DefaultVariableNames.begin();iter != 
// 				  GpuProgramParameter::DefaultVariableNames.end();++iter)
// 				{
// 				  if(iter->second == uniformString)
// 				  {
// 					defaultIter = iter;
// 					break;
// 				  }
// 				}
// 
// 				parameter.setAutoParameter(defaultIter != GpuProgramParameter::DefaultVariableNames.end());
// 				if(parameter.isAutoParameter())
// 				{
// 				  parameter.setAutoName(defaultIter->first);
// 				} 
// 				parameter.setName(uniformName);
// 				parameter.setSize(uniformSizes[i]);
// 				parameter.setType(_getParameterType(uniformTypes[i]));
// 				parameter.setBufferIndex(uniformBlocks[i]);
// 				parameter.setOffset(uniformOffset[i]);
// 				parameter.setRegisterIndex(uniformLocation);
// 			}
		}
	}
	/*********************************************************************************/
	bool GpuProgramImplOGL::compileShaderCode()
	{
		GLenum shaderStage = GLShaderStageConverter[m_GpuProgram->getStage()];
		bool compiledSuccessful = false;
		if(m_Program)
		{
			glDeleteProgram(m_Program);
			m_Program = 0;
		}
		
		const char* shaderCode = m_GpuProgram->getShaderCode().c_str();
		
		const GLuint shader = glCreateShader(shaderStage);
		if (shader) 
		{
			glShaderSource(shader, 1, &shaderCode, NULL);
			glCompileShader(shader);
			m_Program = glCreateProgram();
			if (m_Program) 
			{
				GLint compiled = GL_FALSE;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
				glProgramParameteri(m_Program, GL_PROGRAM_SEPARABLE, GL_TRUE);
				glProgramParameteri(m_Program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
			
				if (compiled) 
				{
					compiledSuccessful = true;
					glAttachShader(m_Program, shader);
					glLinkProgram(m_Program);
					//glDetachShader(m_Program, shader);

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
		//glDeleteShader(shader);

		return compiledSuccessful;
	}
/*********************************************************************************/
	void GpuProgramImplOGL::loadBinaryCode()
	{
		const RawData* binaryShader = m_GpuProgram->getBinaryCode();
		GLenum binaryFormat = GL_NONE;

		//retrieve format
		memcpy(&binaryFormat,binaryShader->data,sizeof(GLenum));

		glProgramBinary(m_Program,binaryFormat,binaryShader->data + sizeof(GLenum),binaryShader->size - sizeof(GLenum));
	}
/*********************************************************************************/
	void GpuProgramImplOGL::getBinaryCode(RawData* p_Buffer)
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
	GLuint GpuProgramImplOGL::getProgramGL() const
	{
		return m_Program;
	}
  /*********************************************************************************/
  Enum GpuProgramImplOGL::_getParameterType(GLenum p_GLType)
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
}}}// end of K15_Engine::Rendering::OGL namespace