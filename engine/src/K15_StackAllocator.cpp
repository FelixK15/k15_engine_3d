/**
 * @file K15_StackAllocator.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/12
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

#include "K15_StackAllocator.h"
#include "K15_MemoryHeader.h"

namespace K15_Engine { namespace System { namespace Memory {

  StackAllocator::StackAllocator(byte* pMemory,size_t iSize)
    : m_pMemory(pMemory),
      m_iMemorySize(iSize),
      m_iUsedMemory(0)
  {

  }

  StackAllocator::StackAllocator(size_t iSize)
    : m_pMemory(malloc(iSize)),
      m_iMemorySize(iSize),
      m_iUsedMemory(0)
  {

  }

  void* StackAllocator::allocate(size_t iSize)
  {
    iSize += sizeof(MemoryHeader);
    K15_ASSERT(iSize + m_iUsedMemory > m_iMemorySize,"Cannot satisfy memory request.");

    //each allocation creates a memory header in which information gets stored
    MemoryHeader* pMemoryHeader = (MemoryHeader*)m_pMemory + m_iUsedMemory;
    memset(pMemoryHeader,0,iSize);
    pMemoryHeader->Size = iSize;

    //increment used memory size
    m_iUsedMemory += iSize;

    //shift and return memory
    return (void*)pMemoryHeader + 1; 
  }

  void* StackAllocator::allocateDebug(size_t iSize,const char* pFile,int iLine,bool bArray,const char* pFunction)
  {
    iSize += sizeof(MemoryHeader);
    K15_ASSERT(iSize + m_iUsedMemory > m_iMemorySize,"Cannot satisfy memory request.");

    //each allocation creates a memory header in which information gets stored
    MemoryHeader* pMemoryHeader = (MemoryHeader*)m_pMemory + m_iUsedMemory;
    memset(pMemoryHeader,0,iSize);
    pMemoryHeader->Size     = iSize;
    pMemoryHeader->File     = pFile;
    pMemoryHeader->Function = pFunction;
    pMemoryHeader->Line     = iLine;
    pMemoryHeader->IsArray  = bArray;

    //increment used memory size
    m_iUsedMemory += iSize;

    //shift and return memory
    return (void*)pMemoryHeader + 1; 
  }

  void StackAllocator::deallocate(void* pPointer)
  {
    if(pPointer)
    {
      //Get the memory header of this pointer
      MemoryHeader* pMemoryHeader = ((MemoryHeader*)pPointer) - 1;

      //Decrease amount of used memory.
      m_iUsedMemory -= pMemoryHeader->Size;

      return;
    }
  }

  void StackAllocator::deallocateDebug(void* pPointer,const char* pFile,int iLine,bool bArray,const char* pFunction)
  {
    if(pPointer)
    {
      //Get the memory header of this pointer
      MemoryHeader* pMemoryHeader = ((MemoryHeader*)pPointer) - 1;

      K15_ASSERT(bArray != pMemoryHeader->IsArray,"Tried to deallocate using the wrong delete operator.");

      //Decrease amount of used memory.
      m_iUsedMemory -= pMemoryHeader->Size;

      return;
    }
  }
}}}