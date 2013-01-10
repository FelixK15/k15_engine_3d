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
#include "K15_HashedString.h"

using namespace K15_EngineV2;

HashedString::HashedString()
	: m_identifier(0)
{

}

HashedString::HashedString( K15_CHAR *string )
	: m_identifier(createIdentifier(string))
{

}

HashedString::~HashedString()
{
	//K15_DELETE_ARR m_string;
}

const U32 HashedString::getIdentifier() const
{
	return reinterpret_cast<U32>(m_identifier);
}

U32 HashedString::getIdentifier()
{
	return reinterpret_cast<U32>(m_identifier);
}

/*const String& HashedString::getString() const
{
	return m_string;
}

String& HashedString::getString()
{
	return m_string;
}*/

bool HashedString::operator< (HashedString const & otherString ) const
{
	return this->getIdentifier() < otherString.getIdentifier();
}

bool HashedString::operator== ( HashedString const & otherString ) const
{
	return this->getIdentifier() == otherString.getIdentifier();
}

void * HashedString::createIdentifier(char *string )
{
#define DO1(buf,i) {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i) DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i) DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i) DO4(buf,i); DO4(buf,i+4);
#define DO16(buf) DO8(buf,0); DO8(buf,8);
#define BASE 65521L
#define NMAX 5552
	
	const char *c_string = string;//string.C_Str();

	U32 s1 = 0;
	U32 s2 = 0;

	for(U32 len = strlen(c_string);len > 0;)
	{
		U32 k = len < NMAX ? len : NMAX;
		len -= k;
		while(k >= 16)
		{
			DO16(string);
			c_string += 16;
			k -= 16;
		}

		if(k != 0) do
		{
			s1 += *c_string++;
			s2 += s1;
		} while(--k);

		s1 %= BASE;
		s2 %= BASE;

	}

#pragma warning(push)
#pragma warning(disable : 4312)

	return reinterpret_cast<void*>((s2 << 16) | s1);

#pragma warning(pop)
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16

}


