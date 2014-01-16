/**
 * @file K15_MemoryPools.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
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
 */

#include "K15_PrecompiledHeader.h"

#include "K15_MemoryPools.h"

namespace K15_Engine { namespace Core {
	RenderOperationAllocator::RenderOperationAllocator()
		: PoolAllocator<K15_SIZE_RENDER_OPERATION>(MemoryPoolsAllocator,K15_RENDER_OPERATION_POOLSIZE,_N(RenderOperationAllocator))
	{

	}
	/*********************************************************************************/
	RenderOperationAllocator::~RenderOperationAllocator()
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	const uint32 STLBlockAllocator::STLBlockAllocatorSize = 5 * MEGABYTE;
	/*********************************************************************************/
	STLBlockAllocator::STLBlockAllocator()
	: BlockAllocator(MemoryPoolsAllocator,STLBlockAllocatorSize,_N(STLBlockAllocator))
	{

	}
	/*********************************************************************************/
	STLBlockAllocator::~STLBlockAllocator()
	{

	}
	/*********************************************************************************/

	MemoryBlockAllocator::MemoryBlockAllocator()
	: PoolAllocator<K15_SIZE_MEMORY_BLOCK>(MemoryPoolsAllocator,K15_MEMORY_BLOCK_POOLSIZE,_N(MemoryBlockAllocator))
	{

	}
	/*********************************************************************************/
	MemoryBlockAllocator::~MemoryBlockAllocator()
	{

	}
	/*********************************************************************************/

	MaterialAllocator::MaterialAllocator()
	: PoolAllocator<K15_SIZE_MATERIAL>(MemoryPoolsAllocator,K15_MATERIAL_POOLSIZE,_N(MaterialAllocator))
	{

	}
	/*********************************************************************************/
	MaterialAllocator::~MaterialAllocator()
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	MemoryPools::MemoryPools()
	: StackAllocator(ApplicationAllocator,10 * MEGABYTE,_N(MemoryPools))
	{
		_LogNormal("Creating RenderOperation memory pool...");
		m_RopAllocator = K15_NEW RenderOperationAllocator();

		_LogNormal("Creating Material memory pool...");
		m_MaterialAllocator = K15_NEW MaterialAllocator();

		_LogNormal("Creating MemoryBlock memory pool...");
		m_MemoryBlockAllocator = K15_NEW MemoryBlockAllocator();

//     _LogNormal("Creating STLBlock memory pool...");
//     m_STLBlockAllocator = K15_NEW STLBlockAllocator();
	}
	/*********************************************************************************/
	MemoryPools::~MemoryPools()
	{
//     _LogNormal("Destroying STLBlock memory pool...");
//     K15_DELETE m_STLBlockAllocator;

		_LogNormal("Destroying MemoryBlock memory pool...");
		K15_DELETE m_MemoryBlockAllocator;

		_LogNormal("Destroying Material memory pool...");
		K15_DELETE m_MaterialAllocator;

    _LogNormal("Creating RenderOperation memory pool...");
    K15_DELETE m_RopAllocator;
	}
  /*********************************************************************************/
}} // end of K15_Engine::Core namespace