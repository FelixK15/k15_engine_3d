/**
 * @file K15_EventTask.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
#include "K15_EventTask.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const uint32 EventTask::TaskPriority = 1;
	K15_IMPLEMENT_RTTI(Core,EventTask);
	/*********************************************************************************/
	EventTask::EventTask()
		: TaskBase(TaskPriority),
		m_EventManager(0)
	{

	}
	/*********************************************************************************/
	EventTask::~EventTask()
	{

	}
	/*********************************************************************************/
	void EventTask::update(const GameTime& p_GameTime)
	{
		if(!m_EventManager)
		{
			m_EventManager = g_EventManager;
		}

		m_EventManager->update();
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace