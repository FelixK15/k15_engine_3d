/**
 * @file K15_GpuProgram.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_GpuProgram_h_
#define _K15Engine_Renderer_GpuProgram_h_

#ifndef K15_RENDERER_USE_PRECOMPILED_HEADERS
#	include "K15_RendererPrerequisites.h"
#endif //K15_RENDERER_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Rendering { 
	class GpuProgramImplBase
	{

	};// end of GpuProgramImplBase class declaration

	/*********************************************************************************/

	class GpuProgram
	{
	public:
		GpuProgram();
		virtual ~GpuProgram();

	protected:
		GpuProgramImplBase* p_Impl;
	};/// end of GpuProgram class declaration
}}//end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_GpuProgram_h_