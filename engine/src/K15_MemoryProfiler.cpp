/**
 * @file K15_MemoryProfiler.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/08
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
#include "K15_MemoryProfiler.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	MemoryProfiler::MemoryProfiler()
		: m_Allocator()
	{

	}
	/*********************************************************************************/
	MemoryProfiler::~MemoryProfiler()
	{

	}
	/*********************************************************************************/
	void MemoryProfiler::addAllocator(BaseAllocator* p_Allocator)
	{
		m_Allocator.push_back(p_Allocator);
	}
	/*********************************************************************************/
	void MemoryProfiler::removeAllocator(BaseAllocator* p_Allocator)
	{
		m_Allocator.remove(p_Allocator);	
	}
	/*********************************************************************************/
	const MemoryProfiler::AllocatorList& MemoryProfiler::getAllocators() const
	{
		return m_Allocator;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace