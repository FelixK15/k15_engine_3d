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
		  m_BufferHandle(0)
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
		if(m_Buffer->isLocked())
		{
			return false;
		}

		return true;
	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::unlock()
	{
		if(m_Buffer->isLocked())
		{
			return true;
		}

		return false;
	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::allocate(uint32 p_Size)
	{
		GLenum target = GLBufferTypeConverter[m_Buffer->getType()];
		GLenum usage = GLBufferUsageConverter[m_Buffer->getUsageOption()];
		glBindBuffer(target,m_BufferHandle);
		glBufferData(target,p_Size,0,usage);
		
		if(glGetError() == GL_OUT_OF_MEMORY)
		{
			glBindBuffer(target,0);
			_LogError("Video card is out of memory.");
			return false;
		}
		
		glBindBuffer(target,0);
		return true;
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
// 		K15_ASSERT(p_Destination,"Destination memory is NULL!");
// 
// 		if(p_Destination)
// 		{
// 			if(p_Size > m_Buffer->getSize())
// 			{
// 				_LogWarning("Buffer size is %ibyte and you want to read %ibyte. Clamping value to max buffer size.",m_Buffer->getSize(),p_Size);
// 				p_Size = m_Buffer->getSize();
// 			}
// 
// 			if((p_Size + p_Offset) > m_Buffer->getSize())
// 			{
// 				uint32 newOffset = p_Size - m_Buffer->getSize();
// 				_LogWarning("Buffer size is %ibyte and you want to read %ibyte starting at %ibyte. Setting offset = %i.",m_Buffer->getSize(),p_Size,p_Offset,newOffset);
// 				p_Offset = newOffset;
// 			}
// 
// 			
// 			return p_Size;
// 		}

		return 0;
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		K15_ASSERT(p_Source,"Source memory is NULL!");

		if(p_Size > m_Buffer->getSize())
		{
			_LogWarning("Buffer size is %ibyte and you want to write %ibyte. Clamping value to max buffer size.",m_Buffer->getSize(),p_Size);
			p_Size = m_Buffer->getSize();
		}

		if((p_Size + p_Offset) > m_Buffer->getSize())
		{
			uint32 newOffset = p_Size - m_Buffer->getSize();
			_LogWarning("Buffer size is %ibyte and you want to write %ibyte starting at %ibyte. Setting offset = %i.",m_Buffer->getSize(),p_Size,p_Offset,newOffset);
			p_Offset = newOffset;
		}
		GLenum target = GLBufferTypeConverter[m_Buffer->getType()];
		
		glBindBuffer(target,m_BufferHandle);
		glBufferSubData(target,p_Offset,p_Size,p_Source);
		glBindBuffer(target,0);

		return p_Size;
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace