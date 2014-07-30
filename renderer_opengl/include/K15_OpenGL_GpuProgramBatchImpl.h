/**
 * @file K15_WGL_GpuProgramBatchImpl.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_OpenGL_GpuProgramBatchImpl_h_
#define _K15Engine_Renderer_OpenGL_GpuProgramBatchImpl_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_OpenGL_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_GpuProgramBatch.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	class K15_RENDERER_API GpuProgramBatchImpl : public GpuProgramBatchImplBase
	{
	public:
		GpuProgramBatchImpl();
		~GpuProgramBatchImpl();

		bool compile() OVERRIDE;
	}; //GpuProgramBatchImpl class declaration
}}} // end of K15_Engine::Rendering::WGL namespace

#endif //_K15Engine_Renderer_OpenGL_GpuProgramBatchImpl_h_