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

#include "K15_IOUtil.h"
#include "K15_RendererBase.h"
#include "K15_GpuProgram.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const uint32 GpuProgram::MaxParameter = 128;
	/*********************************************************************************/

	/*********************************************************************************/
	GpuProgram::GpuProgram(const String& p_ProgramName, Enum p_ProgramStage)
		: ResourceBase(p_ProgramName),
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
		K15_ASSERT(p_ProgramStage >= PS_VERTEX && p_ProgramStage < PS_COUNT,
			StringUtil::format("Invalid program stage %u",p_ProgramStage));

		m_Impl = getRenderer()->createGpuProgramImpl();
		m_Impl->setGpuProgram(this);
		
		m_Attributes.reserve(MaxParameter);
		m_Uniforms.reserve(MaxParameter);
	}
	/*********************************************************************************/
	GpuProgram::~GpuProgram()
	{
		if(isBound())
		{
			getRenderer()->bindGpuProgram(0, m_Stage);
		}

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

				if(getRenderer()->errorOccured())
				{
					m_Error = getRenderer()->getLastError();
					K15_LOG_ERROR("Could not load binary code for shader \"%s\". %s",m_Name.c_str(),m_Error.c_str());
					m_Compiled = false;
				}

				else
				{
					m_Compiled = true;
				}
			}
			else
			{
				_resolveIncludes();
				if(!m_Impl->compileShaderCode())
				{
					m_Error = getRenderer()->getLastError();
					K15_LOG_ERROR("Could not compile shader code for shader \"%s\". %s",m_Name.c_str(),m_Error.c_str());
					m_Compiled = false;
				}
				else
				{
					m_Compiled = true;
//					m_Impl->getBinaryCode(&m_BinaryCode);
				}
			}
		}

		return reflect();
	}
	/*********************************************************************************/
	void GpuProgram::setAmountAttributes(uint32 p_Amount)
	{
		K15_ASSERT(p_Amount <= MaxParameter,
			StringUtil::format("Trying to set more than %u attributes to GpuProgram \"%s\".",
			MaxParameter,m_Name.c_str()));

		m_UsedAttributes = p_Amount;
	}
	/*********************************************************************************/
	void GpuProgram::setAmountUniforms(uint32 p_Amount)
	{
		K15_ASSERT(p_Amount <= MaxParameter,
			StringUtil::format("Trying to set more than %u parameters to GpuProgram \"%s\".",
			MaxParameter,m_Name.c_str()));

		m_UsedUniforms = p_Amount;
	}
	/*********************************************************************************/
	GpuProgramParameter& GpuProgram::getUniform(uint32 p_Index)
	{
		K15_ASSERT(p_Index <= m_UsedUniforms,
			StringUtil::format("Trying to access out of bounds. GpuProgram \"%s\"",
			m_Name.c_str()));

		if(m_Uniforms.size() == p_Index) m_Uniforms.push_back(GpuProgramParameter());
		return m_Uniforms[p_Index];
	}
	/*********************************************************************************/
	GpuProgramParameter& GpuProgram::getAttribute(uint32 p_Index)
	{
		K15_ASSERT(p_Index <= m_UsedAttributes,
			StringUtil::format("Trying to acces out of bounds. GpuProgram \"%s\"",
			m_Name.c_str()));

		if(m_Attributes.size() == p_Index) m_Attributes.push_back(GpuProgramParameter());
		return m_Attributes[p_Index];
	}
	/*********************************************************************************/
	void GpuProgram::_resolveIncludes()
	{
		static String shaderCode;
		static String includeLine;
		static String includeFile;
		static String foreignShaderCode;
		FileStream stream;

		shaderCode = m_ShaderCode;

		String::size_type pos = 0;
		String::size_type eol = 0;
		String::size_type startInclude = 0;
		String::size_type endInclude = 0;

		uint32 length = 0;

		while((pos = shaderCode.find("#include"),pos) != String::npos)
		{
			eol = shaderCode.find_first_of('\n',pos);
			includeLine = StringUtil::removeWhitespaces(shaderCode.substr(pos,eol - pos));
			startInclude = includeLine.find_first_of('\"') + 1;
			endInclude = includeLine.find_last_of('\"');
			length = endInclude - startInclude;
			
			includeFile = includeLine.substr(startInclude, length);

			if(includeFile.empty())
			{
				K15_LOG_ERROR("Invalid line in GpuProgram. Line: %s, Program: \"%s\"",includeLine.c_str(),m_Name.c_str());
				break;
			}

			includeFile = g_Application->getGameRootDir() + includeFile;    
			foreignShaderCode = IOUtil::readWholeFile(includeFile);

			shaderCode.replace(pos, includeLine.size() + 1, foreignShaderCode);
      
			pos = 0;
		}

		m_ShaderCode = shaderCode;
	}
	/*********************************************************************************/
	bool GpuProgram::reflect()
	{
		if(!m_Compiled)
		{
			K15_LOG_ERROR("Can't reflect shader \"%s\". Has not been compiled.",getName().c_str());
			return false;
		}
		else
		{
			m_Impl->reflect();

			//Check and mark auto parameter
			for(uint32 i = 0;i < m_UsedUniforms;++i)
			{
				m_Uniforms[i].setGpuProgram(this);

				const String& uniformName = m_Uniforms[i].getName();

				//check if the uniform is an array
				uint32 indexStart = uniformName.find_first_of('[') + 1;
				uint32 indexEnd = uniformName.find_first_of(']');
				int32 arrayIndex = -1;
				if(indexStart != String::npos && indexEnd != String::npos)
				{
					String newUniformName = uniformName.substr(0, indexStart);
					newUniformName += uniformName.substr(indexStart + 1);

					arrayIndex = StringUtil::toInt(uniformName.substr(indexStart, 1));

					m_Uniforms[i].setArrayIndex(arrayIndex);
					m_Uniforms[i].setName(newUniformName);
				}
				
				for(int j = 0;j < GpuProgramParameter::PI_COUNT;++j)
				{
					if(m_Uniforms[i].getName() == GpuProgramParameter::UniformIdentifierName[j])
					{
						K15_LOG_DEBUG("Found auto parameter \"%s\" in GpuProgram \"%s\".",
							m_Uniforms[i].getName().c_str(),
							getName().c_str());

						m_Uniforms[i].setIdentifier(j);
						break;
					}
				}
			}

			for(uint32 i = 0;i < m_UsedAttributes;++i)
			{
				m_Attributes[i].setGpuProgram(this);
			}

			//did everything went smoothly?
			if(getRenderer()->errorOccured())
			{
				K15_LOG_ERROR("Could not reflect shader \"%s\". %s.",m_Name.c_str(),getRenderer()->getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace