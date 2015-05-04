/**
 * @file K15_GpuProgramCatalog.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/10
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

#include "K15_GpuProgramCatalog.h"
#include "K15_GpuProgram.h"
#include "K15_GpuProgramBatch.h"

#include "K15_IOUtil.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	GpuProgramCatalog::GpuProgramNameMap GpuProgramCatalog::GpuProgramMap = GpuProgramCatalog::GpuProgramNameMap();
	GpuProgramCatalog::GpuProgramBatchNameMap GpuProgramCatalog::GpuProgramBatchMap	= GpuProgramCatalog::GpuProgramBatchNameMap();
	/*********************************************************************************/

	/*********************************************************************************/
	GpuProgram* GpuProgramCatalog::getGpuProgram(const ObjectName& p_GpuProgramName, Enum p_ProgramStage)
	{
		GpuProgram* program = 0;
		GpuProgramNameMap::iterator iter = GpuProgramMap.find(p_GpuProgramName);

		if(iter == GpuProgramMap.end())
		{
			program = K15_NEW GpuProgram(p_GpuProgramName.c_str(),p_ProgramStage);
			String programCode = IOUtil::readWholeFile(g_Application->getGameRootDir() + p_GpuProgramName.c_str());

			program->setProgramCode(programCode,true);

			GpuProgramMap.insert(Pair(ObjectName,GpuProgram*)(p_GpuProgramName,program));
		}
		else
		{
			program = iter->second;
		}

		return program;
	}
	/*********************************************************************************/
	GpuProgramBatch* GpuProgramCatalog::getGpuProgramBatch(const ObjectName& p_GpuProgramBatchName)
	{
		GpuProgramBatch* batch = 0;
		GpuProgramBatchNameMap::iterator iter = GpuProgramBatchMap.find(p_GpuProgramBatchName);

		if(iter == GpuProgramBatchMap.end())
		{
			batch = K15_NEW GpuProgramBatch();

			String fragmentProgramName = p_GpuProgramBatchName.c_str();
			String vertexProgramName = p_GpuProgramBatchName.c_str();
			vertexProgramName += ".vert";
			fragmentProgramName += ".frag";

			batch->addGpuProgram(getGpuProgram(vertexProgramName,GpuProgram::PS_VERTEX));
			batch->addGpuProgram(getGpuProgram(fragmentProgramName,GpuProgram::PS_FRAGMENT));

			batch->compile();

			GpuProgramBatchMap.insert(Pair(ObjectName,GpuProgramBatch*)(p_GpuProgramBatchName,batch));
		}
		else
		{
			batch = iter->second;
		}

		return batch;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace