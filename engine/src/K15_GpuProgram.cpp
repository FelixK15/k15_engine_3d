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
	GpuProgram::GpuProgram()
		: ResourceBase(),
		  m_BinaryCode(),
		  m_Compiled(false),
		  m_Error(),
		  m_ShaderCode(),
		  m_Impl(0)
	{
		m_Impl = g_Application->getRenderTask()->getRenderer()->createGpuProgramImpl();
		m_Impl->setGpuProgram(this);
	}
	/*********************************************************************************/
	GpuProgram::~GpuProgram()
	{
		K15_DELETE m_Impl;
	}
	/*********************************************************************************/
	bool GpuProgram::compile()
	{
		if(m_BinaryCode.size > 0 || !m_ShaderCode.empty())
		{
			if(m_BinaryCode.size > 0)
			{
				if(!m_Impl->loadBinaryCode())
				{
					_LogError("Could not load binary code for shader %s. %s",m_AssetName.c_str(),m_Error.c_str());
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
					_LogError("Could not compile shader code for shader %s. %s",m_AssetName.c_str(),m_Error.c_str());
					m_Compiled = false;
				}
				else
				{
					m_Compiled = true;
					m_Impl->getBinaryCode(&m_BinaryCode);
				}
			}
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
			static char* shaderCode;
			shaderCode = (char*)alloca(p_Data.size);

			memcpy(shaderCode,p_Data.data,p_Data.size);

			m_ShaderCode = shaderCode;
			m_Compiled = false;
			delete m_BinaryCode.data;
			m_BinaryCode.data = 0;
			m_BinaryCode.size = 0;

			return compile();
		}
		
		return false;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace