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

#ifndef _K15Engine_Container_HashItem_h_
#define _K15Engine_Container_HashItem_h_

#include "K15_StdInclude.h"

namespace K15_Engine { namespace Container { 

	template <class TKEY,class TVALUE>
	class HashItem
	{
		template<class TKEY,class TVALUE,uint32 AMOUNT_BUCKETS> friend class HashMap;

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
}} //end of K15_Engine::Container namespace

#endif //_K15Engine_Container_HashItem_h_