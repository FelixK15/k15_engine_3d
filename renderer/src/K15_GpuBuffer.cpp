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

#include "K15_RendererPrecompiledHeader.h"
#include "K15_GpuBuffer.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	GpuBuffer::GpuBuffer(Enum p_BufferType)
		: m_Locked(false),
		  m_LockOption(LO_NORMAL),
		  m_ShadowCopy(0),
		  m_ShadowCopyEnabled(false),
		  m_ShadowCopySize(0),
		  m_UsageOption(UO_DYNAMIC),
		  m_BufferType(p_BufferType)
	{
		//m_Impl = g_Renderer
	}
	/*********************************************************************************/
	GpuBuffer::GpuBuffer(Enum p_BufferType,Enum p_LockOption, Enum p_UsageOption, bool p_ShadowCopyEnabled)
		: m_Locked(false),
		  m_LockOption(p_LockOption),
		  m_UsageOption(p_UsageOption),
		  m_ShadowCopyEnabled(true),
		  m_ShadowCopySize(0),
		  m_ShadowCopy(0),
		  m_BufferType(p_BufferType)
	{

	}
	/*********************************************************************************/
	GpuBuffer::GpuBuffer(Enum p_BufferType,Enum p_LockOption, Enum p_UsageOption, uint32 p_InitialDataSize, byte* p_InitialData, uint32 p_InitialDataOffset, bool p_ShadowCopyEnabled)
		: m_Locked(false),
		  m_LockOption(p_LockOption),
		  m_ShadowCopy(p_InitialData),
		  m_ShadowCopyEnabled(true),
		  m_ShadowCopySize(p_InitialDataSize),
		  m_UsageOption(p_UsageOption),
		  m_BufferType(p_BufferType)
	{
		writeData(p_InitialDataSize,p_InitialData,p_InitialDataOffset);
	}
	/*********************************************************************************/
	void GpuBuffer::lock()
	{
		K15_ASSERT(isLocked(),"GpuBuffer is already locked.");

		if(m_Impl->lock())
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
		if(p_FromShadowCopy)
		{
			return readFromShadowCopy(p_Size,p_Destination,p_Offset);
		}

		return m_Impl->readData(p_Size,p_Destination,p_Offset);
	}
	/*********************************************************************************/
	uint32 GpuBuffer::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		if(getShadowCopyEnabled())
		{
			writeToShadowCopy(p_Size,p_Source,p_Offset);
		}

		return m_Impl->writeData(p_Size,p_Source,p_Offset);
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
}}//end of K15_Engine::Rendering namespace