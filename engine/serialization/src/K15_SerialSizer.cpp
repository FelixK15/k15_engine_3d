/**
 * @file K15_SerialSizer.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/31
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

#include "K15_SerialSizer.h"

namespace K15_Engine { namespace Serialization {
	/*********************************************************************************/
	SerialSizer::SerialSizer()
		: SerializerBase(ST_SIZER)
	{

	}
	/*********************************************************************************/
	SerialSizer::~SerialSizer()
	{

	}
	/*********************************************************************************/
	byte* SerialSizer::getBufferPointer() const
	{
		return 0; //the sizer doesn't have a buffer
	}
	/*********************************************************************************/
	void SerialSizer::_io(byte* p_Data, uint32 p_Size) 
	{
		m_CurrentPosition += sizeof(uint32) + p_Size;
	}
	/*********************************************************************************/
	void SerialSizer::_io(uint32& p_Integer)
	{
		m_CurrentPosition += sizeof(uint32);
	}
	/*********************************************************************************/
	void SerialSizer::_io(int32& p_Integer)
	{
		m_CurrentPosition += sizeof(int32);
	}
	/*********************************************************************************/
	void SerialSizer::_io(float& p_Float)
	{
		m_CurrentPosition += sizeof(float);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace