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
template<class T>
PoolAllocator<T>::PoolAllocator(uint32 p_ObjectCount, const ObjectName& p_Name)
	: BaseAllocator(p_ObjectCount * sizeof(T),p_Name)
{

}
/*********************************************************************************/
template<class T>
PoolAllocator<T>::PoolAllocator(BaseAllocator* p_Allocator,uint32 p_ObjectCount, const ObjectName& p_Name)
	: BaseAllocator(p_Allocator,p_ObjectCount * sizeof(T),p_Name)
{

}
/*********************************************************************************/
template<class T>
PoolAllocator<T>::~PoolAllocator()
{

}
/*********************************************************************************/
template<class T>
void* PoolAllocator<T>::alloc(uint32 p_Size)
{
	K15_ASSERT(p_Size % sizeof(T) == 0,StringUtil::format("PoolAllocator %s Size unequal to object size. size:%i objectsize:%u",m_Name.c_str(),p_Size,sizeof(T)));

	if(m_Position * sizeof(T) == m_MemorySize)
	{
		m_Position = 0;
	}

	uint32 pos = m_Position++ * sizeof(T);
	while(m_Memory[pos] & sizeof(T) > 0)
	{
		pos = m_Position++ * sizeof(T);
		if(pos == m_MemorySize)
		{
			m_Position = 0;
			pos = 0;
		}
	}

	return (void*)m_Memory[pos];
}
/*********************************************************************************/
template<class T>
void PoolAllocator<T>::free(void* p_Pointer)
{
	memset(p_Pointer,0,sizeof(T));
}
/*********************************************************************************/