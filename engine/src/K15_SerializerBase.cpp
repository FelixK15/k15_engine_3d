/**
 * @file K15_SerializerBase.cpp
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

#include "K15_SerializerBase.h"

namespace K15_Engine { namespace Serialization {
	/*********************************************************************************/
	SerializerBase::SerializerBase(eSerializerType p_Type)
    : m_CurrentPosition(0),
    m_Failed(false),
    m_Type(p_Type)
	{

	}
	/*********************************************************************************/
	SerializerBase::~SerializerBase()
	{

	}
	/*********************************************************************************/
	const SerializerBase& SerializerBase::operator&(String& p_String)
	{
		uint32 length = p_String.length();
		char* data = const_cast<char*>(p_String.c_str());

		if(m_Type == ST_WRITER || m_Type == ST_SIZER)
		{
			_io((byte*)data,length);
		}
		else if(m_Type == ST_READER)
		{
			_io(length);
			data = static_cast<char*>(alloca(length));
			_io((byte*)data,length);
			p_String = data;
		}

		return *this;
	}
	/*********************************************************************************/
	const SerializerBase& SerializerBase::operator&(int32& p_Integer)
	{
		_io(p_Integer);

		return *this;
	}
	/*********************************************************************************/
	const SerializerBase& SerializerBase::operator&(uint32& p_Integer)
	{
		_io(p_Integer);

		return *this;
	}
	/*********************************************************************************/
	const SerializerBase& SerializerBase::operator&(float& p_Float)
	{
		_io(p_Float);
		
		return *this;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace