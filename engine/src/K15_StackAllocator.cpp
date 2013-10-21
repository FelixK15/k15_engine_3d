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

#include "K15_PrecompiledHeader.h"

#include "K15_StackAllocator.h"
#include "K15_MemoryHeader.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  const TypeName StackAllocator::Type = _TN(StackAllocator);
  /*********************************************************************************/

  /*********************************************************************************/
  StackAllocator::StackAllocator( BaseAllocator* p_Allocator,uint32 p_Size )
    : BaseAllocator(p_Allocator,p_Size),
      m_Marker(m_Memory)
  {

  }
  /*********************************************************************************/
  StackAllocator::StackAllocator(uint32 p_Size)
    : BaseAllocator(p_Size),
      m_Marker(m_Memory)
  {

  }
/*********************************************************************************/
  void* StackAllocator::alloc(uint32 p_Size)
  {
    void* address = (void*)m_Marker;
    //shift and return memory
    m_Marker += p_Size;

    return address;
  }
  /*********************************************************************************/
  void StackAllocator::free(void* p_Pointer)
  {
    while(m_Marker != p_Pointer)
    {
      --m_Marker; //decrease the marker until enough memory has been 'freed'
    }
  }
  /*********************************************************************************/
  void StackAllocator::clear()
  {
    m_Marker = m_Memory;
  }
  /*********************************************************************************/
}}//end of K15_Engine::System namespace