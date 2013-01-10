/**
 * @file K15_HashMap.cpp
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
 */

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::HashMap()
{
	for(int i = 0;i < AMOUNT_BUCKETS;++i){
		m_arrBuckets[i] = NULL;
	}

	m_iSize = 0;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::HashMap(const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>& hmSource)
{
	_CopyFrom(hmSource);
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::~HashMap()
{
	Clear();
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
void HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::Insert(const TKEY &tKey,const TVALUE &tValue)
{
	U32 iBucketNo = _Hash(tKey);

	if(m_arrBuckets[iBucketNo] == NULL){
		m_arrBuckets[iBucketNo] = new HashItem<TKEY,TVALUE>(tKey,tValue,NULL);
	}else{
		m_arrBuckets[iBucketNo] = new HashItem<TKEY,TVALUE>(tKey,tValue,m_arrBuckets[iBucketNo]);
	}

	++m_iSize;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
bool HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::HasItem(const TKEY &tKey) const
{
	U32 iBucketNo = _Hash(tKey);

	return m_arrBuckets[iBucketNo] != NULL;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
bool HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::Erase(const TKEY &tKey)
{
	U32 iBucketNo = _Hash(tKey);

	if(m_arrBuckets[iBucketNo] == NULL){
		return false;
	}

	HashItem<TKEY,TVALUE> *pPrevious = NULL;
	for(HashItem<TKEY,TVALUE> *pItem = m_arrBuckets[iBucketNo];pItem;pItem = pItem->m_pNext){
		if(pItem->m_tKey == tKey){
			if(pPrevious){
				pPrevious->m_pNext = pItem->m_pNext;
			}

			m_arrBuckets[iBucketNo] = pItem->m_pNext;

			delete pItem;
			--m_iSize;

			return true;
		}

		pPrevious = pItem;
	}

	return false;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
void HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::Clear()
{
	for(U32 i = 0;i < AMOUNT_BUCKETS;++i){
		if(m_arrBuckets[i]){
			for(HashItem<TKEY,TVALUE> *pItem = m_arrBuckets[i];pItem;){
				HashItem<TKEY,TVALUE> *pNext = pItem->m_pNext;
				delete pItem;
				pItem = pNext;
			}
		}
	}

	m_iSize = 0;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
HashItem<TKEY,TVALUE> *HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::Get(const TKEY &tKey) const
{
	U32 iBucketNo = _Hash(tKey);
	HashItem<TKEY,TVALUE>* pDesiredItem = NULL;

	for(HashItem<TKEY,TVALUE> *pItem = m_arrBuckets[iBucketNo];pItem;pItem = pItem->m_pNext){
		if(pItem->m_tKey == tKey){
			pDesiredItem = pItem;
		}
	}

	return pDesiredItem;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
HashItem<TKEY,TVALUE> *HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::GetBucketItem(U32 iBucketNo) const
{
	return m_arrBuckets[iBucketNo];
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
U32 HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::Size() const
{
	return m_iSize;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
U32 HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::BucketSize() const
{
	return AMOUNT_BUCKETS;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>& HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::operator=(const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>& hmSource)
{
	_CopyFrom(hmSource);
	return *this;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
U32 HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>::_Hash(const TKEY &tKey) const
{
	int i = 0;

	char arrData[sizeof(int)];
	Memory::MemCopy(&arrData,&tKey,sizeof(int));
	unsigned long iHash = 0;
	int iCounter;

	while (iCounter = arrData[i++])
		iHash = iCounter + (iHash << 6) + (iHash << 16) - iHash;

	return iHash % AMOUNT_BUCKETS;
}

template <class TKEY,class TVALUE,U32 AMOUNT_BUCKETS>
void HashMap<TKEY, TVALUE, AMOUNT_BUCKETS>::_CopyFrom(const HashMap<TKEY,TVALUE,AMOUNT_BUCKETS>& hmSource)
{
	Clear();

	HashItem<TKEY,TVALUE> *pLastItem = NULL;
	for(U32 i = 0;i < AMOUNT_BUCKETS;++i){
		if(hmSource.m_arrBuckets[i] != NULL){
			HashItem<TKEY,TVALUE> *pCurItem = hmSource.m_arrBuckets[i];
			while(pCurItem){
				HashItem<TKEY,TVALUE> *pNewItem = new HashItem<TKEY,TVALUE>();
				pNewItem->m_tKey = pCurItem->m_tKey;
				pNewItem->m_tValue = pCurItem->m_tValue;
				pNewItem->m_pNext = pLastItem;

				if(!pLastItem){
					m_arrBuckets[i] = pNewItem;
				}

				pLastItem = pNewItem;
				pCurItem = pCurItem->m_pNext;
			}
		}
	}

	m_iSize = hmSource.m_iSize;
}

