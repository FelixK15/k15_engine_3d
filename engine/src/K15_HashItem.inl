/**
 * @file K15_HashItem.inl
 * @author  Felix Klinge <f.klinge15@gmail.com>
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

template<class TKEY,class TVALUE>
HashItem<TKEY,TVALUE>::HashItem()
{
	this->m_pNext = NULL;
}

template<class TKEY,class TVALUE>
HashItem<TKEY,TVALUE>::HashItem(TKEY tKey,TVALUE tValue,HashItem *pNext)
{
	this->m_tKey = tKey;
	this->m_tValue = tValue;
	this->m_pNext = pNext;
}

template<class TKEY,class TVALUE>
TKEY &HashItem<TKEY,TVALUE>::GetKey()
{
	return m_tKey;
}

template<class TKEY,class TVALUE>
TVALUE &HashItem<TKEY,TVALUE>::GetValue()
{
	return m_tValue;
}

template<class TKEY,class TVALUE>
HashItem<TKEY,TVALUE> *HashItem<TKEY,TVALUE>::GetNext()
{
	return m_pNext;
}