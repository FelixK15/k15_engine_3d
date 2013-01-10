/**
 * @file K15_MemoryHeap.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
 * MemoryHeap represents a continuous block of memory where you can allocate 
 * and free memory in arbitrary order.
 *
 * Memory will get split up internally in several Heaps.
 * Once memory will get allocated, the caller of the Allocate function will receive
 * the ID of the Heap, the allocated memory is held in. This ID can be used later to speed
 * up the freeing process extremly.
 */
#pragma once

#ifndef __K15_ABSTRACTMEMORYHEAP__
#define __K15_ABSTRACTMEMORYHEAP__

#include "K15_StdInclude.h"
#include "K15_Heap.h"

#include "K15_DynamicArray.h"

namespace K15_EngineV2
{
	typedef DynamicArray<Heap> HeapArray;
	typedef U16 HeapID;

	template <class T,size_t OBJ_COUNT_PER_HEAP>
	class MemoryHeap
	{
	public:
		/**
		* Same as Allocate(size_t), but allocates just 1 object.
		*
		* @return T* - ptr to newly allocated memory (or NULL if something went wrong).
		*/
		static T *Allocate();

		/**
		* Allocates a new block of memory.
		*
		* @param  numObjects - number of objects you want to allocate.
		*
		* @return T* - ptr to newly allocated memory (or NULL if something went wrong).
		*/
		static T *Allocate(size_t iNumberObjects);

		/**
		* Try to free previously allocated memory WITHOUT the use of the heapID.
		*
		* @param  pPointer - Pointer to previously allocated memory.
		*/
		static void Free(void* pPointer);

		/**
		* Erases the complete memory within each heaps and deletes each heap itself
		*/
		static void Clear();

		/**
		* Get the total amount of memory in byte that is still unused.
		*
		* @return size_t - total amount of memory in byte that is still unused.
		*/
		static size_t GetFreeMemory();

		/**
		* Get the total amount of memory in byte that is in use.
		*
		* @return size_t - total amount of memory in byte that is in use.
		*/
		static size_t GetUsedMemory();

		/**
		* Get the total amount of memory that is used by the class.
		*
		* @return size_t - total amount of memory that is used by the class.
		*/
		static size_t GetAmountMemory();

		/**
		* Get the size of one heap in byte.
		*
		* @return size_t - size of one heap in byte.
		*/
		static size_t GetSizePerHeap();

		/**
		* Get the amount of heaps used by MemoryHeap.
		*
		* @return U32 - amount of heaps used by MemoryHeap.
		*/
		static U32 GetAmountHeaps();

	private:
		static T* _AllocateOnHeap(size_t iSize,HeapID iID);

	private:
		static HeapArray ms_arrHeaps;

		static size_t ms_iSizePerHeap;
		static size_t ms_iUsedMemory;

		static const size_t MEMORY_OFFSET;
	};

	#include "../src/K15_MemoryHeap.inl"
}

#endif //__K15_ABSTRACTMEMORYHEAP__