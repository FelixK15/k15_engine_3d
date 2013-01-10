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
#pragma once

#ifndef __K15_HASHEDSTRING__
#define __K15_HASHEDSTRING__

#include "K15_StdInclude.h"
#include "K15_String.h"

namespace K15_EngineV2
{
	class K15ENGINE2_API HashedString
	{
	public:
		explicit HashedString();
		explicit HashedString(K15_CHAR *string);
		~HashedString();

		const U32 getIdentifier() const;
		U32 getIdentifier();

		//const String& getString() const;
		//String& getString();

		bool operator< (HashedString const &otherString) const;
		bool operator== (HashedString const &otherString) const;

		static void * createIdentifier(K15_CHAR *string);

	private:
		void *m_identifier;
	};
}

#endif //__K15_HASHEDSTRING__