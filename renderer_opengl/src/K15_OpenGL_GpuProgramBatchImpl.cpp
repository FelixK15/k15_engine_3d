/**
 * @file K15_WGL_GpuProgramBatchImpl.cpp
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

#include "K15_OpenGL_PrecompiledHeader.h"

#include "K15_OpenGL_GpuProgramBatchImpl.h"
#include "K15_OpenGL_GpuProgramImpl.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
	GpuProgramBatchImpl::GpuProgramBatchImpl()
		: GpuProgramBatchImplBase()
	{
		
	}
	/*********************************************************************************/
	GpuProgramBatchImpl::~GpuProgramBatchImpl()
	{
		
	}
	/*********************************************************************************/
	bool GpuProgramBatchImpl::compile()
	{
		GpuProgram* program = 0;
		const GpuProgramBatch::GpuProgramList& programs = m_ProgramBatch->getGpuProgramList();

		for(GpuProgramBatch::GpuProgramList::const_iterator iter = programs.begin();iter != programs.end();++iter)
		{
			program = (*iter);

			if(!program->compile())
			{
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::WGL namespace