/**
 * @file K15_GpuBufferImplOGL.cpp
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

#include "K15_GpuBufferImplOGL.h"
#include "K15_LogManager.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	const GLenum GpuBufferImplOGL::GLBufferTypeConverter[GpuBuffer::BT_COUNT] = {
		GL_VERTEX_ARRAY,		 //BT_VERTEX_BUFFER
		GL_ELEMENT_ARRAY_BUFFER, //BT_INDEX_BUFFER
	};//GLBufferTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum GpuBufferImplOGL::GLBufferUsageConverter[GpuBuffer::UO_COUNT] = {
		GL_STATIC_DRAW, //UO_STATIC
		GL_DYNAMIC_DRAW //UO_DYNAMIC
	};//GLBufferTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum GpuBufferImplOGL::GLBufferAccessConverter[GpuBuffer::BA_COUNT] = {
		GL_READ_ONLY,	//BA_READ_ONLY
		GL_WRITE_ONLY,	//BA_WRITE_ONLY
		GL_READ_WRITE	//BA_READ_WRITE
	};//GLBufferAccessConverter
	/*********************************************************************************/

	/*********************************************************************************/
	GpuBufferImplOGL::GpuBufferImplOGL()
		: GpuBufferImplBase(),
		  m_BufferHandle(0),
		  m_BufferMemory(0),
		  m_BufferMemorySize(0)
	{
		glGenBuffers(1,&m_BufferHandle);
	}
	/*********************************************************************************/
	GpuBufferImplOGL::~GpuBufferImplOGL()
	{
		glDeleteBuffers(1,&m_BufferHandle);
	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::lock(uint32 p_StartPos,int32 p_Count)
	{
		GLenum bufferType = GLBufferTypeConverter[m_Buffer->getType()];
		GLenum bufferAccess = GLBufferAccessConverter[m_Buffer->getAccessOption()];
		glBindBuffer(bufferType,m_BufferHandle);

		m_BufferMemory = (byte*)glMapBufferRange(bufferType,p_StartPos,p_Count,bufferAccess);
		
		if(!m_BufferMemory)
		{
			_LogError("Error mapping buffer from startpos %u with size %i. Error:%s",p_StartPos,p_Count,glGetString(glGetError()));
			return false;
		}

		m_BufferMemorySize = p_Count;

		glBindBuffer(bufferType,0);

		return true;
	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::unlock()
	{
		GLenum bufferType = GLBufferTypeConverter[m_Buffer->getType()];
		glBindBuffer(bufferType,m_BufferHandle);
		if(glUnmapBuffer(bufferType) == GL_FALSE)
		{
			_LogError("Error unmapping buffer. Error:%s",glGetString(glGetError()));
			return false;
		}

		//ptr to memory is invalid now
		m_BufferMemory = 0;
		m_BufferMemorySize = 0;

		glBindBuffer(bufferType,0);

		return true;
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		K15_ASSERT(m_BufferMemory,"Buffer memory pointer is NULL! Buffer probably not locked.");
		K15_ASSERT(p_Destination,"Destination memory is NULL!");

		if(m_BufferMemory && p_Destination)
		{
			if(p_Size > m_BufferMemorySize)
			{
				_LogWarning("Buffer size is %ibyte and you want to read %ibyte. Clamping value to max buffer size.",m_BufferMemorySize,p_Size);
				p_Size = m_BufferMemorySize;
			}

			if((p_Size + p_Offset) > m_BufferMemorySize)
			{
				uint32 newOffset = p_Size - m_BufferMemorySize;
				_LogWarning("Buffer size is %ibyte and you want to read %ibyte starting at %ibyte. Setting offset = %i.",m_BufferMemorySize,p_Size,p_Offset,newOffset);
				p_Offset = newOffset;
			}

			memcpy(p_Destination,m_BufferMemory + p_Offset,p_Size);

			return p_Size;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		K15_ASSERT(m_BufferMemory,"Buffer memory pointer is NULL! Buffer probably not locked.");
		K15_ASSERT(p_Source,"Source memory is NULL!");

		if(m_BufferMemory && p_Source)
		{
			if(p_Size > m_BufferMemorySize)
			{
				_LogWarning("Buffer size is %ibyte and you want to write %ibyte. Clamping value to max buffer size.",m_BufferMemorySize,p_Size);
				p_Size = m_BufferMemorySize;
			}

			if((p_Size + p_Offset) > m_BufferMemorySize)
			{
				uint32 newOffset = p_Size - m_BufferMemorySize;
				_LogWarning("Buffer size is %ibyte and you want to write %ibyte starting at %ibyte. Setting offset = %i.",m_BufferMemorySize,p_Size,p_Offset,newOffset);
				p_Offset = newOffset;
			}

			memcpy(m_BufferMemory + p_Offset,p_Source,p_Size);

			return p_Size;
		}
		
		return 0;
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace