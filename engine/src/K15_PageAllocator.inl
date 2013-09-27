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
PageAllocator<PageSize,TotalSize>::PageAllocator()
	: m_MemorySize(TotalSize),
	  m_UsedMemory(0),
	  m_PageSize(PageSize),
	  m_CurrentPageIndex(0)
{
#if defined (K15_DEBUG)
	m_MemorySize += sizeof(MemoryHeader) * PageCount;
	m_PageSize += sizeof(MemoryHeader);
#endif //K15_DEBUG

	K15_ASSERT(m_MemorySize < m_PageSize,"PageAllocator total size is smaller than the size of 1 page.");

	m_MemorySize -= m_MemorySize % m_PageSize;
	m_Memory = (byte*)malloc(m_MemorySize);

	createPages();
}

template<uint16 PageSize,uint32 TotalSize>
template<class Allocator>
PageAllocator<PageSize,TotalSize>::PageAllocator(Allocator* p_Allocator)
	: m_MemorySize(TotalSize),
	  m_UsedMemory(0),
	  m_PageSize(PageSize),
	  m_CurrentPageIndex(0)
{
#if defined (K15_DEBUG)
	m_MemorySize += sizeof(MemoryHeader) * PageCount;
	m_PageSize += sizeof(MemoryHeader);
#endif //K15_DEBUG

	K15_ASSERT(p_Allocator == 0,"Custom allocator for PageAllocator is NULL.");
	K15_ASSERT(m_MemorySize < m_PageSize,"PageAllocator total size is smaller than the size of 1 page.");

	m_MemorySize -= m_MemorySize % m_PageSize;
	m_Memory = (byte*)p_Allocator->allocate(m_MemorySize);

	createPages();
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
PageAllocator<PageSize,TotalSize>::PageAllocator(byte* p_Memory)
	: m_MemorySize(TotalSize),
	  m_UsedMemory(0),
	  m_PageSize(PageSize),
	  m_CurrentPageIndex(0)
{
#if defined (K15_DEBUG)
	m_MemorySize += sizeof(MemoryHeader) * PageCount;
	m_PageSize += sizeof(MemoryHeader);
#endif //K15_DEBUG

	K15_ASSERT(p_Memory == 0,"pre-allocated memory for PageAllocator is NULL.");
	K15_ASSERT(m_MemorySize < m_PageSize,"PageAllocator total size is smaller than the size of 1 page.");

	m_MemorySize -= m_MemorySize % m_PageSize;
	m_Memory = p_Memory;

	createPages();
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void* PageAllocator<PageSize,TotalSize>::allocate(uint32 p_Size)
{
	uint16 pagesNeeded = 1;

	if(p_Size > PageSize)
	{
		pagesNeeded = p_Size / m_PageSize;
	}

	K15_ASSERT((pagesNeeded + m_CurrentPageIndex) > PageCount,"Not enough pages left to satisfy memory request");

	byte* memory = m_PageData[m_CurrentPageIndex].Memory;
	m_PageData[m_CurrentPageIndex].Size = p_Size;

	for(uint32 i = m_CurrentPageIndex;i < m_CurrentPageIndex + pagesNeeded;++i)
	{
		m_PageData[i].Used = true;
		m_PageData[i].Size = 0;
	}

	m_CurrentPageIndex += pagesNeeded;

	return memory;
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void* PageAllocator<PageSize,TotalSize>::allocateDebug(uint32 p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function)
{
	uint16 pagesNeeded = 1;

	if(p_Size > PageSize)
	{
		pagesNeeded = p_Size / m_PageSize;
	}

	K15_ASSERT((pagesNeeded + m_CurrentPageIndex) > PageCount,"Not enough pages left to satisfy memory request");

	byte* memory = m_PageData[m_CurrentPageIndex].Memory;
	m_PageData[m_CurrentPageIndex].Size = p_Size;

	MemoryHeader* header = (MemoryHeader*)memory;
	header->File = p_File;
	header->Function = p_Function;
	header->IsArray = p_Array;
	header->Line = p_Line;
	header->Size = p_Size;

	memory += sizeof(MemoryHeader);

	for(uint32 i = m_CurrentPageIndex;i < m_CurrentPageIndex + pagesNeeded;++i)
	{
		m_PageData[i].Used = true;
		m_PageData[i].Size = 0;
	}

	m_CurrentPageIndex += pagesNeeded;

	return memory;
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void PageAllocator<PageSize,TotalSize>::deallocate(void* p_Pointer)
{
	K15_ASSERT((size_t)p_Pointer > (size_t)m_Memory && (size_t)p_Pointer < ((size_t)m_Memory + m_MemorySize),"Trying to free pointer, which is not part of the PageAllocator");

	uint32 pageIndex = 0;
	ptrdiff_t ptrOffset = (size_t)m_Memory - (size_t)p_Pointer;

	pageIndex = ptrOffset / m_PageSize;
	pageIndex -= 1;

	K15_ASSERT(pageIndex > PageCount,"pageIndex out of bounds")
	K15_ASSERT(m_PageData[pageIndex].Size == 0,"Error calculating pageIndex.");

	m_PageData[pageIndex].Used = false;
	m_PageData[pageIndex].Size = 0;

	while(m_PageData[++pageIndex].Size == 0)
	{
		m_PageData[pageIndex].Used = false;
	}
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void PageAllocator<PageSize,TotalSize>::deallocateDebug(void* p_Pointer,const char* p_File,int p_Line,bool p_Array,const char* p_Function)
{
	K15_ASSERT((size_t)p_Pointer > (size_t)m_Memory && (size_t)p_Pointer < ((size_t)m_Memory + m_MemorySize),"Trying to free pointer, which is not part of the PageAllocator");

	uint32 pageIndex = 0;
	ptrdiff_t ptrOffset = (size_t)m_Memory - (size_t)p_Pointer;

	pageIndex = ptrOffset / m_PageSize;
	pageIndex -= 1;

	K15_ASSERT(pageIndex > PageCount,"pageIndex out of bounds")
	K15_ASSERT(m_PageData[pageIndex].Size == 0,"Error calculating pageIndex.");

	MemoryHeader* header = (MemoryHeader*)((size_t)p_Pointer - sizeof(MemoryHeader));

	K15_ASSERT(p_Array != header->IsArray,"Tried to deallocate using the wrong delete operator.");

	m_PageData[pageIndex].Used = false;
	m_PageData[pageIndex].Size = 0;

	while(m_PageData[++pageIndex].Size == 0)
	{
		m_PageData[pageIndex].Used = false;
	}
}
/*********************************************************************************/
template<uint16 PageSize,uint32 TotalSize>
void PageAllocator<PageSize,TotalSize>::createPages()
{
	byte* memory = (byte*)m_Memory;
	uint32 offset = 0;
	for(int i = 0;i < PageCount;++i)
	{
		m_PageData[i].Used = false;
		m_PageData[i].Size = 0;
		memcpy(m_PageData[i].Memory,(byte*)memory + offset,m_PageSize);

		offset += m_PageSize;
	}
}
/*********************************************************************************/