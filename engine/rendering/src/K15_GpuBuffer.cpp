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
#include "K15_StringUtil.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const GpuBuffer::CreationOptions GpuBuffer::DefaultOptions = GpuBuffer::CreationOptions();
	/*********************************************************************************/

	/*********************************************************************************/
	GpuBuffer::CreationOptions::CreationOptions()
		: BufferType(GpuBuffer::BT_VERTEX_BUFFER),
		UsageOption(GpuBuffer::UO_DYNAMIC),
		AccessOption(GpuBuffer::BA_READ_WRITE),
		IndexType(IndexBuffer::IT_UINT16),
		Size(size_megabyte(1))
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
		m_ShadowCopy(0),
		m_Dirty(false),
		m_UsageOption(p_Options.UsageOption),
		m_BufferType(p_Options.BufferType),
		m_AccessOption(p_Options.AccessOption),
		m_Size(0),
		m_UsedSize(0)
	{
		K15_ASSERT(m_UsageOption >= 0 && m_UsageOption < UO_COUNT,"Invalid usage option.");
		K15_ASSERT(m_AccessOption >= 0 && m_AccessOption < BA_COUNT,"Invalid access option.");
		K15_ASSERT(m_BufferType >= 0 && m_BufferType < BT_COUNT,"Invalid buffer type.");

 		m_Impl = g_Application->getRenderer()->createGpuBufferImpl();
		m_Impl->setBuffer(this);

		_createBufferStorage(p_Options);
	}
	/*********************************************************************************/
	GpuBuffer::~GpuBuffer()
	{
		if(isBound())
		{
			getRenderer()->bindBuffer(0, m_BufferType);
		}

		K15_DELETE m_Impl;
		K15_DELETE_SIZE(Allocators[AC_RENDERING], m_ShadowCopy, m_Size);
	}
	/*********************************************************************************/
	void* GpuBuffer::lock(uint32 p_Size, uint32 p_Offset)
	{
		K15_ASSERT(!isLocked(),"GpuBuffer is already locked.");

		m_Locked = true;
		_addPendingChange(p_Size, p_Offset);

		return m_ShadowCopy + p_Offset;
	}
	/*********************************************************************************/
	void GpuBuffer::unlock()
	{
		K15_ASSERT(isLocked(),"GpuBuffer is not locked.");

		m_Locked = false;
	}
	/*********************************************************************************/
	uint32 GpuBuffer::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		K15_ASSERT(p_Size + p_Offset <= m_Size,"Trying to read out of bounds.");

		//Data will be read from shadow copy.
		return _readFromShadowCopy(p_Size,p_Destination,p_Offset);
	}
	/*********************************************************************************/
	uint32 GpuBuffer::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		K15_ASSERT((p_Offset + p_Size) <= m_Size, "Buffer is not big enough.");

		m_Dirty = true;

		//write to shadow copy and add an entry to the pending changes list.
		//actual gpu buffer will only get updated when really needed or when
		//forcing via forceBufferUpdate();
		_addPendingChange(p_Size,p_Offset);
		return _writeToShadowCopy(p_Size,p_Source,p_Offset);
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
	bool GpuBuffer::uploadShadowBufferToGpu()
	{
		bool successful = true;
		if(m_Dirty)
		{
			for(uint32 i = 0;i < m_PendingChanges.size();++i)
			{
				PendingChange& change = m_PendingChanges.at(i);

				if(m_Impl->writeData(change.size,m_ShadowCopy + change.offset,change.offset) != change.size)
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
		
		m_Dirty = true;
		m_PendingChanges.push_back(PendingChange(p_Size,p_Offset));
	}	
	/*********************************************************************************/
	void GpuBuffer::_createBufferStorage(const CreationOptions& p_Options)
	{
// 		K15_ASSERT(p_Options.InitialData.data || p_Options.Size > 0, 
// 			StringUtil::format("Can't create a GPU Buffer of type \"%s\" with no initial data or size of 0.",
// 								eBufferTypeStr[p_Options.BufferType]));

		uint32 size = p_Options.InitialData.size > 0 ? p_Options.InitialData.size :
													   p_Options.Size;

		K15_ASSERT(m_Impl->allocate(size), 
			StringUtil::format("Could not allocate %.2f kilobyte for GPU Buffer.", size / 1024.f));

		m_Size = size;
		m_ShadowCopy = K15_NEW_SIZE(Allocators[AC_RENDERING], size) byte;

		if(p_Options.InitialData.data)
		{
			writeData(p_Options.InitialData.size, p_Options.InitialData.data);
			uploadShadowBufferToGpu();
		}
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace