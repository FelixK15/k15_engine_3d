/**
 * @file K15_DynamicArray.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/13
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
 * Dynamic Array is an array which grows by usage (unlike a static allocated array).
 * The growth is programmed that each time the array is full, it will get reallocated
 * with twice the space it has been allocated previously.
 *
 * For example:
 * Size() = 8
 * - _Reallocate is called internally
 * Size() = 16
 * - _Reallocate is called internally
 * Size() = 32
 * - _Reallocate is called internally
 * Size() = 64
 */
#pragma once

#ifndef __K15_DYNAMICARRAY__
#define __K15_DYNAMICARRAY__

#include "K15_StdInclude.h"

#include "K15_System.h"

namespace K15_EngineV2
{
	template<class T>
	class DynamicArray
	{
	public:

		/** 
		  * Standard Constructor
		  *
		  * @param iSize - amount of elements the array will alocate on construction. (8 by default)
		  */
		DynamicArray(U32 iSize = 8);

		/** 
		  * Copy Constructor
		  *
		  * @param arrSource - DynamicArray Object you want to copy from.
		  */
		DynamicArray(const DynamicArray<T> &arrSource);

		/** 
		  * Standard Destructor
		  */
		~DynamicArray();

		/** 
		  * Adds an object at the end of the array.
		  * @note Reallocation might occur during call if the size of the array is insufficient to hold any more objects.
		  *
		  * @param tObject - Reference of an object that shall be added at the end of the array.
		  */
		void PushBack(const T &tObject);
		
		/** 
		  * Adds an object at the beginning of the array.
		  * @note Reallocation might occur during call if the size of the array is insufficient to hold any more objects.
		  *
		  * @param tObject - Reference of an object that shall be added at the beginning of the array.
		  */
		void PushFront(const T &tObject);

		/** 
		  * Removes the first object of the array.
		  * @warning Be aware that memory shifting occurs during this call to prevent holes at the beginning of the array.
		  * @todo maybe just set m_iFirstElement instead of memory shifting.
		  *
		  * Example:
		  * DynamicArray content:  5, 8, 7, 6, 16
		  * Index Position:        0  1  2  3  4
		  * - PopFront() is called.
		  * DynamicArray content:  8, 7, 6, 16
		  * Index Position:        0  1  2  3
		  */
		//void PopFront();

		/** 
		  * Removes the last object of the array.
		  */
		//void PopBack();

		/** 
		  * Removes an object at a given position.
		  * @warning Be aware that memory shifting occurs during this call to prevent holes within the array
		  *
		  * @param iPos - index pos where the object lies that shall get popped.
		  */
		void Pop(U32 iPos);

		/** 
		  * Removes objects between the given start and end positions.
		  * @warning Be aware that memory shifting occurs during this call to prevent holes within the array
		  *
		  * @param iStart - start index position.
		  * @param iEnd - end index positition.
		  */
		//void Pop(U32 iStart,U32 iEnd);

		/** 
		  * Erases the complete content of the DynamicArray.
		  */
		void Clear();

		/** 
		  * Use this function to retrieve an object at the given index position.
		  *
		  * @param iPos - index position of the object you want to retrieve.
		  * @returns T& - Reference to the object at the given index position.
		  */
		T& Get(U32 iPos);

		/** 
		  * Use this function to retrieve an object at the given index position.
		  *
		  * @param iPos - index position of the object you want to retrieve.
		  * @returns T& - Reference to the object at the given index position.
		  */
		const T& Get(U32 iPos) const;


		/** 
		  * Returns the Size of the DynamicArray (How many objects are in this container)
		  *
		  * @returns U32 - amount of objects.
		  */
		U32 Size() const;

		/** 
		  * Returns the capacity of the DynamicArray. (How many objects can the DynamicArray 
		  * hold before it needs to reallocate)
		  *
		  * @returns U32 - Capacity (in object size)
		  */
		U32 Capacity() const;

		/** 
		  * Returns the size in byte the DynamicArray will reallocate if the current capacity is full.
		  *
		  * @returns U32 - amount of memory that'll get reallocated when the current capacity is full.
		  */
		U32 ReallocationSize() const;

		void Resize(U32 iObjCount);

		/**
		* Checks if the array contains a specific item.
		*
		* @param  tItem - Reference to the item
		*
		* @return bool - true if array contains the item or false if not
		*/
		bool HasItem(const T &tItem) const;

		const DynamicArray<T> &operator=(const DynamicArray<T> &arrSource);
		T &operator[](U32 iPos);

	private:

		void _CopyFrom(const DynamicArray<T> &arrList);
		void _Reallocate();
		void _ZeroUnusedMemory();

	private:
		T *m_pArr;					//Pointer to the array.
		size_t m_iSizeT;			//Size of the template class.
		U32 m_iCapacity;			//Capacity of the array (capacity in objects, not bytes).
		U32 m_iReallocateSize;		//Amount of objects that the array will grow if it is full. (amount in objects, not bytes).
		U32 m_iFirst;				//Index of the first element (is currently always == 0).
		U32 m_iLast;				//Index of the last element.
		U32 m_iGrowthFactor;		//Factor by which m_iReallocateSize will get multiplied if the array is full.
	};

	#include "../src/K15_DynamicArray.inl"
}



#endif //__K15_ARRAYLIST__