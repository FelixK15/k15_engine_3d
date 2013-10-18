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

namespace K15_Engine { namespace System {
  /*********************************************************************************/
  BaseAllocator::BaseAllocator()
    : m_Allocator(0),
      m_Memory(0),
      m_MemorySize(0),
      m_UsedMemory(0)
  {

  }
  /*********************************************************************************/
  BaseAllocator::BaseAllocator(uint32 p_Size)
    : m_Allocator(0),
      m_Memory(0),
      m_MemorySize(p_Size),
      m_UsedMemory(0)
  {
	m_Memory = (byte*)malloc(p_Size);
  }
  /*********************************************************************************/
  BaseAllocator::BaseAllocator(BaseAllocator* p_Allocator,uint32 p_Size)
    : m_Allocator(p_Allocator),
      m_Memory(0),
      m_MemorySize(p_Size),
      m_UsedMemory(0)
  {
    K15_ASSERT(p_Allocator != 0,"Custom allocator is NULL.");
#if defined (K15_DEBUG)
	m_Memory = (byte*)p_Allocator->allocateDebug(p_Size,__FILE__,__LINE__,false,__FUNCTION__);
#else
	m_Memory = (byte*)p_Allocator->allocate(p_Size);
#endif //K15_DEBUG
  }
  /*********************************************************************************/
  BaseAllocator::~BaseAllocator()
  {
    if(m_Allocator)
    {
#     if defined K15_DEBUG
        m_Allocator->deallocateDebug(m_Memory,__FILE__,__LINE__,false,__FUNCTION__);
#     else
        m_Allocator->deallocate(m_Memory);
#     endif //K15_DEBUG
    }
    else
    {
#     if defined K15_DEBUG
        deallocate(m_Memory);
#     else
        deallocateDebug(m_Memory,__FILE__,__LINE__,false,__FUNCTION__);
#     endif //K15_DEBUG
    }
  }
  /*********************************************************************************/
  void* BaseAllocator::allocate(uint32 p_Size)
  {
	  K15_ASSERT(p_Size + m_UsedMemory <= m_MemorySize,"Cannot satisfy memory request.");
	  void* memory = alloc(p_Size);
	  memset(memory,0,p_Size);
	  m_UsedMemory += p_Size;
	  return memory;
  }
  /*********************************************************************************/
  void BaseAllocator::deallocate(void* p_Pointer)
  {
	  return free(p_Pointer);
  }
  /*********************************************************************************/
  void* BaseAllocator::allocateDebug( uint32 p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function )
  {
   // p_Size += sizeof(MemoryHeader);
    K15_ASSERT(p_Size + m_UsedMemory <= m_MemorySize,"Cannot satisfy memory request.");
    byte* memory = (byte*)alloc(p_Size);
	memset(memory,0,p_Size);
// 	MemoryHeader* header = (MemoryHeader*)memory;
// 	header->File = p_File;
// 	header->Function = p_Function;
// 	header->IsArray = p_Array;
// 	header->Size = p_Size;
// 	header->Line = p_Line;

	m_UsedMemory += p_Size;

    //return (void*)(memory += sizeof(MemoryHeader)); //shift and return memory
	return (void*)memory;
  }
/*********************************************************************************/
}}//end of K15_Engine::System