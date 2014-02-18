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

#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	const GLenum GpuBufferImplOGL::GLBufferTypeConverter[GpuBuffer::BT_COUNT] = {
		GL_ARRAY_BUFFER,		 //BT_VERTEX_BUFFER
		GL_ELEMENT_ARRAY_BUFFER, //BT_INDEX_BUFFER
	};//GLBufferTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum GpuBufferImplOGL::GLBufferUsageConverter[GpuBuffer::UO_COUNT] = {
		GL_STATIC_DRAW,		//UO_STATIC
		GL_DYNAMIC_DRAW,	//UO_DYNAMIC
		GL_STREAM_DRAW,		//UI_STREAM
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
	const GLenum GpuBufferImplOGL::GLIndexBufferTypeConverter[IndexBuffer::IT_COUNT] = {
		GL_UNSIGNED_BYTE,   //IT_UINT8
		GL_UNSIGNED_SHORT,  //IT_UINT16
		GL_UNSIGNED_INT     //IT_UINT32
	};//GLIndexBufferTypeConverter
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

# if defined K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
		return true;
# else

    GLenum target = GLBufferTypeConverter[m_Buffer->getType()];
    GLbitfield flags = 0;

# if defined K15_OGL_EXPERIMENT_MAP_BUFFER_WITH_UNSYNCHRONIZED
    flags |= GL_MAP_UNSYNCHRONIZED_BIT;
# endif

    m_MappedBufferRange = (byte*)glMapBufferRange(target,p_StartPos,p_Count,flags);

    return m_MappedBufferRange != 0;
# endif // K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::unlock()
	{
    if(m_Buffer->isLocked())
    {
      return true;
    }

# if defined K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
    return false;
# else

    GLenum target = GLBufferTypeConverter[m_Buffer->getType()];
    if(glUnmapBuffer(target) == GL_TRUE)
    {
      m_MappedBufferRange = 0;
      return true;
    }

    return false;
# endif // K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::allocate(uint32 p_Size)
	{
		GLenum usage = GLBufferUsageConverter[m_Buffer->getUsageOption()];

		glNamedBufferDataEXT(m_BufferHandle,p_Size,0,usage);
	
		return true;
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		K15_ASSERT(p_Destination,"Destination memory is NULL!");

		if(p_Destination)
		{
			if(p_Size > m_Buffer->getSize())
			{
				_LogWarning("Buffer size is %ibyte and you want to read %ibyte. Clamping value to max buffer size.",m_Buffer->getSize(),p_Size);
				p_Size = m_Buffer->getSize();
			}

			if((p_Size + p_Offset) > m_Buffer->getSize())
			{
				uint32 newOffset = p_Size - m_Buffer->getSize();
				_LogWarning("Buffer size is %ibyte and you want to read %ibyte starting at %ibyte. Setting offset = %i.",m_Buffer->getSize(),p_Size,p_Offset,newOffset);
				p_Offset = newOffset;
			}

#if defined K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
			glGetNamedBufferSubDataEXT(m_BufferHandle,p_Offset,p_Size,p_Destination);
#else
      memcpy(p_Destination,m_MappedBufferRange,p_Size);
#endif //K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

			return p_Size;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		K15_ASSERT(p_Source,"Source memory is NULL!");

		if(p_Size > m_Buffer->getSize())
		{
			_LogWarning("Buffer size is %i byte and you want to write %i byte. Clamping value to max buffer size.",m_Buffer->getSize(),p_Size);
			p_Size = m_Buffer->getSize();
		}

		if((p_Size + p_Offset) > m_Buffer->getSize())
		{
			uint32 newOffset = p_Size - m_Buffer->getSize();
			_LogWarning("Buffer size is %i byte and you want to write %i byte starting at %i byte. Setting offset = %i.",m_Buffer->getSize(),p_Size,p_Offset,newOffset);
			p_Offset = newOffset;
		}
		GLenum target = GLBufferTypeConverter[m_Buffer->getType()];

#if defined K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE
		glNamedBufferSubDataEXT(m_BufferHandle,p_Offset,p_Size,p_Source);
#else
    memcpy(m_MappedBufferRange,p_Source,p_Size);
#endif //K15_OGL_EXPERIMENT_BUFFERSUBDATA_INSTEAD_OF_MAPBUFFERRANGE

		return p_Size;
	}
  /*********************************************************************************/
  GLuint GpuBufferImplOGL::getBufferGL() const
  {
    return m_BufferHandle;
  }
  /*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace