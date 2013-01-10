/**
 * @file K15_Heap.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/06
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
 * Heap is a class that represents a memory heap of specific size.
 * Heaps are not meant to be used directly. The Heap class is just a 
 * helper class for the class MemoryHeap.
 *
 * Memory will be stored in a continuous block of memory.
 * For each new allocation, a new block of memory will get saved in an intern list.
 *
 * As soon as blocks are freed, the class will try to merge them together with adjacent 
 * blocks to prevent memory fragmentation.
 */
#pragma once

#ifndef __K15_HEAP__
#define __K15_HEAP__

#include "K15_StdInclude.h"

#include "K15_MemoryHeapNode.h"

namespace K15_EngineV2
{
	template<class T>
	class List;

	typedef List<MemoryHeapNode>* MemoryBlockList;

	class K15ENGINE2_API Heap
	{
	public:
		/**
		* Standard Constructor.
		*
		* @param  iSize - amount of memory that is going to get allocated for this heap. (in byte)
		*/
		Heap(size_t iSize);

		/**
		* Copy Constructor.
		* Will make a deep copy of otherHeap.
		*
		* @param  hpSource - A Heap object you want to create a deep copy of.
		*
		* @return 
		*/
		Heap(const Heap& hpSource);

		/**
		* Standard Destructor.
		*/
		~Heap();

		/**
		* Allocates a new block of memory of given size (Or NULL if there's not enough space).
		*
		* @param iSize - size of memory you want to allocate.
		*
		* @return void* - ptr to new memory or NULL if there was not enough space.
		*/
		void *Allocate(size_t iSize);

		/**
		* Frees a previously allocated block of memory.
		*
		* @param  pPointer - ptr to previously allocated memory.
		* @param  iSizeOut - (out) size of memory ptr is pointing to.
		*
		* @return bool - true if free was succesful and false if not.
		*/
		bool Free(void *pPointer,size_t &iSizeOut);

		/**
		* Returns to total amount of free memory in byte.
		*
		* @return size_t - amount of free memory in byte.
		*/
		size_t GetFreeMemory() const;

		/**
		* Returns to total amount of used memory in byte.
		*
		* @return size_t - amount of used memory in byte.
		*/
		size_t GetUsedMemory() const;

		/**
		* Returns to total amount of memory for this heap in byte.
		*
		* @return size_t - amount of memory for this heap in byte.
		*/
		size_t GetHeapSize() const;

	private:
		bool _MergeBlocks(MemoryHeapNode &mbBlock);	//Function will get called only internally after a block of memory has been freed.

	private:
		byte *m_pHeap;						//Block of memory where memory will be saved.
		size_t m_iSize;						//Size of the block of memory.
		size_t m_iUsedMemory;				//Size of memory that is in use.
		MemoryBlockList m_lMemoryBlocks;	//List of memory blocks.
	};

	#include "../src/K15_Heap.inl"
}

#endif //__K15_HEAP__