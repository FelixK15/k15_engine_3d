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
#include "K15_MemoryBlock.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	BlockAllocator::BlockAllocator(size_t p_Size, const String& p_Name, BaseAllocator* p_BaseAllocator)
		: BaseAllocator(p_Size, p_Name, p_BaseAllocator),
		  m_First(0),
		  m_Current(0)
	{

	}
	/*********************************************************************************/
	BlockAllocator::~BlockAllocator()
	{
		clear();
	}
	/*********************************************************************************/
	void BlockAllocator::clear()
	{
		BaseAllocator::clear();
	}
	/*********************************************************************************/
	void* BlockAllocator::alloc(size_t p_Size)
	{
		MemoryBlock* block = findBlock_R(m_Current, p_Size);

		if(!block)
		{
			//no block has been found, that either means we are out of memory or there are no blocks
			//with enough size...Maybe if we defragment the block we'll have enough size...?
			defragment_R(m_First, p_Size);

			if((block = findBlock_R(m_First,p_Size)) == 0)
			{
				//...nope still no luck...out of memory
				return 0;
			}
		}

		m_Current = block;

		return block->Memory;
	}
  /*********************************************************************************/
  MemoryBlock* BlockAllocator::findBlock_R(MemoryBlock* p_Block,size_t p_Size)
  {
		MemoryBlock* newBlock = 0;

		//is there enough memory left to satisfy allocation?
		if(m_MemorySize - m_UsedMemory < p_Size + sizeof(MemoryBlock))
		{
			return 0; // nope, try defragmentation
		}

		//can the passed block maybe satisfy the allocation?
		if(p_Block && !p_Block->Used && p_Block->Size >= p_Size)
		{
			//yep, mark block as used and return it.
			p_Block->Used = true;
			return p_Block;
		}

		//if current block can not satisfy allocation,
		//maybe its neighbor can?
		if(p_Block && p_Block->Next)
		{
			return findBlock_R(p_Block->Next, p_Size);
		}

		// if no block has been passed in or the block has no neighbor,
		// create new block
		if(!p_Block)
		{
			newBlock = (MemoryBlock*)m_Memory;
		}
		else if(!p_Block->Next)
		{
			//Check if we're at the memory boundary
			ptrdiff_t newBlockEndPos = (ptrdiff_t)(p_Block->Memory + p_Size + sizeof(MemoryBlock));
			ptrdiff_t memoryEndPos = (ptrdiff_t)m_MemoryEndAddress;

			if(newBlockEndPos >= memoryEndPos)
			{
			//yep, we are at the memory boundary - try defragmentation
			return 0;
			}

			newBlock = (MemoryBlock*)(p_Block->Memory + p_Block->Size);
			p_Block->Next = newBlock;
		}

		newBlock->Memory = (byte*)newBlock;
		newBlock->Memory += sizeof(MemoryBlock);
		newBlock->Size = p_Size;
		newBlock->Next = 0;
		newBlock->Used = true;

		// is this the first allocation?
		if(!m_First)
		{
			m_First = newBlock;
		}

		// add size of memory block to used memory size as 
		// we use that much memory additionally per allocation
		m_UsedMemory += sizeof(MemoryBlock);

		return newBlock;
	}
	/*********************************************************************************/
	void BlockAllocator::dealloc(void* p_Pointer,size_t p_Size)
	{
		dealloc_R(m_Current, p_Pointer, p_Size);
	}
	/*********************************************************************************/
	void BlockAllocator::defragment_R(MemoryBlock* p_Block, size_t p_Size)
	{
		while(p_Block && p_Block->Next)
		{
			if(!p_Block->Used && !p_Block->Next->Used)
			{
				if((p_Block->Size + p_Block->Next->Size) >= p_Size)
				{
						  K15_LOG_DEBUG("Merging block %p(size %u) and block %p(size %u) from BlockAllocator \"%s\"...",
							  p_Block,p_Block->Size,p_Block->Next,p_Block->Next->Size,m_Name.c_str());

						  p_Block->Size += p_Block->Next->Size + sizeof(MemoryBlock); //memblock is part of memory so we add the size of the block to get the 'real' size.
						  p_Block->Next = p_Block->Next->Next;

				  // keep track of how much memory is used
				  m_UsedMemory -= sizeof(MemoryBlock);
				  break;
				}
			}

			p_Block = p_Block->Next;
		}
	}
	/*********************************************************************************/
	void BlockAllocator::dealloc_R(MemoryBlock* p_Block,void* p_Pointer,size_t p_Size)
	{
		while(p_Block)
		{
			if(p_Block->Memory == p_Pointer)
			{
				p_Block->Used = false;
				break;
			}

			p_Block = p_Block->Next;
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace
