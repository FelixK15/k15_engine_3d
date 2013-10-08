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
 *
 * @section DESCRIPTION
 * 
 */

#ifndef _K15Engine_System_StackAllocationPattern_h_
#define _K15Engine_System_StackAllocationPattern_h_

#include "K15_Prerequisites.h"
#include "K15_BaseAllocator.h"

namespace K15_Engine { namespace System {

  class K15_API_EXPORT StackAllocator : public BaseAllocator
  {
  public:
	  /*********************************************************************************/
	  static const TypeName Type;
	  /*********************************************************************************/
  public:
	//allocate memory from another allocator
    StackAllocator(BaseAllocator* p_Allocator,uint32 p_Size);
    
	//allocate memory using malloc
    StackAllocator(uint32 p_Size);

    virtual void clear();
  protected:
    virtual void* alloc(uint32 p_Size);
    virtual void free(void* p_Pointer);

  protected:
    byte* m_Marker;
  };
#include "K15_StackAllocator.inl"
}} //end of K15_Engine::System namespace

#endif //_K15Engine_System_StackAllocationPattern_h_