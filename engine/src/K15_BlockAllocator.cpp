/**
 * @file K15_BlockAllocator.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/07
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

#include "K15_BlockAllocator.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	BlockAllocator::BlockAllocator(size_t p_Size,const ObjectName& p_Name)
		: BaseAllocator(p_Size,p_Name),
		  m_First(0)
	{
		m_First = new MemoryBlock;
		m_First->Memory = m_Memory;
		m_First->Size = m_MemorySize;
		m_First->Used = false;
		m_First->Next = 0;
		clear();
	}
	/*********************************************************************************/
	BlockAllocator::BlockAllocator(BaseAllocator* p_Allocator,size_t p_Size,const ObjectName& p_Name)
		: BaseAllocator(p_Allocator,p_Size,p_Name),
		  m_First(0)
	{
		m_First = new MemoryBlock;
		m_First->Memory = m_Memory;
		m_First->Size = m_MemorySize;
		m_First->Used = false;
		m_First->Next = 0;
		clear();
	}
	/*********************************************************************************/
	BlockAllocator::~BlockAllocator()
	{
		clear();
		delete m_First;
	}
	/*********************************************************************************/
	void BlockAllocator::clear()
	{
		MemoryBlock* currentBlock = m_First->Next;
		MemoryBlock* nextBlock = 0;
		while(currentBlock)
		{
			nextBlock = currentBlock->Next;
			::free(currentBlock);
			currentBlock = nextBlock;
		}
	}
	/*********************************************************************************/
	void* BlockAllocator::alloc(size_t p_Size)
	{
		MemoryBlock* currentBlock = m_First;
		bool merged = false;
		while(true)
		{
			if(!currentBlock->Used)
			{
				if(currentBlock->Size >= p_Size)
				{
					break; //we got our block!
				}
			}
			if(!currentBlock->Next && !merged)
			{
				_LogWarning("BlockAllocator \"%s\" has not enough free memory. Trying to merge blocks...",m_Name.c_str());
				mergeBlocks();
				merged = true;
				currentBlock = m_First;
			}
			else
			{
				currentBlock = currentBlock->Next;

				K15_ASSERT(currentBlock,StringUtil::format("BlockAllocator \"%s\" has not enough free memory to satisfy memory request.",m_Name.c_str()));
			}
		}

		byte* memory = currentBlock->Memory;
		currentBlock->Size = p_Size;
		currentBlock->Used = true;

		createBlock(currentBlock);

		return memory;
	}
	/*********************************************************************************/
	void BlockAllocator::free(void* p_Pointer,size_t p_Size)
	{
		MemoryBlock* currentBlock = m_First;

		while(currentBlock->Memory != p_Pointer)
		{
			currentBlock = currentBlock->Next;
			
			K15_ASSERT(currentBlock,StringUtil::format("Address %p does not belong to BlockAllocator \"%s\".",p_Pointer,m_Name.c_str()));
		}

		currentBlock->Used = false;
	}
	/*********************************************************************************/
	void BlockAllocator::mergeBlocks()
	{
		MemoryBlock* currentBlock = m_First;
		while(currentBlock && currentBlock->Next)
		{
			if(!currentBlock->Used && !currentBlock->Next->Used)
			{
				_LogDebug("Merging block %p(size %u) and block %p(size %u) from BlockAllocator \"%s\"...",
					currentBlock,currentBlock->Size,currentBlock->Next,currentBlock->Next->Size,m_Name.c_str());
				
				currentBlock->Size += currentBlock->Next->Size;
				currentBlock->Next = currentBlock->Next->Next;
				
				delete currentBlock->Next;
			}

			currentBlock = currentBlock->Next;
		}
	}
	/*********************************************************************************/
	void BlockAllocator::createBlock(MemoryBlock* p_Successor)
	{
		if((ptrdiff_t)(p_Successor->Memory + p_Successor->Size) > (ptrdiff_t)m_MemoryEndAddress)
		{
			_LogWarning("BlockAllocator \"%s\" is full.",m_Name.c_str());
		}
		else if((ptrdiff_t)(p_Successor->Memory + p_Successor->Size) != (ptrdiff_t)m_MemoryEndAddress)
		{
			MemoryBlock* newblock = new MemoryBlock;
			newblock->Used = false;
			newblock->Memory = p_Successor->Memory + p_Successor->Size;
			newblock->Size = m_MemorySize - (m_UsedMemory + p_Successor->Size);
			newblock->Next = 0;

			p_Successor->Next = newblock;
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace