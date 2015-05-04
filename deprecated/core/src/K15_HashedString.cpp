/**
 * @file K15_HashedString.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/11
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

#include "K15_HashedString.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const HashedString HashedString::BLANK("0x0");
	/*********************************************************************************/
	HashedString::HashedString()
		: m_Hash(0),
		  m_String()
	{
		
	}
	/*********************************************************************************/
	HashedString::HashedString(uint32 p_HashValue)
		: m_Hash(p_HashValue),
		m_String()
	{

	}
	/*********************************************************************************/
	HashedString::HashedString(const char *pString)
		: m_Hash(createHash(pString,strlen(pString))),
		  m_String(pString)
	{

	}
	/*********************************************************************************/
	HashedString::HashedString(const HashedString& hsOther)
		: m_Hash(hsOther.m_Hash),
		  m_String(hsOther.m_String)
	{
	
	}
	/*********************************************************************************/
	HashedString::HashedString(const String& p_String)
		: m_Hash(createHash(p_String.c_str(),p_String.size())),
		  m_String(p_String)
	{

	}
	/*********************************************************************************/
	HashedString::~HashedString()
	{
		
	}
	/*********************************************************************************/
	unsigned long HashedString::getIdentifier() const
	{
		return m_Hash;
	}
#ifndef K15_NO_STRINGS
	/*********************************************************************************/
	const char* HashedString::c_str() const
	{
		return m_String.c_str();
	}
	/*********************************************************************************/
	const String HashedString::toString() const
	{
		return m_String;
	}
	/*********************************************************************************/
#endif //K15_NO_STRINGS
	bool HashedString::operator< (HashedString const & otherString) const
	{
		return this->getIdentifier() < otherString.getIdentifier();
	}
	/*********************************************************************************/
	bool HashedString::operator== (HashedString const & otherString) const
	{
		return this->getIdentifier() == otherString.getIdentifier();
	}
	/*********************************************************************************/
	unsigned long HashedString::createHash(const char *pString,unsigned int iLength)
	{
		unsigned long hash = 0;
		unsigned int i;

		for (i=0; i<iLength; i++) 
		{
			hash = 33*hash + 720 + pString[i];
		}

		return hash;
	}
	/*********************************************************************************/
	const HashedString& HashedString::operator=(const HashedString& p_Other)
	{
		m_Hash = p_Other.m_Hash;
#ifndef K15_NO_STRINGS
		m_String = p_Other.m_String;
#endif

		return *this;
	}
	/*********************************************************************************/
}} //end of K15_Engine::Core namespace

