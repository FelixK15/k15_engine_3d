/**
 * @file K15_GpuProgram.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/28
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

#include "K15_PrecompiledHeader.h"

#include "K15_RenderTask.h"
#include "K15_RendererBase.h"
#include "K15_GpuProgram.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const uint32 GpuProgram::MaxParameter = 32;
	/*********************************************************************************/

	/*********************************************************************************/
	GpuProgram::GpuProgram(Enum p_ProgramStage)
		: ResourceBase(),
		  m_BinaryCode(),
		  m_Compiled(false),
		  m_Error(),
		  m_ShaderCode(),
		  m_Impl(0),
		  m_Attributes(),
		  m_Uniforms(),
		  m_UsedUniforms(0),
		  m_UsedAttributes(0),
		  m_Stage(p_ProgramStage)
	{
		m_Impl = g_Application->getRenderTask()->getRenderer()->createGpuProgramImpl();
		m_Impl->setGpuProgram(this);
		
		m_Attributes.reserve(MaxParameter);
		m_Uniforms.reserve(MaxParameter);
	}
	/*********************************************************************************/
	GpuProgram::~GpuProgram()
	{
		K15_DELETE m_Impl;
	}
	/*********************************************************************************/
	void GpuProgram::setProgramCode(const String& p_Code, bool p_Compile)
	{
		m_ShaderCode = p_Code;

		if(p_Compile)
		{
			compile();
		}
	}
	/*********************************************************************************/
	bool GpuProgram::compile()
	{
		if(m_BinaryCode.size > 0 || !m_ShaderCode.empty())
		{
			if(m_BinaryCode.size > 0)
			{
				m_Impl->loadBinaryCode();

				if(g_Application->getRenderer()->errorOccured())
				{
					m_Error = g_Application->getRenderer()->getLastError();
					_LogError("Could not load binary code for shader %s. %s",m_Name.c_str(),m_Error.c_str());
					m_Compiled = false;
				}

				else
				{
					m_Compiled = true;
				}
			}
			else
			{
				if(!m_Impl->compileShaderCode())
				{
					m_Error = g_Application->getRenderer()->getLastError();
					_LogError("Could not compile shader code for shader %s. %s",m_Name.c_str(),m_Error.c_str());
					m_Compiled = false;
				}
				else
				{
					m_Compiled = true;
//					m_Impl->getBinaryCode(&m_BinaryCode);
				}
			}
		}

		m_Impl->reflect();

		if(g_Application->getRenderer()->errorOccured())
		{
			_LogError("Could not reflect shader %s. %s.",m_Name.c_str(),g_Application->getRenderer()->getLastError().c_str());
			return false;
		}

		return m_Compiled;
	}
	/*********************************************************************************/
	void GpuProgram::loadDebug(RawData& p_Data)
	{

	}
	/*********************************************************************************/
	bool GpuProgram::internalLoad(const RawData& p_Data)
	{
		if(p_Data.data)
		{
			m_ShaderCode = _resolveIncludes((const char*)p_Data.data);

			m_Compiled = false;
// 			K15_DELETE m_BinaryCode.data;
// 			m_BinaryCode.data = 0;
// 			m_BinaryCode.size = 0;
      
			return compile();
		}
		
		return false;
	}
	/*********************************************************************************/
	void GpuProgram::setAmountAttributes(uint32 p_Amount)
	{
		K15_ASSERT(p_Amount <= MaxParameter,StringUtil::format("Trying to set more than %u attributes to GpuProgram %s.",MaxParameter,m_Name.c_str()));

		m_UsedAttributes = p_Amount;
	}
	/*********************************************************************************/
	void GpuProgram::setAmountUniforms(uint32 p_Amount)
	{
		K15_ASSERT(p_Amount <= MaxParameter,StringUtil::format("Trying to set more than %u parameters to GpuProgram %s.",MaxParameter,m_Name.c_str()));

		m_UsedUniforms = p_Amount;
	}
	/*********************************************************************************/
	GpuProgramParameter& GpuProgram::getUniform(uint32 p_Index)
	{
		K15_ASSERT(p_Index <= m_UsedUniforms,StringUtil::format("Trying to access out of bounds. GpuProgram %s",m_Name.c_str()));
		m_Uniforms.push_back(GpuProgramParameter());
		return m_Uniforms[p_Index];
	}
	/*********************************************************************************/
	GpuProgramParameter& GpuProgram::getAttribute(uint32 p_Index)
	{
		K15_ASSERT(p_Index <= m_UsedAttributes,StringUtil::format("Trying to acces out of bounds. GpuProgram %s",m_Name.c_str()));
		m_Attributes.push_back(GpuProgramParameter());
		return m_Attributes[p_Index];
	}
	/*********************************************************************************/
	const String& GpuProgram::_resolveIncludes(const char* p_ShaderCode)
	{
		static String shaderCode;
		static String includeLine;
		static String includeFile;
		static String foreignShaderCode;
		FileStream stream;

		shaderCode = p_ShaderCode;

		String::size_type pos = 0;
		String::size_type eol = 0;

		while((pos = shaderCode.find("#include"),pos) != String::npos)
		{
			eol = shaderCode.find_first_of('\n',pos);
			includeLine = StringUtil::removeWhitespaces(shaderCode.substr(pos,eol - pos));
			includeFile = includeLine.substr(includeLine.find_first_of('"'),includeLine.find_last_of('"'));

			if(includeFile.empty())
			{
				_LogError("Invalid line in GpuProgram. Line: %s, Program: %s",includeLine.c_str(),m_Name.c_str());
				continue;
			}

			stream.open(includeFile);

			if(!stream.is_open())
			{
				_LogError("Could not open GpuProgram include \"%s\".",includeFile);
				continue;
			}
      
			stream.seekg(0, FileStream::end); //move cursor to end
			foreignShaderCode.resize((size_t)stream.tellg()); //stream.tellg = where are we?
			stream.seekg(0, FileStream::beg); //move cursor to begin
			stream.read(&foreignShaderCode[0], foreignShaderCode.size());
			stream.close();

			shaderCode.replace(pos,shaderCode.size(),foreignShaderCode);
      
			pos = 0;
		}

		return shaderCode;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace