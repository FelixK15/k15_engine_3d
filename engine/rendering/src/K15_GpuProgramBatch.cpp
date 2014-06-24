/**
 * @file K15_GpuProgramBatch.cpp
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

#include "K15_PrecompiledHeader.h"
#include "K15_RendererBase.h"

#include "K15_GpuProgramBatch.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	GpuProgramBatchImplBase::GpuProgramBatchImplBase()
		: m_ProgramBatch(0)
	{

	}
	/*********************************************************************************/
	GpuProgramBatchImplBase::~GpuProgramBatchImplBase()
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	GpuProgramBatch::GpuProgramBatch()
		: m_Error(),
		m_Compiled(false),
		m_Programs(),
		m_Impl(0)
	{
		m_Impl = g_Application->getRenderer()->createGpuProgramBatchImpl();
		m_Impl->setProgramBatch(this);
	}
	/*********************************************************************************/
	GpuProgramBatch::GpuProgramBatch(const GpuProgramList& p_Programs)
		: m_Error(),
		m_Compiled(false),
		m_Programs(p_Programs),
		m_Impl(0)
	{
		m_Impl = g_Application->getRenderer()->createGpuProgramBatchImpl();
		m_Impl->setProgramBatch(this);
	}
	/*********************************************************************************/
	GpuProgramBatch::~GpuProgramBatch()
	{

	}
	/*********************************************************************************/
	bool GpuProgramBatch::compile()
	{
		return m_Impl->compile();
	}
	/*********************************************************************************/
	bool GpuProgramBatch::hasGpuProgramStage(Enum p_ProgramStage)
	{
		GpuProgram* program = 0;
		for(GpuProgramList::iterator iter = m_Programs.begin();iter != m_Programs.end();++iter)
		{
			program = (*iter);
			if(program->getStage() == p_ProgramStage)
			{
				return true;
			}
		}

		return false;
	}
	/*********************************************************************************/
	GpuProgram* GpuProgramBatch::getGpuProgramByStage(Enum p_ProgramStage)
	{
		GpuProgram* program = 0;
		for(GpuProgramList::iterator iter = m_Programs.begin();iter != m_Programs.end();++iter)
		{
			program = (*iter);
			if(program->getStage() == p_ProgramStage)
			{
				return program;
			}
		}

		return 0;
	}
	/*********************************************************************************/
	void GpuProgramBatch::addGpuProgram(GpuProgram* p_Program, bool p_OverwriteProgramWithSameState)
	{
		K15_ASSERT(p_Program,"GpuProgram can't get added to GpuProgramBatch because GpuProgram is NULL.");

		if(hasGpuProgramStage(p_Program->getStage()) && !p_OverwriteProgramWithSameState)
		{
			K15_LOG_WARNING("Did not add GpuProgram \"%s\" to GpuProgramBatch because the GpuProgramBatch already has a GpuProgram for the stage.");
			return;
		}

		removeGpuProgram(p_Program->getStage());
		
		m_Programs.push_back(p_Program);
	}
	/*********************************************************************************/
	void GpuProgramBatch::removeGpuProgram(Enum p_ProgramStage)
	{
		GpuProgram* program = 0;
		for(GpuProgramList::iterator iter = m_Programs.begin();iter != m_Programs.end();++iter)
		{
			program = (*iter);
			if(program->getStage() == p_ProgramStage)
			{
				m_Programs.erase(iter);
				break;
			}
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace