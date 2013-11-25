/**
 * @file K15_GpuBuffer.cpp
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

#include "K15_PrecompiledHeader.h"
#include "K15_GpuBuffer.h"
#include "K15_RendererBase.h"
#include "K15_RenderTask.h"
#include "K15_LogManager.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	GpuBufferImplBase::GpuBufferImplBase()
		: m_Buffer(0)
	{
		
	}
	/*********************************************************************************/
	GpuBufferImplBase::~GpuBufferImplBase()
	{

	}
	/*********************************************************************************/
	void GpuBufferImplBase::setBuffer(GpuBuffer* p_Buffer)
	{
		m_Buffer = p_Buffer;
	}
	/*********************************************************************************/
	GpuBuffer* GpuBufferImplBase::getBuffer() const
	{
		return m_Buffer;
	}
	/*********************************************************************************/

	/*********************************************************************************/
	GpuBuffer::GpuBuffer(Enum p_BufferType, Enum p_LockOption, Enum p_UsageOption, Enum p_AccessOption, bool p_ShadowCopyEnabled)
		: m_Locked(false),
		  m_LockOption(p_LockOption),
		  m_UsageOption(p_UsageOption),
		  m_ShadowCopyEnabled(true),
		  m_ShadowCopySize(0),
		  m_ShadowCopy(0),
		  m_Size(0),
		  m_BufferType(p_BufferType),
		  m_AccessOption(p_AccessOption)
	{
 		m_Impl = g_Application->getRenderTask()->getRenderer()->createGpuBufferImpl();
		m_Impl->setBuffer(this);
 	}
	/*********************************************************************************/
	GpuBuffer::GpuBuffer(Enum p_BufferType, uint32 p_InitialDataSize, byte* p_InitialData, uint32 p_InitialDataOffset, Enum p_LockOption, Enum p_UsageOption, Enum p_AccessOption, bool p_ShadowCopyEnabled)
		: m_Locked(false),
		  m_LockOption(p_LockOption),
		  m_ShadowCopy(p_InitialData),
		  m_ShadowCopyEnabled(true),
		  m_ShadowCopySize(p_InitialDataSize),
		  m_UsageOption(p_UsageOption),
		  m_BufferType(p_BufferType),
		  m_AccessOption(p_AccessOption)
	{
// 		m_Impl = g_Renderer->getGpuBufferImpl();
		writeData(p_InitialDataSize,p_InitialData,p_InitialDataOffset);
	}
	/*********************************************************************************/
	GpuBuffer::~GpuBuffer()
	{
		K15_DELETE m_Impl;

		if(m_ShadowCopy)
		{
			K15_DELETE[] m_ShadowCopy;
		}
	}
	/*********************************************************************************/
	void GpuBuffer::lock(uint32 p_StartPos, int32 p_Count)
	{
		K15_ASSERT(isLocked(),"GpuBuffer is already locked.");

		if(m_Impl->lock(p_StartPos,p_Count))
		{
			m_Locked = true;
		}
	}
	/*********************************************************************************/
	void GpuBuffer::unlock()
	{
		K15_ASSERT(!isLocked(),"GpuBuffer is not locked.");

		if(m_Impl->unlock())
		{
			m_Locked = false;
		}
	}
	/*********************************************************************************/
	uint32 GpuBuffer::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset, bool p_FromShadowCopy)
	{
		bool wasLocked = false;
		if(!m_AccessOption == BA_WRITE_ONLY)
		{
			_LogWarning("Not allowed to read directly from GPU buffer due to limited access option (Buffer is write only)...Trying to read from shadow copy.");
			p_FromShadowCopy = true;
		}

		if(p_FromShadowCopy)
		{
			if(getShadowCopyEnabled())
			{
				return readFromShadowCopy(p_Size,p_Destination,p_Offset);
			}
			else
			{
				_LogError("Shadow copy not enabled. Can't read any data.");
				return 0;
			}
		}

		if(!isLocked())
		{
			wasLocked = true;
			lock(p_FromShadowCopy,p_Size);
		}

		uint32 dataCount = m_Impl->readData(p_Size,p_Destination,p_Offset);

		if(wasLocked)
		{
			unlock();
		}

		return dataCount;
	}
	/*********************************************************************************/
	uint32 GpuBuffer::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		bool wasLocked = false;
		if(!m_AccessOption == BA_READ_ONLY)
		{
			_LogWarning("Not allowed to write directly to GPU buffer due to limited access option. (Buffer is read only)");
			return 0;
		}

		if(getShadowCopyEnabled())
		{
			writeToShadowCopy(p_Size,p_Source,p_Offset);
		}

		if(m_Size < p_Size + p_Offset)
		{
			K15_ASSERT(allocate(p_Size + p_Offset),"Could not allocate memory for GpuBuffer.");
			m_Impl->writeData(m_ShadowCopySize,m_ShadowCopy,0); //write back shadow copy
		}

		if(!isLocked())
		{
			lock(p_Offset,p_Size);
			wasLocked = true;
		}

		uint32 dataCount = m_Impl->writeData(p_Size,p_Source,p_Offset);

		if(wasLocked)
		{
			unlock();
		}

		return dataCount;
	}
	/*********************************************************************************/
	void GpuBuffer::writeToShadowCopy(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		if(p_Size > m_ShadowCopySize || !m_ShadowCopy)
		{
			//create a new shadow buffer
			byte* buffer = new byte[p_Size];
			memcpy(buffer,p_Source + p_Offset,p_Size);
			memset(buffer,0,p_Size);

			if(m_ShadowCopy)
			{
				delete[] m_ShadowCopy;
			}

			m_ShadowCopy = buffer;
		}
		else
		{
			memcpy(m_ShadowCopy,p_Source + p_Offset,p_Size);
		}
	}
	/*********************************************************************************/
	uint32 GpuBuffer::readFromShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		memcpy(p_Destination,m_ShadowCopy + p_Offset,p_Size);

		return p_Size;
	}
	/*********************************************************************************/
	bool GpuBuffer::allocate(uint32 p_Size)
	{
		if(m_Impl->allocate(p_Size))
		{
			m_Size = p_Size;
			return true;
		}

		return false;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace