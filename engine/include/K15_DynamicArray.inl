/**
 * @file K15_DynamicArray.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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
DynamicArray<T>::DynamicArray(uint32 iSize)
{
	assert(iSize > 0);

	m_iCapacity = iSize;
	m_iReallocateSize = iSize;

	m_iSizeT = sizeof(T);

	m_pArr = (T*)Memory::Allocate(iSize * m_iSizeT,__FILE__,__LINE__,true);

	m_iFirst = m_iLast = 0;
	m_iGrowthFactor = 2;
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &arrSource)
{
	_CopyFrom(arrSource);
}

template<class T>
DynamicArray<T>::~DynamicArray()
{
	Clear();
	Memory::Free(m_pArr,true);
}

template<class T>
void DynamicArray<T>::PushBack(const T &tObject)
{
	//pos == end of array.
	uint32 iPos = Size();

	if(iPos > (m_iCapacity - 1)){
		//If there's not enough space to hold a new object, more memory will get allocated using _Reallocate().
		_Reallocate();
	}

	size_t iAddressArr = (size_t)m_pArr;
	size_t iAddressPos = iAddressArr + (iPos * m_iSizeT);

	void *pPointer = (void*)iAddressPos;
	//Create a copy of obj using the copy constructor
	::new(pPointer) T(tObject);


	m_iLast = ++iPos;
}

template<class T>
void DynamicArray<T>::PushFront(const T &tObject)
{
	//Calculate the current size of the array in byte.
	uint32 iCurSizeInByte = m_iCapacity * m_iSizeT;
	uint32 iSize = Size() + 1;

	if(iSize > m_iCapacity){
		//If there's not enough space to hold a new object, more memory will get allocated using _Reallocate().
		_Reallocate();
	}

	m_iLast = m_iCapacity;

	//Calculate the memory addresses by adding offset to the address of the array.
	size_t iArrayStartPos = (size_t)m_pArr;
	size_t iNewPos = iArrayStartPos + m_iSizeT;

	void* pDestinationAddress = (void*)iArrayStartPos;
	void *pSourceAddress = (void*)iNewPos;

	//Shift memory so that m_pArr[0] is empty.
	Memory::MemMove(pDestinationAddress,pSourceAddress,iCurSizeInByte);

	//Create a copy of obj using the copy constructor
	::new((void*)m_pArr) T(tObject);
}

// template<class T>
// void DynamicArray<T>::PopFront()
// {
// 	if(Size() > 0){
// 		--m_iLast;
// 
// 		//Calculate the size of the array.
// 		size_t iSize = m_iLast * m_iSizeT;
// 
// 		//Get the address of the second element in the array.
// 		void* pSourceAddress = (void*)m_pArr + m_iSizeT;
// 
// 		//Shift memory so that the beginning of the array will get overridden by the rest
// 		//of the array.
// 		Memory::MemMove(m_pArr,pSourceAddress,iSize);
// 	}
// }

// template<class T>
// void DynamicArray<T>::PopBack()
// {
// 	if(Size() > 0){
// 		--m_iLast;
// 		_ZeroUnusedMemory();
// 	}
// }
// 
template<class T>
void DynamicArray<T>::Pop(uint32 iPos)
{
	assert(iPos <= Size());

	--m_iLast;
	m_pArr[iPos].~T();
	
	Memory::MemSet(&m_pArr[iPos],0,m_iSizeT);
}
// 
// template<class T>
// void DynamicArray<T>::Pop(U32 iStart,U32 iEnd)
// {
// 	assert((iEnd - iStart) > 0);
// 	
// 	U32 iSpan = iEnd - iStart;
// 			
// 	m_iLast -= iSpan;
// 
// 	//To a little hacky address calculation to get the block of memory that should get popped.
// 	size_t iStartArrayAddress = (U32)(m_pArr);
// 	size_t iEndArrayAddress = (m_iCapacity * m_iSizeT) + iStartArrayAddress;
// 	size_t iMemoryStartOffset = iStart * m_iSizeT;
// 	size_t iMemoryEndOffset = iEnd * m_iSizeT;
// 
// 	//Cast the calculated addresses back to 'real' address pointer.
// 	void *pDestinationAddress = (void*)(iStartArrayAddress + iMemoryStartOffset);
// 	void *pSourceAddress = (void*)(iStartArrayAddress + iMemoryEndOffset);
// 
// 	size_t iSize = iEndArrayAddress - (iMemoryEndOffset + iStartArrayAddress);
// 			
// 	//Memory will get shifted so that the span within the array will get overriden.
// 	Memory::MemMove(pDestinationAddress,pSourceAddress,iSize);
// }

template<class T>
void DynamicArray<T>::Clear()
{
	//Call the destructor of all objects.
	for(uint32 i = 0;i < Size();++i){
		m_pArr[i].~T();
	}

	Memory::MemSet((void*)m_pArr,0,Size() * m_iSizeT);
	m_iLast = m_iFirst = 0;
}

template<class T>
T& DynamicArray<T>::Get(uint32 iPos)
{
	assert(iPos < m_iCapacity);

	if(iPos > m_iCapacity){
		iPos = m_iLast;
	}

	return m_pArr[iPos];
}

template<class T>
const T& DynamicArray<T>::Get(uint32 iPos) const
{
	return Get(iPos);
}

template<class T>
uint32 DynamicArray<T>::Size() const
{
	return m_iLast - m_iFirst;
}

template<class T>
uint32 DynamicArray<T>::Capacity() const
{
	return m_iCapacity;
}

template<class T>
uint32 DynamicArray<T>::ReallocationSize() const
{
	return m_iReallocateSize;
}

template<class T>
bool DynamicArray<T>::HasItem( const T &tItem ) const
{
	bool bReturn = false;
	uint32 iSize = Size();
	for(uint32 i = 0;i < iSize;++i){
		if(m_pArr[i] == tItem){
			bReturn = true;
			break;
		}
	}

	return bReturn;
}

template<class T>
void DynamicArray<T>::Resize( uint32 iObjCount )
{
	if(iObjCount > Size()){
		uint32 iTotalSize = iObjCount * m_iSizeT;
		uint32 iOldReallocationSize = m_iReallocateSize;
		m_iReallocateSize = iTotalSize;
		_Reallocate();
		m_iReallocateSize = iOldReallocationSize;
	}
}

template<class T>
const DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T> &arrSource)
{
	_CopyFrom(arrSource);
	return *this;
}

template<class T>
T& DynamicArray<T>::operator[](uint32 iPos)
{
	return Get(iPos);
}

template<class T>
void DynamicArray<T>::_CopyFrom(const DynamicArray<T> &arrSource)
{
	if(m_pArr){
		K15_DELETE[] m_pArr;
	}
	
	m_pArr = (T*)Memory::Allocate(arrSource.Capacity() * arrSource.m_iSizeT,__FILE__,__LINE__,true);
	Memory::MemCopy(m_pArr,arrSource.m_pArr,Capacity() * m_iSizeT);
}

template<class T>
void DynamicArray<T>::_Reallocate()
{
	uint32 iOldSize = m_iCapacity * m_iSizeT;
	m_iCapacity += (m_iReallocateSize * m_iGrowthFactor);

	m_iGrowthFactor *= 2;

	T *pNewArr = (T*)Memory::Allocate(m_iCapacity * m_iSizeT,__FILE__,__LINE__,true);
	Memory::MemCopy(pNewArr,m_pArr,iOldSize);
	K15_DELETE[] m_pArr;

	m_pArr = pNewArr;
}

template<class T>
void DynamicArray<T>::_ZeroUnusedMemory()
{
	uint32 iSize = (m_iCapacity - Size()) * m_iSizeT;

	size_t iStartArrayAddress = (size_t)m_pArr;
	size_t iMemoryOffset = m_iCapacity * m_iSizeT;

	void *pAddress = (void*)(iStartArrayAddress + iMemoryOffset);

	Memory::MemSet(pAddress,0,iSize);
}