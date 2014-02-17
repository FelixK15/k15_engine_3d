/**
 * @file K15_SerialWriter.cpp
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

#include "K15_SerialWriter.h"

namespace K15_Engine { namespace Serialization {
	/*********************************************************************************/
	SerialWriter::SerialWriter(uint32 p_Size)
	: SerializerBase(ST_WRITER),
	m_Buffer(static_cast<byte*>(K15_MALLOC(p_Size))),
	m_Length(p_Size)
	{

	}
	/*********************************************************************************/
	SerialWriter::~SerialWriter()
	{
		K15_FREE(m_Buffer);
	}
	/*********************************************************************************/
	byte* SerialWriter::getBufferPointer() const
	{
		return m_Buffer;
	}
	/*********************************************************************************/
	void SerialWriter::_io(byte* p_Data, uint32 p_Size)
	{
		static const uint32 size_predata = sizeof(int32);

		if(isFailed()) return;

		if(m_CurrentPosition + (p_Size + size_predata) > m_Length)
		{
			m_Failed = true;
			return;
		}

		//write length first and then data
		memcpy(m_Buffer + m_CurrentPosition,&p_Size,size_predata);
		m_CurrentPosition += size_predata;

		memcpy(m_Buffer + m_CurrentPosition,p_Data,p_Size);
		m_CurrentPosition += p_Size;
	}
	/*********************************************************************************/
	void SerialWriter::_io(int32& p_Integer)
	{
		static const uint32 size = sizeof(int32);

		if(isFailed()) return;

		if(m_CurrentPosition + size > m_Length)
		{
			m_Failed = true;
			return;
		}

		memcpy(m_Buffer + m_CurrentPosition,&p_Integer,size);
		m_CurrentPosition += size;
	}
	/*********************************************************************************/
	void SerialWriter::_io(uint32& p_Integer)
	{
		static const uint32 size = sizeof(uint32);

		if(isFailed()) return;

		if(m_CurrentPosition + size > m_Length)
		{
			m_Failed = true;
			return;
		}

		memcpy(m_Buffer + m_CurrentPosition,&p_Integer,size);
		m_CurrentPosition += size;
	}
	/*********************************************************************************/
	void SerialWriter::_io(float& p_Float)
	{
		static const uint32 size = sizeof(float);

		if(isFailed()) return;

		if(m_CurrentPosition + size > m_Length)
		{
			m_Failed = true;
			return;
		}

		memcpy(m_Buffer + m_CurrentPosition,&p_Float,size);
		m_CurrentPosition += size;
	}
	/*********************************************************************************/
}} //end of K15_Engine::Serialization