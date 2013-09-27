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

namespace K15_Engine { namespace System {
  /*********************************************************************************/
  const TypeName StackAllocator::Type = _TN(StackAllocator);
  /*********************************************************************************/
  StackAllocator::StackAllocator(byte* p_Memory,uint32 p_Size)
    : m_Memory(p_Memory),
      m_MemorySize(p_Size),
      m_UsedMemory(0)
  {

  }
  /*********************************************************************************/
  StackAllocator::StackAllocator(uint32 p_Size)
    : m_Memory((byte*)malloc(p_Size)),
      m_MemorySize(p_Size),
      m_UsedMemory(0)
  {

  }
  /*********************************************************************************/
  void* StackAllocator::allocate(uint32 p_Size)
  {
    p_Size += sizeof(MemoryHeader);
    K15_ASSERT(p_Size + m_UsedMemory > m_MemorySize,"Cannot satisfy memory request.");

    //each allocation creates a memory header in which information gets stored
    MemoryHeader* pMemoryHeader = (MemoryHeader*)m_Memory + m_UsedMemory;
    memset(pMemoryHeader,0,p_Size);
    pMemoryHeader->Size = p_Size;

    //increment used memory size
    m_UsedMemory += p_Size;

    //shift and return memory
    return (void*)pMemoryHeader + 1; 
  }
  /*********************************************************************************/
  void* StackAllocator::allocateDebug(uint32 p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function)
  {
    p_Size += sizeof(MemoryHeader);
    K15_ASSERT(p_Size + m_UsedMemory > m_MemorySize,"Cannot satisfy memory request.");

    //each allocation creates a memory header in which information gets stored
    MemoryHeader* pMemoryHeader = (MemoryHeader*)m_Memory + m_UsedMemory;
    memset(pMemoryHeader,0,p_Size);
    pMemoryHeader->Size     = p_Size;
    pMemoryHeader->File     = p_File;
    pMemoryHeader->Function = p_Function;
    pMemoryHeader->Line     = p_Line;
    pMemoryHeader->IsArray  = p_Array;

    //increment used memory size
    m_UsedMemory += p_Size;

    //shift and return memory
    return (void*)pMemoryHeader + 1; 
  }
  /*********************************************************************************/
  void StackAllocator::deallocate(void* p_Pointer)
  {
    if(p_Pointer)
    {
      //Get the memory header of this pointer
      MemoryHeader* pMemoryHeader = ((MemoryHeader*)p_Pointer) - 1;

      //Decrease amount of used memory.
      m_UsedMemory -= pMemoryHeader->Size;
    }
  }
  /*********************************************************************************/
  void StackAllocator::deallocateDebug(void* p_Pointer,const char* p_File,int p_Line,bool p_Array,const char* p_Function)
  {
    if(p_Pointer)
    {
      //Get the memory header of this pointer
      MemoryHeader* pMemoryHeader = ((MemoryHeader*)p_Pointer) - 1;

      K15_ASSERT(p_Array != pMemoryHeader->IsArray,"Tried to deallocate using the wrong delete operator.");

      //Decrease amount of used memory.
      m_UsedMemory -= pMemoryHeader->Size;

      return;
    }
  }
  /*********************************************************************************/
}}//end of K15_Engine::System namespace