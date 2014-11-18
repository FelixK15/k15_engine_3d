/**
 * @file K15_GpuBufferImpl.cpp
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
 */

#include "K15_OpenGL_PrecompiledHeader.h"

#include "K15_OpenGL_GpuBufferImpl.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
	const GLenum GpuBufferImpl::GLBufferTypeConverter[GpuBuffer::BT_COUNT] = {
		GL_ARRAY_BUFFER,		 //BT_VERTEX_BUFFER
		GL_ELEMENT_ARRAY_BUFFER, //BT_INDEX_BUFFER
	};//GLBufferTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum GpuBufferImpl::GLBufferUsageConverter[GpuBuffer::UO_COUNT] = {
		GL_STATIC_DRAW,		//UO_STATIC
		GL_DYNAMIC_DRAW,	//UO_DYNAMIC
		GL_STREAM_DRAW,		//UI_STREAM
	};//GLBufferTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum GpuBufferImpl::GLBufferAccessConverter[GpuBuffer::BA_COUNT] = {
		GL_READ_ONLY,	//BA_READ_ONLY
		GL_WRITE_ONLY,	//BA_WRITE_ONLY
		GL_READ_WRITE	//BA_READ_WRITE
	};//GLBufferAccessConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum GpuBufferImpl::GLIndexBufferTypeConverter[IndexBuffer::IT_COUNT] = {
		GL_UNSIGNED_BYTE,   //IT_UINT8
		GL_UNSIGNED_SHORT,  //IT_UINT16
		GL_UNSIGNED_INT     //IT_UINT32
	};//GLIndexBufferTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	GpuBufferImpl::GpuBufferImpl()
		: GpuBufferImplBase(),
		  m_BufferHandle(0)
	{
		glGenBuffers(1,&m_BufferHandle);
	}
	/*********************************************************************************/
	GpuBufferImpl::~GpuBufferImpl()
	{
		glDeleteBuffers(1,&m_BufferHandle);
	}
	/*********************************************************************************/
	bool GpuBufferImpl::lock(uint32 p_StartPos,int32 p_Count)
	{
		if(m_Buffer->isLocked())
		{
			return false;
		}

# if defined K15_WGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
		return true;
# else

		GLenum target = GLBufferTypeConverter[m_Buffer->getType()];
		GLbitfield flags = GL_MAP_WRITE_BIT;

# if defined K15_WGL_EXPERIMENT_MAP_BUFFER_WITH_UNSYNCHRONIZED
		flags |= GL_MAP_UNSYNCHRONIZED_BIT;
# endif //K15_OGL_EXPERIMENT_MAP_BUFFER_WITH_UNSYNCHRONIZED

#if defined K15_WGL_EXPERIMENT_MAP_BUFFER_WITH_INVALIDATE_RANGE
    flags |= GL_MAP_INVALIDATE_RANGE_BIT;
# endif //K15_OGL_EXPERIMENT_MAP_BUFFER_WITH_INVALIDATE_RANGE

		glBindBuffer(target,m_BufferHandle);
		m_MappedBufferRange = (byte*)glMapBufferRange(target,p_StartPos,p_Count,flags);
		glBindBuffer(target,0);
		return m_MappedBufferRange != 0;
# endif // K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
	}
	/*********************************************************************************/
	bool GpuBufferImpl::unlock()
	{
		if(!m_Buffer->isLocked())
		{
			return true;
		}

# if defined K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
		return false;
# else

		GLenum target = GLBufferTypeConverter[m_Buffer->getType()];
		glBindBuffer(target,m_BufferHandle);
		if(glUnmapBuffer(target) == GL_TRUE)
		{
			m_MappedBufferRange = 0;
			glBindBuffer(target,0);
			return true;
		}
	
		glBindBuffer(target,0);
		return false;
# endif // K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
	}
	/*********************************************************************************/
	bool GpuBufferImpl::allocate(uint32 p_Size)
	{
		GLenum usage = GLBufferUsageConverter[m_Buffer->getUsageOption()];

		kglNamedBufferDataEXT(m_BufferHandle,p_Size,0,usage);
	
		return true;
	}
	/*********************************************************************************/
	uint32 GpuBufferImpl::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		K15_ASSERT(p_Destination,"Destination memory is NULL!");

		if(p_Destination)
		{
			if(p_Size > m_Buffer->getSize())
			{
				K15_LOG_WARNING("Buffer size is %ibyte and you want to read %ibyte. Clamping value to max buffer size.",m_Buffer->getSize(),p_Size);
				p_Size = m_Buffer->getSize();
			}

			if((p_Size + p_Offset) > m_Buffer->getSize())
			{
				uint32 newOffset = p_Size - m_Buffer->getSize();
				K15_LOG_WARNING("Buffer size is %ibyte and you want to read %ibyte starting at %ibyte. Setting offset = %i.",m_Buffer->getSize(),p_Size,p_Offset,newOffset);
				p_Offset = newOffset;
			}

#if defined K15_WGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
			glGetNamedBufferSubDataEXT(m_BufferHandle,p_Offset,p_Size,p_Destination);
#else
			memcpy(p_Destination,m_MappedBufferRange,p_Size);
#endif //K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

			return p_Size;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 GpuBufferImpl::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		K15_ASSERT(p_Source,"Source memory is NULL!");

		if(p_Size > m_Buffer->getSize())
		{
			K15_LOG_WARNING("Buffer size is %i byte and you want to write %i byte. Clamping value to max buffer size.",m_Buffer->getSize(),p_Size);
			p_Size = m_Buffer->getSize();
		}

		if((p_Size + p_Offset) > m_Buffer->getSize())
		{
			uint32 newOffset = p_Size - m_Buffer->getSize();
			K15_LOG_WARNING("Buffer size is %i byte and you want to write %i byte starting at %i byte. Setting offset = %i.",m_Buffer->getSize(),p_Size,p_Offset,newOffset);
			p_Offset = newOffset;
		}
		GLenum target = GLBufferTypeConverter[m_Buffer->getType()];

#if defined K15_GL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
		kglNamedBufferSubDataEXT(m_BufferHandle,p_Offset,p_Size,p_Source);
#else
		memcpy(m_MappedBufferRange,p_Source,p_Size);
#endif //K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

		return p_Size;
	}
  /*********************************************************************************/
  GLuint GpuBufferImpl::getBufferGL() const
  {
    return m_BufferHandle;
  }
  /*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace