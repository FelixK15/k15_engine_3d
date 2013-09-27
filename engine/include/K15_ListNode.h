/**
 * @file K15_ListNode.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/09
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
 * ListNode are nodes within a list.
 * They provide public functions to get the next and the previous node as well
 * as a function to get the element this node represents.
 *
 * ListNode uses pointers to reference the element it holds.
 */
#pragma once

#ifndef __K15_LISTNODE__
#define __K15_LISTNODE__

#include "K15_StdInclude.h"
#include "K15_MemoryPool.h"

namespace K15_Engine { namespace Container { 
{
	template <class T>
	class ListNode
	{
	public: 
		template<class T> friend class List;

		/**
		* Standard Constructor
		*/
		ListNode();

		/**
		* Standard Destructor
		*/
		~ListNode();

		/**
		* Retrieve the element the node is associated with.
		*
		* @return T& - Reference to the element the node is associated with.
		*/
		T& GetElement();

		/**
		* Retrieve the next node in the list.
		*
		* @return ListNode* - pointer to the next node in the list.
		*/
		ListNode<T> *GetNext();

		/**
		* Retrieve the previous node in the list.
		*
		* @return ListNode* - pointer to the nprevious node in the list.
		*/
		ListNode<T> *GetPrev();

		void *operator new(uint32 iSize);

		void operator delete(void *pPointer);

	private:
		static MemoryPool<ListNode<T>,50000> ms_mpMemoryPool;

		T *m_pElement;			//Pointer to the element the node is associated with.
		ListNode<T> *m_pNext;		//Pointer to the next element in the list.
		ListNode<T> *m_pPrevious;		//Pointer to the previous element in the list.
	};

	#include "K15_ListNode.inl"
}}

#endif //__K15_LISTNODE__