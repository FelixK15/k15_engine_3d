/**
 * @file K15_Event.cpp
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
#include "K15_PrecompiledHeader.h"

#include "K15_GameEvent.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	GameEvent::GameEvent(const EventName& p_Name,void* p_Argument,uint32 p_ArgumentSize)
		: m_Name(p_Name),
		  m_ArgumentSize(p_ArgumentSize),
		  m_Argument(0)
	{
		if(m_ArgumentSize > 0)
		{
      m_Argument = K15_NEW_SIZE(MemoryAllocator,m_ArgumentSize) byte[m_ArgumentSize];
			memcpy(m_Argument,p_Argument,m_ArgumentSize);
		}
	}
	/*********************************************************************************/
	GameEvent::GameEvent(const GameEvent& p_Other)
		: m_Name(p_Other.m_Name),
		  m_Argument(0),
		  m_ArgumentSize(0)
	{
		copyFrom(p_Other);
	}
	/*********************************************************************************/
	GameEvent::~GameEvent()
	{
		if(m_Argument != 0)
		{
      K15_DELETE_T(MemoryAllocator,m_Argument,m_ArgumentSize);
		}
	}
	/*********************************************************************************/
	void GameEvent::copyFrom(const GameEvent& p_Other)
	{
		m_Name = p_Other.m_Name;
		m_ArgumentSize = p_Other.m_ArgumentSize;

		if(p_Other.m_Argument)
		{
      m_Argument = K15_NEW_SIZE(MemoryAllocator,m_ArgumentSize) byte[m_ArgumentSize];
			memcpy(m_Argument,p_Other.m_Argument,m_ArgumentSize);
		}
	}
	/*********************************************************************************/
	const GameEvent& GameEvent::operator=(const GameEvent& p_Other)
	{
		copyFrom(p_Other);

		return *this;
	}
	/*********************************************************************************/
	bool GameEvent::operator==(const GameEvent& p_Other) const
	{
		return m_Name == p_Other.m_Name;
	}
	/*********************************************************************************/
	bool GameEvent::operator!=(const GameEvent& p_Other) const
	{
		return !(m_Name == p_Other.m_Name);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace