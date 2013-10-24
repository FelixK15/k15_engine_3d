/**
 * @file K15_ShaderImplOGL.h
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

#ifndef K15_RENDERER_USE_PRECOMPILED_HEADERS
#	include "K15_RendererPrerequisites.h"
#endif //K15_RENDERER_USE_PRECOMPILED_HEADERS

#include "K15_GpuProgram.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	class GpuProgramImplOGL : public GpuProgramImplBase
	{
	public:
		GpuProgramImplOGL(Enum p_Type);
		virtual ~GpuProgramImplOGL();

		virtual void init(Enum p_Type) OVERRIDE;
		virtual void shutdown() OVERRIDE;

		virtual void setProgramCode(const String& p_Code) OVERRIDE;
	};// end of GpuProgramImplOGL class declaration
}}}// end of K15_Engine::Rendering::OGL namespace