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
#include "K15_LogManager.h"
#include "K15_IndexBuffer.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const GpuBuffer::CreationOptions GpuBuffer::DefaultOptions = GpuBuffer::CreationOptions();
	/*********************************************************************************/

	/*********************************************************************************/
	GpuBuffer::CreationOptions::CreationOptions()
		: BufferType(GpuBuffer::BT_VERTEX_BUFFER),
		LockOption(GpuBuffer::LO_NORMAL),
		UsageOption(GpuBuffer::UO_DYNAMIC),
		AccessOption(GpuBuffer::BA_READ_WRITE),
		IndexType(IndexBuffer::IT_UINT16),
		VertexLayout(0)
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	GpuBuffer::PendingChange::PendingChange(uint32 p_Size, uint32 p_Offset)
		: size(p_Size),
		offset(p_Offset)
	{

	}
	/*********************************************************************************/

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
	GpuBuffer::GpuBuffer(const CreationOptions& p_Options)
		: m_Locked(false),
		m_LockOption(p_Options.LockOption),
		m_ShadowCopy(0),
		m_Dirty(false),
		m_ShadowCopySize(0),
		m_UsageOption(p_Options.UsageOption),
		m_BufferType(p_Options.BufferType),
		m_AccessOption(p_Options.AccessOption),
		m_Size(0),
		m_UsedSize(0)
	{
		K15_ASSERT(m_UsageOption >= 0 && m_UsageOption < UO_COUNT,"Invalid usage option.");
		K15_ASSERT(m_AccessOption >= 0 && m_AccessOption < BA_COUNT,"Invalid access option.");
		K15_ASSERT(m_BufferType >= 0 && m_BufferType < BT_COUNT,"Invalid buffer type.");
		K15_ASSERT(m_LockOption >= 0 && m_LockOption < LO_COUNT,"Invalid lock option.");

 		m_Impl = g_Application->getRenderer()->createGpuBufferImpl();
		m_Impl->setBuffer(this);
		if(p_Options.InitialData.size > 0)
		{
			writeData(p_Options.InitialData.size,p_Options.InitialData.data,0);
		}
	}
	/*********************************************************************************/
	GpuBuffer::~GpuBuffer()
	{
		K15_DELETE m_Impl;

		if(m_ShadowCopy)
		{
			K15_DELETE_SIZE(Allocators[AC_RENDERING],m_ShadowCopy,m_ShadowCopySize);
		}
	}
	/*********************************************************************************/
	void GpuBuffer::lock(uint32 p_StartPos, int32 p_Count)
	{
		K15_ASSERT(!isLocked(),"GpuBuffer is already locked.");

		if(m_Impl->lock(p_StartPos,p_Count))
		{
			m_Locked = true;
		}
	}
	/*********************************************************************************/
	void GpuBuffer::unlock()
	{
		K15_ASSERT(isLocked(),"GpuBuffer is not locked.");

		if(m_Impl->unlock())
		{
			m_Locked = false;
		}
	}
	/*********************************************************************************/
	uint32 GpuBuffer::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset, bool p_ReadFromGpuBuffer)
	{
		K15_ASSERT(p_Size + p_Offset <= m_ShadowCopySize,"Trying to read out of bounds.");

		if(!p_ReadFromGpuBuffer)
		{
			return _readFromShadowCopy(p_Size,p_Destination,p_Offset);
		}

		bool wasLocked = true;

		if(!isLocked())
		{
			lock(p_Offset,p_Size);
			wasLocked = false;
		}

		uint32 bytesRead = m_Impl->readData(p_Size,p_Destination,p_Offset);

		K15_ASSERT(bytesRead == p_Size,
			StringUtil::format("Could not read %ubytes from GpuBuffer. Only read %ubytes.",p_Size,bytesRead));

		if(wasLocked)
		{
			unlock();
		}

		return bytesRead;
	}
	/*********************************************************************************/
	uint32 GpuBuffer::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset, bool p_WriteToGpuBuffer)
	{
		if(m_AccessOption == BA_READ_ONLY && p_WriteToGpuBuffer)
		{
			_LogWarning("Not allowed to write directly to GPU buffer due to limited access option. (Buffer is read only)");
			return 0;
		}

		if(m_Size < p_Size + p_Offset)
		{
			K15_ASSERT(allocate(p_Size + p_Offset,p_WriteToGpuBuffer),"Could not allocate memory for GpuBuffer.");
		}

		m_Dirty = true;
		if(p_WriteToGpuBuffer)
		{
			if(p_Source != m_ShadowCopy)
			{
				_writeToShadowCopy(p_Size,p_Source,p_Offset);
			}
		}
		else
		{
			_addPendingChange(p_Size,p_Offset);
			return _writeToShadowCopy(p_Size,p_Source,p_Offset);
		}
		
		bool wasLocked = false;

		if(!isLocked())
		{
			lock(p_Offset,p_Size);
			wasLocked = true;
		}

		uint32 bytesWritten = m_Impl->writeData(p_Size,p_Source,p_Offset);
		m_Dirty = false;
		m_UsedSize += bytesWritten;

		if(wasLocked)
		{
			unlock();
		}

		return bytesWritten;
	}
	/*********************************************************************************/
	uint32 GpuBuffer::_writeToShadowCopy(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		memcpy(m_ShadowCopy + p_Offset,p_Source,p_Size);

		return p_Size;
	}
	/*********************************************************************************/
	uint32 GpuBuffer::_readFromShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		memcpy(p_Destination,m_ShadowCopy + p_Offset,p_Size);

		return p_Size;
	}
	/*********************************************************************************/
	bool GpuBuffer::allocate(uint32 p_Size, bool p_AllocateFromGpuBuffer)
	{
		if(!p_AllocateFromGpuBuffer)
		{
			byte* newShadowCopy = K15_NEW_SIZE(Allocators[AC_RENDERING],p_Size) byte;

			if(m_ShadowCopy)
			{
				memcpy(newShadowCopy,m_ShadowCopy,m_ShadowCopySize);
				K15_DELETE_SIZE(Allocators[AC_RENDERING],m_ShadowCopy,m_ShadowCopySize);
			}

			m_ShadowCopy = newShadowCopy;

			m_ShadowCopySize = p_Size;

			return true;
		}
		
		if(m_Impl->allocate(p_Size))
		{
			m_Size = p_Size;
			return true;
		}

		return false;
	}
	/*********************************************************************************/
	bool GpuBuffer::uploadShadowBufferToGpu()
	{
		bool successful = true;
		if(m_Dirty)
		{
			for(uint32 i = 0;i < m_PendingChanges.size();++i)
			{
				PendingChange& change = m_PendingChanges.at(i);

				if(writeData(change.size,m_ShadowCopy + change.offset,change.offset,true) != change.size)
				{
					successful = false;
				}
			}

			m_PendingChanges.clear();
		}

		return successful;
	}
	/*********************************************************************************/
	void GpuBuffer::_addPendingChange(uint32 p_Size, uint32 p_Offset)
	{
		for(uint32 i = 0; i < m_PendingChanges.size();++i)
		{
			PendingChange& change = m_PendingChanges.at(i);
			
			if(p_Offset >= change.offset && p_Offset < change.offset + change.size)
			{
				//we update previously updated memory
				return;
			}
			else if(p_Offset + p_Size == change.offset)
			{
				//we can concatenate two changes as they're consecutive
				change.offset = p_Offset;
				change.size = p_Size + change.size;
				return;
			}
			else if(change.offset + change.size == p_Offset)
			{
				//we can concatenate two changes as they're consecutive
				change.size = p_Size + change.size;
				return;
			}
			else if(p_Offset + p_Size > change.offset && p_Offset + p_Size < change.offset + change.size)
			{
				//we can concatenate two changes as they're consecutive
				change.offset = p_Offset;
				change.size += change.size - p_Size;
				return;
			}
			else if(change.offset + change.size > p_Offset && change.offset + change.size < p_Offset + p_Size)
			{
				//we can concatenate two changes as they're consecutive
				change.size += p_Size - change.size;
				return;
			}
		}
		
		m_PendingChanges.push_back(PendingChange(p_Size,p_Offset));
	}	
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace