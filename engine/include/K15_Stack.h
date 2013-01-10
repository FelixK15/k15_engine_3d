/**
 * @file K15_Stack.h
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
 * 
 */
#pragma once

#ifndef __K15_STACK__
#define __K15_STACK__

#include "K15_StdInclude.h"
#include "K15_StackItem.h"

namespace K15_EngineV2
{
	template <class T>
	class Stack
	{
	public:
		Stack();
		~Stack();

		void Push(const T& tElement);
		void Pop();
		void Clear();

		T& Top();

		U32 Size() const;

	private:
		StackItem<T> *m_pTop;
		U32 m_iSize;
	};

	#include "../src/K15_Stack.inl"
}



#endif //__K15_STACK__