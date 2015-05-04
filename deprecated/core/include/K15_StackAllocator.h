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

#ifndef _K15Engine_Core_StackAllocator_h_
#define _K15Engine_Core_StackAllocator_h_

#include "K15_Prerequisites.h"
#include "K15_BaseAllocator.h"

namespace K15_Engine { namespace Core {

  class K15_CORE_API StackAllocator : public BaseAllocator
  {
  public:
    StackAllocator(size_t p_Size,const String& p_Name,BaseAllocator* p_BaseAllocator = 0);

    virtual void clear();

  protected:
    virtual void* alloc(size_t p_Size);
    virtual void dealloc(void* p_Pointer,size_t p_Size);

  protected:
    byte* m_Marker;
  };// end of StackAllocator class
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_StackAllocator_h_