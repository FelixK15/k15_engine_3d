/**
 * @file K15_List.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/24
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
 * This is a classical implementation of a linked list.
 *
 * @todo Provide a memory container from which the ListNodes objects will get allocated.
 */

#pragma once

#ifndef __K15_LIST__
#define __K15_LIST__

#include "K15_StdInclude.h"
#include "K15_ListNode.h"

namespace K15_EngineV2
{
	template<class T>
	class List
	{
	public:
		/**
		* Standard Constructor
		*/
		List();

		/**
		* Standard Destructor
		*/
		~List();

		/**
		* Adds an element at the end of the list.
		*
		* @param  tElement - The element that gets added at the end of the list.
		*/
		void PushBack(const T& tElement);

		/**
		* Adds an element at the beginning of the list.
		*
		* @param tElement - The element that gets added at the beginning of the list.
		*/
		void PushFront(const T& tElement);

		/**
		* Erase the element at the end of the array.
		*/
		void PopBack();

		/**
		* Erase the element at the beginning of the array.
		*/
		void PopFront();

		/**
		* Clears the complete list (All nodes will get deleted).
		*/
		void Clear();

		/**
		* Erases a specific node within the list.
		*
		* @param  pNodeToErase - pointer to the node that shall be erased.
		*/
		void Erase(ListNode<T> *pNodeToErase);

		/**
		* Returns the amount of elements in the list.
		*
		* @return U32 - amount of elements in the list.
		*/
		U32 Size() const;

		/**
		* Returns a pointer to the ListNode at the beginning of the list.
		*
		*
		* @return ListNode* - pointer to the ListNode at the beginning of the list.
		*/
		ListNode<T> *GetFirstElement() const;

		/**
		* Returns a pointer to the ListNode at the end of the list.
		*
		*
		* @return ListNode* - pointer to the ListNode at the end of the list.
		*/
		ListNode<T> *GetLastElement() const;

	private:
		ListNode<T> *_CreateNewNode(const T& tElement);
		void _CheckForOphrandTailAndHead();

	private:

		U32 m_iSize;
		ListNode<T> *m_pFirst;
		ListNode<T> *m_pLast;
	};

	#include "../src/K15_List.inl"
}



#endif //__K15_LIST__
