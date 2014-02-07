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

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	StackAllocator::StackAllocator(size_t p_Size,const String& p_Name,BaseAllocator* p_BaseAllocator)
		: BaseAllocator(p_Size,p_Name,p_BaseAllocator),
		m_Marker(m_Memory)
	{

	}
	/*********************************************************************************/
	void* StackAllocator::alloc(size_t p_Size)
	{
		void* address = (void*)m_Marker;
		//shift and return memory
		m_Marker += p_Size;

		return address;
	}
	/*********************************************************************************/
	void StackAllocator::dealloc(void* p_Pointer, size_t p_Size)
	{
		K15_ASSERT((byte*)p_Pointer + p_Size == m_Marker,
		StringUtil::format("Trying to free not in-order from StackAllocator %s. StackAllocator deallocations have to be in the reversed order as the allocations.",m_Name.c_str()));
		m_Marker = (byte*)p_Pointer;
	}
	/*********************************************************************************/
	void StackAllocator::clear()
	{
		BaseAllocator::clear();
		m_Marker = m_Memory;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace