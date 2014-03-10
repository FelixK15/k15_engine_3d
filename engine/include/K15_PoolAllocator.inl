/**
 * @file K15_PoolAllocator.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/12
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

/*********************************************************************************/
template<class Type>
PoolAllocator<Type>::PoolAllocator(uint32 p_ObjectCount, const String& p_Name, BaseAllocator* p_BaseAllocator)
	: BasePoolAllocator<sizeof(Type)>(p_ObjectCount,p_Name,p_BaseAllocator)
{

}
/*********************************************************************************/
template<class Type>
PoolAllocator<Type>::~PoolAllocator()
{

}
/*********************************************************************************/

/*********************************************************************************/
template<size_t TypeSize>
BasePoolAllocator<TypeSize>::BasePoolAllocator(uint32 p_ObjectCount, const String& p_Name, BaseAllocator* p_BaseAllocator)
	: BaseAllocator(p_ObjectCount * TypeSize,p_Name,p_BaseAllocator),
  m_Position(0)
{

}
/*********************************************************************************/
template<size_t TypeSize>
BasePoolAllocator<TypeSize>::~BasePoolAllocator()
{

}
/*********************************************************************************/
template<size_t TypeSize>
void* BasePoolAllocator<TypeSize>::alloc(size_t p_Size)
{
	K15_ASSERT(p_Size % TypeSize == 0,StringUtil::format("PoolAllocator %s Size unequal to object size. size:%i objectsize:%u",m_Name.c_str(),p_Size,TypeSize));

	if(m_Position * TypeSize == m_MemorySize)
	{
		m_Position = 0;
	}

	uint32 pos = m_Position++ * TypeSize;

	return (void*)(m_Memory + pos);
}
/*********************************************************************************/
template<size_t TypeSize>
void BasePoolAllocator<TypeSize>::dealloc(void* p_Pointer, size_t p_Size)
{
	memset(p_Pointer,0,TypeSize);
}
/*********************************************************************************/