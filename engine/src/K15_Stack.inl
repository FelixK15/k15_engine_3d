/**
 * @file K15_Stack.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
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
 */

template<class T>
Stack<T>::Stack()
{
	m_iSize = 0;
	m_pTop = 0;
}

template<class T>
Stack<T>::~Stack()
{
	Clear();
}

template<class T>
void Stack<T>::Push(const T& tElement)
{
	StackItem<T> *pNewItem = 0;

	pNewItem = K15_NEW StackItem<T>();
	pNewItem->m_pObjectPointer = K15_NEW T(tElement);
	pNewItem->m_pPrevious = m_pTop;

	m_pTop = pNewItem;

	++m_iSize;
}

template<class T>
void Stack<T>::Pop()
{
	StackItem<T> *pPreviousItem = m_pTop->m_pPrevious;
	K15_DELETE m_pTop;

	m_pTop = pPreviousItem;

	--m_iSize;
}

template<class T>
void Stack<T>::Clear()
{
	for(uint32 i = 0;i < m_iSize;Pop());
	m_iSize = 0;
}

template<class T>
T& Stack<T>::Top()
{
	return (*m_pTop->m_pObjectPointer);
}

template<class T>
uint32 Stack<T>::Size() const
{
	return m_iSize;
}