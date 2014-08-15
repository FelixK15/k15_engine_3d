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
	BlockAllocator::BlockAllocator(size_t p_Size,const String& p_Name,BaseAllocator* p_BaseAllocator)
		: BaseAllocator(p_Size,p_Name,p_BaseAllocator),
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
		if(!m_First) // first block allocation
		{
			p_Size += sizeof(MemoryBlock);
            m_First = (MemoryBlock*)m_Memory;
            m_First->Memory = m_Memory + sizeof(MemoryBlock);
			m_First->Used = true;
			m_First->Size = p_Size -= sizeof(MemoryBlock);
			m_First->Next = 0;

			m_Current = m_First;

			return m_First->Memory;
		}
  
        MemoryBlock* block = findBlock_R(m_Current, p_Size);

		if(!block)
		{
			//no block has been found, that either means we are out of memory or there are no blocks
			//with enough size...Maybe if we defragment the block we'll have enough size...?
			defragment_R(m_First);

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
	if(!p_Block->Next)
	{
		if(m_MemorySize - m_UsedMemory >= p_Size+sizeof(MemoryBlock))
		{
			p_Block->Next = (MemoryBlock*)(p_Block->Memory+p_Block->Size);
			p_Block->Next->Memory = p_Block->Memory + (p_Block->Size + sizeof(MemoryBlock));
			p_Block->Next->Size = p_Size;
			p_Block->Next->Used = true;

			return p_Block->Next;
		}

		return 0; //not enough free space left
	}
    else if(!p_Block->Used && p_Size <= p_Block->Size)
    {
		p_Block->Used = true;
		return p_Block; //current block is not used and has enough size
    }

		return findBlock_R(p_Block->Next,p_Size);
	}
	/*********************************************************************************/
	void BlockAllocator::dealloc(void* p_Pointer,size_t p_Size)
	{
		dealloc_R(m_First,p_Pointer,p_Size);
	}
	/*********************************************************************************/
	void BlockAllocator::defragment_R(MemoryBlock* p_Block)
	{
		while(p_Block && p_Block->Next)
		{
			if(!p_Block->Used && !p_Block->Next->Used)
			{
				K15_LOG_DEBUG("Merging block %p(size %u) and block %p(size %u) from BlockAllocator \"%s\"...",
					p_Block,p_Block->Size,p_Block->Next,p_Block->Next->Size,m_Name.c_str());

				p_Block->Size += p_Block->Next->Size + sizeof(MemoryBlock); //memblock is part of memory so we add the size of the block to get the 'real' size.
				p_Block->Next = p_Block->Next->Next;
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
