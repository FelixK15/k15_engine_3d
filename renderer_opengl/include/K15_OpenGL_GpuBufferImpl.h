/**
 * @file K15_GpuBufferImpl.h
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

#ifndef _K15Engine_Renderer_OpenGL_GpuBufferImpl_h_
#define _K15Engine_Renderer_OpenGL_GpuBufferImpl_h_

#ifndef K15_RENDERER_USE_PREDEFINED_HEADERS
#	include "K15_OpenGL_Prerequisites.h"
#endif //K15_RENDERER_USE_PREDEFINED_HEADERS

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	class GpuBufferImpl : public GpuBufferImplBase
	{
	public:
		/*********************************************************************************/
		static const GLenum GLBufferTypeConverter[GpuBuffer::BT_COUNT];
		static const GLenum GLBufferUsageConverter[GpuBuffer::UO_COUNT];
		static const GLenum GLBufferAccessConverter[GpuBuffer::BA_COUNT];
		static const GLenum GLIndexBufferTypeConverter[IndexBuffer::IT_COUNT];
		/*********************************************************************************/
	public:
		GpuBufferImpl();
		virtual ~GpuBufferImpl();

		virtual bool lock(uint32 p_StartPos,int32 p_Count) OVERRIDE;
		virtual bool unlock() OVERRIDE;

		virtual bool allocate(uint32 p_Size) OVERRIDE;

		virtual uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset) OVERRIDE;
		virtual uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset) OVERRIDE;

		GLuint getBufferGL() const;

	private:
		GLuint m_BufferHandle;
# ifndef K15_WGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
		byte* m_MappedBufferRange;
# endif //K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
	};// end of GpuBufferImpl class declaration
}}}//end of K15_Engine::Rendering::WGL namespace

#endif //_K15Engine_Renderer_OpenGL_GpuBufferImpl_h_