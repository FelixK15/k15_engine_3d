/**
 * @file K15_HashedString.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#ifndef _K15Engine_Core_HashedString_h_
#define _K15Engine_Core_HashedString_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core { 
	class K15_CORE_API HashedString
	{
	public:
		static const HashedString BLANK;

	public:
		HashedString();
		HashedString(uint32 p_HashValue);
		HashedString(const char* p_String);
		HashedString(const String& p_String);
		HashedString(const HashedString& p_Other);
		~HashedString();

		unsigned long getIdentifier() const;

#		ifndef K15_NO_STRINGS
			const char* c_str() const;
			const String toString() const;
#		endif

		bool operator< (const HashedString& p_Other) const;
		bool operator== (const HashedString& p_Other) const;
		
		const HashedString& operator=(const HashedString& p_Other);

		static unsigned long createHash(const char* p_String,unsigned int p_Length);

	private:
		unsigned long m_Hash;
#		ifndef K15_NO_STRINGS
			/*String m_String;*/
			String m_String;
#		endif
	};//end of HashedString class
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_HashedString_h_