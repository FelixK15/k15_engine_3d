/**
 * @file K15_ListNode.cpp
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

template<class T> MemoryPool<ListNode<T>,50000> ListNode<T>::ms_mpMemoryPool;

template<class T>
ListNode<T>::ListNode()
{
	m_pElement = 0;
	m_pNext = 0;
	m_pPrevious = 0;
}

template<class T>
ListNode<T>::~ListNode()
{
	//Delete the pointer to the element if there ever was an element.
	if(m_pElement){
		delete m_pElement;
	}
}

template<class T>
T& ListNode<T>::GetElement()
{
	return (*m_pElement);
}

template<class T>
ListNode<T> *ListNode<T>::GetNext()
{
	return m_pNext;
}

template<class T>
ListNode<T> *ListNode<T>::GetPrev()
{
	return m_pPrevious;
}

template<class T>
void *ListNode<T>::operator new(uint32 iSize)
{
	return ms_mpMemoryPool.Allocate();
}

template<class T>
void ListNode<T>::operator delete(void *pPointer)
{
	ms_mpMemoryPool.Free(pPointer);
}