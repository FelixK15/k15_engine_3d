/**
 * @file K15_StackAllocator.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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
template<uint16 PageSize,uint32 TotalSize> const uint32 PageAllocator<PageSize,TotalSize>::PageCount = TotalSize / PageSize;
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
PageAllocator<PageSize,TotalSize>::PageAllocator(const ObjectName& p_Name)
  : BaseAllocator(TotalSize - (TotalSize % PageSize),p_Name),
	  m_PageSize(PageSize),
	  m_CurrentPageIndex(0)
{
	K15_ASSERT(m_MemorySize >= m_PageSize,StringUtil::format("PageAllocator \"%s\" total size is smaller than the size of 1 page.",m_Name.c_str()));
	createPages();
}
//---------------------------------------------------------------------------//
template<uint16 PageSize,uint32 TotalSize>
PageAllocator<PageSize,TotalSize>::PageAllocator(BaseAllocator* p_Allocator,const ObjectName& p_Name)
  : BaseAllocator(p_Allocator,TotalSize - (TotalSize % PageSize),p_Name),
	  m_PageSize(PageSize),
	  m_CurrentPageIndex(0)
{
	K15_ASSERT(m_MemorySize >= m_PageSize,StringUtil::format("PageAllocator \"%s\" total size is smaller than the size of 1 page.",m_Name.c_str()));
	createPages();
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void* PageAllocator<PageSize,TotalSize>::alloc(uint32 p_Size)
{
	uint16 pagesNeeded = 1;

	if(p_Size > PageSize)
	{
		pagesNeeded = p_Size / m_PageSize;
	}

	if(pagesNeeded > PageCount - m_CurrentPageIndex)
	{
		m_CurrentPageIndex = 0;
	}
	else if(m_CurrentPageIndex == PageCount)
	{
		m_CurrentPageIndex = 0;
	}

	while(m_PageData[m_CurrentPageIndex].Size < p_Size) ++m_CurrentPageIndex;

	return memory;
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void PageAllocator<PageSize,TotalSize>::free(void* p_Pointer)
{
	K15_ASSERT((size_t)p_Pointer >= (size_t)m_Memory && (size_t)p_Pointer < ((size_t)m_Memory + m_MemorySize),
		StringUtil::format("Trying to free pointer which is not part of the PageAllocator \"%s\".",m_Name.c_str()));

	uint32 pageIndex = 0;
	ptrdiff_t ptrOffset = (size_t)p_Pointer - (size_t)m_Memory;

	pageIndex = ptrOffset / m_PageSize;
	if(pageIndex != 0) pageIndex -= 1;
	
	K15_ASSERT(pageIndex <= PageCount,StringUtil::format("pageIndex out of bounds PageAllocator \"%s\".",m_Name.c_str()))
	
	while(m_PageData[pageIndex++].Used)
	{
		m_PageData[pageIndex].Size = 0;
		m_PageData[pageIndex].Used = false;
	}
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void PageAllocator<PageSize,TotalSize>::createPages()
{
	byte* memory = (byte*)m_Memory;
	uint32 offset = 0;
	for(uint32 i = 0;i < PageCount;++i)
	{
		m_PageData[i].Used = false;
		m_PageData[i].Size = 0;
		m_PageData[i].Memory = m_Memory + offset;
	
		offset += m_PageSize;
	}

	m_PageData[0].Size = m_MemorySize;
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void K15_Engine::Core::PageAllocator<PageSize, TotalSize>::clear()
{
  createPages();
}
/*********************************************************************************/