/**
 * @file K15_BaseAllocator.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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

#include "K15_PrecompiledHeader.h"

#include "K15_BaseAllocator.h"
#include "K15_MemoryHeader.h"

#if defined K15_DEBUG
#	include "K15_MemoryProfiler.h"
#endif //K15_DEBUG

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	BaseAllocator::BaseAllocator(size_t p_Size,const ObjectName& p_Name,BaseAllocator* p_BaseAllocator)
		: ThreadSafe(),
		m_Memory(0),
		m_MemorySize(p_Size),
		m_UsedMemory(0),
#if defined K15_DEBUG
		m_MemoryHeaderMap(),
#endif
		m_Name(p_Name),
		m_BaseAllocator(p_BaseAllocator)
	{
#		if defined K15_DEBUG
			//g_MemoryProfiler->addAllocator(this);
#		endif //K15_DEBUG
		
		if(p_BaseAllocator)
		{
			m_Memory = K15_NEW_SIZE(p_BaseAllocator,p_Size) byte;
		}
		else
		{
			m_Memory = (byte*)malloc(p_Size);
		}

		m_MemoryEndAddress = m_Memory + m_MemorySize;
	}
	/*********************************************************************************/
	BaseAllocator::~BaseAllocator()
	{
#		if defined K15_DEBUG
		//g_MemoryProfiler->removeAllocator(this);
#		endif // K15_DEBUG

		if(m_BaseAllocator)
		{
			K15_DELETE_SIZE(m_BaseAllocator,m_Memory,m_MemorySize);
		}
		else
		{
			free(m_Memory);
		}
		
		m_Memory = 0;

#if defined K15_DEBUG
		if(m_MemoryHeaderMap.size() > 0)
		{
			_LogWarning("Memory from allocator %s is still being used at destruction time. Dumping memory statistics.",m_Name.c_str());

			for(PointerMemoryHeaderMap::iterator iter = m_MemoryHeaderMap.begin();iter != m_MemoryHeaderMap.end();++iter)
			{
				_LogWarning("Address: %p. Size:%u byte Array:%s\n\tFile: %s(%u)\n\tFunction: %s\n\tFrom Allocator: %s",(void*)iter->first,iter->second->Size,
					iter->second->IsArray ? "true" : "false",iter->second->File,iter->second->Line,iter->second->Function,
					((BaseAllocator*)iter->second->Allocator)->getName().c_str());
			}
		}
#endif //K15_DEBUG
	}
	/*********************************************************************************/
	void* BaseAllocator::allocate(size_t p_Size)
	{
		lockMutex();

		K15_ASSERT(p_Size + m_UsedMemory <= m_MemorySize,StringUtil::format("Cannot satisfy memory request. Allocator:%s",m_Name.c_str()));
		void* memory = alloc(p_Size);
		memset(memory,0,p_Size);
		m_UsedMemory += p_Size;

		unlockMutex();
		return memory;
	}
	/*********************************************************************************/
	void BaseAllocator::deallocate(void* p_Pointer, size_t p_Size)
	{
		lockMutex();

		K15_ASSERT((ptrdiff_t)p_Pointer >= (ptrdiff_t)m_Memory && (ptrdiff_t)p_Pointer < (ptrdiff_t)m_MemoryEndAddress,
			StringUtil::format("Pointer %p is not part of Allocator:%s",m_Name.c_str()));

		K15_ASSERT((m_UsedMemory - p_Size) >= 0,StringUtil::format("Trying to release more memory than there is available. Allocator:%s",m_Name.c_str()));

		dealloc(p_Pointer, p_Size);

		m_UsedMemory -= p_Size;

		unlockMutex();
	}
#if defined K15_DEBUG
	/*********************************************************************************/
	void* BaseAllocator::allocateDebug(size_t p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function)
	{
		lockMutex();

		K15_ASSERT(p_Size + m_UsedMemory <= m_MemorySize,StringUtil::format("Cannot satisfy memory request. Allocator:%s",m_Name.c_str()));
		byte* memory = (byte*)alloc(p_Size);
		memset(memory,0,p_Size);

		//memory header will always get created on the heap
 	 	MemoryHeader* header = 0;
		K15_ASSERT((header = (MemoryHeader*)malloc(sizeof(MemoryHeader))) != 0,"Out of memory.");
		header->Allocator = (void*)this;
 	 	header->File = p_File;
 	 	header->Function = p_Function;
 	 	header->IsArray = p_Array;
 	 	header->Size = p_Size;
 	 	header->Line = p_Line;

		m_MemoryHeaderMap.insert(Pair(size_t,MemoryHeader*)((size_t)memory,header));

		m_UsedMemory += p_Size;


		unlockMutex();

		return (void*)memory;
	}
	/*********************************************************************************/
	void BaseAllocator::deallocateDebug(void* p_Pointer,size_t p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function)
	{
		lockMutex();

		K15_ASSERT((ptrdiff_t)p_Pointer >= (ptrdiff_t)m_Memory && (ptrdiff_t)p_Pointer < (ptrdiff_t)m_MemoryEndAddress,
		  StringUtil::format("Pointer %p is not part of Allocator:%s",m_Name.c_str()));

		K15_ASSERT((m_UsedMemory - p_Size) >= 0,StringUtil::format("Trying to release more memory than there is available. Allocator:%s",m_Name.c_str()));
    
		PointerMemoryHeaderMap::iterator iter = m_MemoryHeaderMap.find((size_t)p_Pointer);

		K15_ASSERT(iter != m_MemoryHeaderMap.end(),StringUtil::format("Pointer %p is not part of Allocator:%s",m_Name.c_str()));

		MemoryHeader* header = iter->second;
		K15_ASSERT(header->IsArray == p_Array,StringUtil::format(
		  "Trying to free memory that has been allocated via new[] with delete. Allocator:%s\n\tFile:%s(%i)\n\tFunction:%s",
		  m_Name.c_str(),p_File,p_Line,p_Function));

		K15_ASSERT(header->Allocator == this,StringUtil::format("Pointer %p has been allocated by allocator %s but tried to free via allocator %s.",
		  p_Pointer,((BaseAllocator*)header->Allocator)->getName().c_str(),m_Name.c_str()));

		free(header);

		m_MemoryHeaderMap.erase(iter);

		dealloc(p_Pointer, p_Size);

		m_UsedMemory -= p_Size;

		unlockMutex();
	}
#endif //K15_DEBUG
	/*********************************************************************************/
	const ObjectName& BaseAllocator::getName() const
	{
		return m_Name;
	}
	/*********************************************************************************/
	void BaseAllocator::clear()
	{
		m_UsedMemory = 0;
	}
	/*********************************************************************************/
	uint32 BaseAllocator::getSize() const
	{
		return m_MemorySize;
	}
	/*********************************************************************************/
	uint32 BaseAllocator::getFreeSize() const
	{
		return m_MemorySize - m_UsedMemory;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core