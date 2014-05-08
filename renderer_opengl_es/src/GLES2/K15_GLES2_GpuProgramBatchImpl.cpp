/**
 * @file K15_GLES2_GpuProgramBatchImpl.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/05
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

#include "K15_LogManager.h"

#include "K15_GLES2_GpuProgramBatchImpl.h"

#include "K15_GLES2_GpuProgramImpl.h"

namespace K15_Engine { namespace Rendering { namespace GLES2 {
	/*********************************************************************************/
	GpuProgramBatchImpl::GpuProgramBatchImpl()
		: GpuProgramBatchImplBase(),
		m_Program(0)
	{
		m_Program = glCreateProgram();
	}
	/*********************************************************************************/
	GpuProgramBatchImpl::~GpuProgramBatchImpl()
	{
		glDeleteProgram(m_Program);
	}
	/*********************************************************************************/
	bool GpuProgramBatchImpl::compile()
	{
		const GpuProgramBatch::GpuProgramList& programs = m_ProgramBatch->getGpuProgramList();
		GpuProgram* program = 0;
		GpuProgramImpl* programImpl = 0;
		GLint shader = 0;
		GLint compileSuccess = 0;
		const char* shadercode[] = {
			"#version 120\n"
			"precision mediump float;\n",
			0,
			0
		};

		for(GpuProgramBatch::GpuProgramList::const_iterator iter = programs.begin();iter != programs.end();++iter)
		{
			program = (*iter);
			programImpl = (GpuProgramImpl*)(program->getImpl());
			shader = glCreateShader(GpuProgramImpl::GLShaderStageConverter[program->getStage()]);

			//set shader code for compatibility reasons
			if(program->getStage() == GpuProgram::PS_VERTEX)
			{
				shadercode[1] = "#define in attribute\n"
								"#define out varying\n";
			}
			else if(program->getStage() == GpuProgram::PS_FRAGMENT)
			{
				shadercode[1] = "#define in varying\n";
			}

			shadercode[2] = program->getShaderCode().c_str();
			glShaderSource(shader,3,shadercode,0);
			glCompileShader(shader);

			glGetShaderiv(shader,GL_COMPILE_STATUS,&compileSuccess);

			if(compileSuccess == GL_FALSE)
			{
				GLint logLength = 0;
				glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logLength);

				char* infoLogBuffer = (char*)alloca(logLength);
				glGetShaderInfoLog(shader,logLength,&logLength,infoLogBuffer);

				_LogError("Could not compile shader \"%s\" in GpuProgramBatch. Error:\"%s\".",program->getName().c_str(),infoLogBuffer);
				m_ProgramBatch->setError(infoLogBuffer);

				glDeleteShader(shader);

				return false;
			}
			else
			{
				//Attach shader to program object
				glAttachShader(m_Program,shader);
				programImpl->setShaderGL(shader);
				programImpl->setProgramGL(m_Program);
				program->setCompiled(true);
			}
		}

		glLinkProgram(m_Program);

		GLint linkSuccess = 0;
		glGetProgramiv(m_Program,GL_LINK_STATUS,&linkSuccess);

		if(linkSuccess == GL_FALSE)
		{
			GLint logLength = 0;
			glGetProgramiv(m_Program,GL_INFO_LOG_LENGTH,&logLength);

			char* infoLogBuffer = (char*)alloca(logLength);
			glGetProgramInfoLog(m_Program,logLength,&logLength,infoLogBuffer);

			_LogError("Could not link GpuShaderBatch. Error:\"%s\".",infoLogBuffer);
			m_ProgramBatch->setError(infoLogBuffer);

			//recreate program
			glDeleteProgram(m_Program);
			m_Program = glCreateProgram();

			return false;
		}

		if(m_ProgramBatch->getGpuProgramList().size() > 0)
		{
			GpuProgram* lastProgram = m_ProgramBatch->getGpuProgramList().back();
			lastProgram->reflect();
		}
		
		return true;
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::WGL namespace