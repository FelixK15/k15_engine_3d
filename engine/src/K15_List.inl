/**
 * @file K15_List.inl
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

template<class T>
List<T>::List()
{
	m_pFirst = NULL;
	m_pLast = NULL;
	m_iSize = 0;
}

template<class T>
List<T>::~List()
{
	Clear();
}

template<class T>
void List<T>::PushBack( const T& tElement )
{
	//Create a new node with the element as content.
	ListNode<T> *pNewNode = _CreateNewNode(tElement);
	pNewNode->m_pPrevious = m_pLast;

	//Check what pointers we need to 'realign' in order to ensure a positive order.
	if(m_pLast){
		m_pLast->m_pNext = pNewNode;
	}

	if(!m_pFirst){
		m_pFirst = pNewNode;
	}

	m_pLast = pNewNode;

	++m_iSize;
}

template<class T>
void List<T>::PushFront( const T& tElement )
{
	//Create a new node with the element as content.
	ListNode<T> *pNewNode = _CreateNewNode(tElement);
	pNewNode->m_pNext = m_pFirst;

	//Check what pointers we need to 'realign' in order to ensure a positive order.
	if(m_pFirst){
		m_pFirst->m_pPrevious = pNewNode;
	}

	if(!m_pLast){
		m_pLast = pNewNode;
	}

	m_pFirst = pNewNode;

	++m_iSize;
}

template<class T>
void List<T>::PopBack()
{
	assert(Size() > 0);

	if(Size() > 0){
		ListNode<T> *pNewTail = m_pLast->GetPrev();

		if(pNewTail){
			pNewTail->m_pNext = NULL;
		}

		K15_DELETE m_pLast;

		m_pLast = pNewTail;

		--m_iSize;

		_CheckForOphrandTailAndHead();
	}
}

template<class T>
void List<T>::PopFront()
{
	assert(Size() > 0);

	if(Size() > 0){
		ListNode<T> *pNewHead = m_pFirst->GetNext();

		if(pNewHead){
			pNewHead->m_pPrevious = NULL;
		}

		K15_DELETE m_pFirst;

		m_pFirst = pNewHead;

		--m_iSize;

		_CheckForOphrandTailAndHead();
	}
}

template<class T>
void List<T>::Clear()
{
	//Every node will get deleted.
	U32 iCounter = 0;
	ListNode<T> *pNextNode = NULL;
	for(ListNode<T> *pNode = m_pFirst;pNode;){
		pNextNode = pNode->m_pNext;
		K15_DELETE pNode;

		pNode = pNextNode;

		++iCounter;
	}
	
	m_pFirst = m_pLast = NULL;
	m_iSize = 0;
}

template<class T>
void List<T>::Erase(ListNode<T> *pNodeToErase)
{
	if(pNodeToErase == m_pFirst){
		PopFront();
	}else if(pNodeToErase == m_pLast){
		PopBack();
	}else{
		pNodeToErase->m_pNext->m_pPrevious = pNodeToErase->GetPrev();
		pNodeToErase->m_pPrevious->m_pNext = pNodeToErase->GetNext();

		K15_DELETE pNodeToErase;
		pNodeToErase = NULL;
		--m_iSize;
	}
}

template<class T>
U32 List<T>::Size() const
{
	return m_iSize;
}

template<class T>
ListNode<T> *List<T>::GetFirstElement() const
{
	return m_pFirst;
}

template<class T>
ListNode<T> *List<T>::GetLastElement() const
{
	return m_pLast;
}

template<class T>
void List<T>::_CheckForOphrandTailAndHead()
{
	//Function is used internally to check if m_pFirst and m_pLast are still valid pointers.
	if(Size() == 0){
		if(m_pFirst){
			m_pFirst = NULL;
		}else if(m_pLast){
			m_pLast = NULL;
		}
	}
}

template<class T>
ListNode<T> *List<T>::_CreateNewNode(const T& tElement)
{
	//Create a new node and a new object of class T.
	//Copy to data from element (parameter) into the new object of class T.
	ListNode<T> *pNewNode = new ListNode<T>();
	pNewNode->m_pElement = K15_NEW T(tElement);

	return pNewNode;
}