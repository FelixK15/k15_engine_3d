/**
 * @file K15_BaseAllocator.h
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
 *
 * @section DESCRIPTION
 *
 */

#ifndef _K15Engine_Core_BaseAllocator_h_
#define _K15Engine_Core_BaseAllocator_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_HashedString.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_MemoryHeader.h"

namespace K15_Engine { namespace Core {
  
  class K15_CORE_API BaseAllocator
  {
  public:
    BaseAllocator(size_t p_Size,const ObjectName& p_Name);
    BaseAllocator(BaseAllocator* p_Allocator,size_t p_Size,const ObjectName& p_Name);
    virtual ~BaseAllocator();

    void* allocate(size_t p_Size);
#if defined K15_DEBUG
    void* allocateDebug(size_t p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function);
#endif //K15_DEBUG
    void  deallocate(void* p_Pointer,size_t p_Size);
#if defined K15_DEBUG
    void  deallocateDebug(void* p_Pointer,size_t p_Size,const char* p_File,int p_Line,bool p_Array,const char* p_Function);
#endif //K15_DEBUG

	const ObjectName& getName() const;

    virtual void clear();

  protected:
    virtual void* alloc(size_t p_Size) = 0;
    virtual void  dealloc(void* p_Pointer,size_t p_Size) = 0;

  protected:
	ObjectName m_Name;
    BaseAllocator* m_Allocator;
    byte* m_Memory;
    byte* m_MemoryEndAddress;
	uint32 m_MemorySize;
    uint32 m_UsedMemory;
  };//end of BaseAllocator class
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_BaseAllocator_h_