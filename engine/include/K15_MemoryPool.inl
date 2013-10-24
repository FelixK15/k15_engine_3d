/**
 * @file K15_MemoryPool.cpp
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

template<class T,U32 CAPACITY> const U32 MemoryPool<T,CAPACITY>::ms_iSizePerMemoryNode = sizeof(T) + sizeof(MemoryPoolNode);

template<class T,U32 CAPACITY>
MemoryPool<T,CAPACITY>::MemoryPool()
	: m_iSize(ms_iSizePerMemoryNode * CAPACITY)
{
	m_iUsedMemory = 0;
	m_pMemory = K15_NEW byte[m_iSize+1];
	m_pFreeNode = (MemoryPoolNode*)m_pMemory;
	_CreateNodes();
}

template<class T,U32 CAPACITY>
MemoryPool<T,CAPACITY>::~MemoryPool()
{
	K15_DELETE[] m_pMemory;
}

template <class T,U32 CAPACITY>
T* MemoryPool<T,CAPACITY>::Allocate()
{
	assert(m_pFreeNode);
	MemoryPoolNode *pFreeNode = m_pFreeNode;
	m_pFreeNode = m_pFreeNode->Next;

	m_iUsedMemory += ms_iSizePerMemoryNode;

	return (T*)++pFreeNode;
}

template <class T,U32 CAPACITY>
void MemoryPool<T,CAPACITY>::Free(void *pPointer)
{
	assert(pPointer);
	assert(pPointer > m_pMemory && pPointer < m_pMemory + m_iSize);

	MemoryPoolNode *pMemoryNode = (MemoryPoolNode*)pPointer;
	pMemoryNode -= 1;

	pMemoryNode->Next = m_pFreeNode;
	m_pFreeNode = pMemoryNode;

	m_iUsedMemory -= ms_iSizePerMemoryNode;
}

template <class T,U32 CAPACITY>
void MemoryPool<T,CAPACITY>::Clear()
{
	Memory::MemSet(m_pMemory,0,m_iSize);
	m_iUsedMemory = 0;
}

template <class T,U32 CAPACITY>
U32 MemoryPool<T,CAPACITY>::GetUsedMemory() const
{
	return m_iUsedMemory;
}

template <class T,U32 CAPACITY>
U32 MemoryPool<T,CAPACITY>::GetFreeMemory() const
{
	return m_iSize - m_iUsedMemory;
}

template <class T,U32 CAPACITY>
U32 MemoryPool<T,CAPACITY>::GetTotalMemory() const
{
	return m_iSize;
}

template <class T,U32 CAPACITY>
void MemoryPool<T,CAPACITY>::_CreateNodes()
{
	U32 iCounter = 0;
	MemoryPoolNode *pNode = m_pFreeNode;
	byte *pByteNode = NULL;

	while((iCounter + ms_iSizePerMemoryNode) < m_iSize){
		pByteNode = (byte*)pNode;
		//pNode->Pointer = m_pMemory + iCounter;
		pNode->Next = (MemoryPoolNode*)(pByteNode + ms_iSizePerMemoryNode);
		pNode = pNode->Next;
		iCounter += ms_iSizePerMemoryNode;
	}

	pNode->Next = NULL;
}