/**
 * @file K15_MemoryHeap.inl
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

template <class T,size_t OBJ_COUNT_PER_HEAP> size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::ms_iSizePerHeap = OBJ_COUNT_PER_HEAP * (sizeof(T) + MEMORY_OFFSET);
template <class T,size_t OBJ_COUNT_PER_HEAP> size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::ms_iUsedMemory = 0;
template <class T,size_t OBJ_COUNT_PER_HEAP> const size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::MEMORY_OFFSET = sizeof(U16);	
template <class T,size_t OBJ_COUNT_PER_HEAP> HeapArray MemoryHeap<T,OBJ_COUNT_PER_HEAP>::ms_arrHeaps;

template <class T,size_t OBJ_COUNT_PER_HEAP>
T* MemoryHeap<T,OBJ_COUNT_PER_HEAP>::Allocate()
{
	return Allocate(1);
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
T* MemoryHeap<T,OBJ_COUNT_PER_HEAP>::Allocate(size_t iNumberObjects)
{
	/*
	* @todo Check if size of memory that is going to get allocated is
	* greater than the size of one heap and if numObjects == 0;
	*/
	T* pNewObject = NULL;
	size_t iSize = (sizeof(T) + MEMORY_OFFSET) * iNumberObjects;
	HeapID iHeapID = 0;

	//Look at each Heap to check if we find a Heap with enough space in it.
	for(iHeapID = 0;iHeapID < ms_arrHeaps.Size();++iHeapID){
		Heap &hpCurrentHeap = ms_arrHeaps.Get(iHeapID);
		if(hpCurrentHeap.GetFreeMemory() >= iSize){
			pNewObject = _AllocateOnHeap(iSize,iHeapID);
		}
	}

	//If no Heap has been found, create a new one.
	if(pNewObject == NULL){
		Heap hpNewHeap(ms_iSizePerHeap);
		ms_arrHeaps.PushBack(hpNewHeap);
		pNewObject = _AllocateOnHeap(iSize,iHeapID);
	}

	ms_iUsedMemory += iSize;

	return pNewObject;
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
void MemoryHeap<T,OBJ_COUNT_PER_HEAP>::Free(void* pPointer)
{
	size_t iSize = 0;

	size_t iPointerAddress = (size_t)pPointer;
	iPointerAddress -=  MEMORY_OFFSET;

	U16* pPointerHeapID = (U16*)iPointerAddress;
	HeapID iHeapID = *pPointerHeapID;

	pPointer = (void*)iPointerAddress;

	Heap &hpHeap = ms_arrHeaps.Get((U32)iHeapID);
	hpHeap.Free(pPointer,iSize);
	ms_iUsedMemory -= iSize;
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
void MemoryHeap<T,OBJ_COUNT_PER_HEAP>::Clear()
{
	ms_arrHeaps.Clear();
	ms_iUsedMemory = 0;
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::GetFreeMemory()
{
	return GetAmountMemory() - ms_iUsedMemory;
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::GetUsedMemory()
{
	return ms_iUsedMemory;
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::GetAmountMemory()
{
	return ms_iSizePerHeap;
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::GetAmountHeaps()
{
	return ms_arrHeaps.Size();
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
size_t MemoryHeap<T,OBJ_COUNT_PER_HEAP>::GetSizePerHeap()
{
	return GetSizePerHeap() * GetAmountHeaps();
}

template <class T,size_t OBJ_COUNT_PER_HEAP>
T* MemoryHeap<T,OBJ_COUNT_PER_HEAP>::_AllocateOnHeap(size_t iSize,U16 iHeapID)
{
	Heap &hpHeap = ms_arrHeaps.Get(iHeapID);
	void *pPointer = hpHeap.Allocate(iSize);

	U16* pIDPointer = (U16*)pPointer;
	*pIDPointer = iHeapID;

	T* pNewObject = (T*)((byte*)pPointer + MEMORY_OFFSET);

	return pNewObject;
}
