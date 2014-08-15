/**
 * @file K15_BlockAllocator.h
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
 *
 * @section DESCRIPTION
 *
 * The BlockAllocator is a variation of the FreeList (http://en.wikipedia.org/wiki/Free_list)
 * data structure. The allocator typically works on a larger block of memory and
 * divides the block into smaller blocks with each memory allocation.
 *
 * Upon freeing previously allocated memory, the allocator will simply mark the
 * previously allocated block as "unused". Once the allocator runs out of memory
 * it will try to defragment the memoryblock it used by merging contigiuous unused
 * memory blocks.
 *
 * For each allocation the allocator will allocate and addition of sizeof(MemoryBlock) bytes
 * to store block information per allocation. These information will be stored at the
 * beginning of each allocation (however only the memory the user can actually work
 * with will be returned by alloc).
 */

#ifndef _K15Engine_Core_BlockAllocator_h_
#define _K15Engine_Core_BlockAllocator_h_

#include "K15_Prerequisites.h"
#include "K15_BaseAllocator.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API BlockAllocator : public BaseAllocator
	{
	public:
		BlockAllocator(size_t p_Size,const String& p_Name,BaseAllocator* p_BaseAllocator = 0);
	
		virtual ~BlockAllocator();

		virtual void clear() OVERRIDE;
		virtual void* alloc(size_t p_Size) OVERRIDE;
		virtual void dealloc(void* p_Pointer,size_t p_Size) OVERRIDE;

	private:
		void defragment_R(MemoryBlock* p_Block);
		void dealloc_R(MemoryBlock* p_Block,void* p_Pointer,size_t p_Size);
		MemoryBlock* findBlock_R(MemoryBlock* p_Block,size_t p_Size);

	private:
		MemoryBlock* m_First;
		MemoryBlock* m_Current;
	};// end of BlockAllocator class declaration
}}// end of K15_Engine::Core namespace

#endif //K15Engine_Core_BlockAllocator_h_
