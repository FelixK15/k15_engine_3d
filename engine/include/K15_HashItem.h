/**
 * @file K15_HashItem.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/09
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

#ifndef __K15_HASHITEM__
#define __K15_HASHITEM__

#include "K15_StdInclude.h"

namespace K15_EngineV2
{
	template <class TKEY,class TVALUE>
	class HashItem
	{
		template<class TKEY,class TVALUE,U32 AMOUNT_BUCKETS> friend class HashMap;

	public:
		HashItem();
		HashItem(TKEY tKey,TVALUE tValue,HashItem *pNext);

		TKEY &GetKey();
		TVALUE &GetValue();

		HashItem<TKEY,TVALUE> *GetNext();
	private:
		TKEY m_tKey;
		TVALUE m_tValue;
		HashItem *m_pNext;
	};

	#include "../src/K15_HashItem.inl"
}

#endif //__K15_HASHITEM__