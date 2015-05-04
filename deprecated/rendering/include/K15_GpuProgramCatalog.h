/**
 * @file K15_GpuProgramCatalog.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Rendering_GpuProgramCatalog_h_
#define _K15Engine_Rendering_GpuProgramCatalog_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API GpuProgramCatalog
	{
	public:
		typedef HashMap(ObjectName,GpuProgram*) GpuProgramNameMap;
		typedef HashMap(ObjectName,GpuProgramBatch*) GpuProgramBatchNameMap;
		static GpuProgramNameMap GpuProgramMap;
		static GpuProgramBatchNameMap GpuProgramBatchMap;

	public:
		static GpuProgram* getGpuProgram(const ObjectName& p_GpuProgramName, Enum p_ProgramStage);
		static GpuProgramBatch* getGpuProgramBatch(const ObjectName& p_GpuProgramBatchName);
	};// end of * class declaration
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Rendering_GpuProgramCatalog_h_