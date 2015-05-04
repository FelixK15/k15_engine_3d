/**
 * @file K15_SerialReader.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/20
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

#include "K15_SerialReader.h"

namespace K15_Engine { namespace Serialization {
	/*********************************************************************************/
	SerialReader::SerialReader()
	: SerializerBase(ST_READER),
	m_InputBufferSize(0),
	m_InputBuffer(0)
	{

	}
	/*********************************************************************************/
	SerialReader::~SerialReader()
	{

	}
	/*********************************************************************************/
	byte* SerialReader::getBufferPointer() const
	{
		return m_InputBuffer;
	}
	/*********************************************************************************/
	void SerialReader::_io(byte* p_Data, uint32 p_Size)
	{
		if(isFailed()) return;

		if(m_CurrentPosition + p_Size > m_InputBufferSize)
		{
			m_Failed = true;
			return;
		}

		memcpy(p_Data,m_InputBuffer + m_CurrentPosition,p_Size);
		m_CurrentPosition += p_Size;
	}
	/*********************************************************************************/
	void SerialReader::_io(int32& p_Integer)
	{
		static const uint32 size = sizeof(int32);
		if(isFailed()) return;

		if(m_CurrentPosition + size > m_InputBufferSize)
		{
			m_Failed = true;
			return;
		}
		
		memcpy(&p_Integer,m_InputBuffer + m_CurrentPosition,size);
		m_CurrentPosition += size;
	}
	/*********************************************************************************/
	void SerialReader::_io(uint32& p_Integer)
	{
		static const uint32 size = sizeof(uint32);
		if(isFailed()) return;

		if(m_CurrentPosition + size > m_InputBufferSize)
		{
			m_Failed = true;
			return;
		}

		memcpy(&p_Integer,m_InputBuffer + m_CurrentPosition,size);
		m_CurrentPosition += size;
	}
	/*********************************************************************************/
	void SerialReader::_io(float& p_Float)
	{
		static const uint32 size = sizeof(float);
		if(isFailed()) return;

		if(m_CurrentPosition + size > m_InputBufferSize)
		{
			m_Failed = true;
			return;
		}

		memcpy(&p_Float,m_InputBuffer + m_CurrentPosition,size);
		m_CurrentPosition += size;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Serialization namespace