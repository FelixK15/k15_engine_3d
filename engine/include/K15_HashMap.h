/**
 * @file K15_HashMap.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/06
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

#ifndef __K15_HASHMAP__
#define __K15_HASHMAP__

#include "K15_StdInclude.h"
#include "K15_HashItem.h"

namespace K15_EngineV2
{
	template <class TKEY,class TVALUE,uint32 AMOUNT_BUCKETS = 200>
	class HashMap
	{
	public:
		HashMap();
		HashMap(const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>& hmSource);

		~HashMap();

		void Insert(const TKEY &tKey,const TVALUE &tValue);

		bool HasItem(const TKEY &tKey) const;

		bool Erase(const TKEY &tKey);
		void Clear();

		HashItem<TKEY,TVALUE> *Get(const TKEY &tKey) const;
		HashItem<TKEY,TVALUE> *GetBucketItem(uint32 iBucketNumber) const;

		uint32 Size() const;
		uint32 BucketSize() const;

		const HashMap& operator=(const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS> &hmSource);

	private:
		uint32 _Hash(const TKEY &tKey) const;
		void _CopyFrom(const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>& hmSource);

	private: 
		uint32 m_iSize;
		HashItem<TKEY,TVALUE> *m_arrBuckets[AMOUNT_BUCKETS];
	};

	#include "../src/K15_HashMap.inl"
}

#endif //__K15_HASHMAP__