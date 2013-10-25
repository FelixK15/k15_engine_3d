/**
 * @file K15_GpuBufferImplOGL.h
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

#ifndef _K15Engine_RendererOGL_GpuBufferImpl_h_
#define _K15Engine_RendererOGL_GpuBufferImpl_h_

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_RendererOGLPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_GpuBuffer.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	class GpuBufferImplOGL : public GpuBufferImplBase
	{
	public:
		GpuBufferImplOGL();
		virtual ~GpuBufferImplOGL();

		virtual void init(Enum p_BufferType,Enum p_UsageOptions,Enum p_LockOptions);
		virtual void shutdown();

		virtual void setLockOption(Enum p_LockOption);
		virtual void setUsageOption(Enum p_UsageOption);

		virtual bool lock();
		virtual bool unlock();

		virtual uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset);
		virtual uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset);

	private:
		GLuint m_BufferHandle;
	};// end of GpuBufferImplOG class declaration
}}}//end of K15_Engine::Rendering::OGL namespace

#endif //_K15Engine_RendererOGL_GpuBufferImpl_h_