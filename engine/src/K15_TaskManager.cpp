/**
 * @file K15_TaskManager.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/12
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

#include "K15_TaskManager.h"

namespace K15_Engine { namespace System {
	/*********************************************************************************/
	TaskManager::TaskManager()
		: PageAllocator(MemoryAllocator),
		  m_Tasks()
	{

	}
	/*********************************************************************************/
	TaskManager::~TaskManager()
	{

	}
	/*********************************************************************************/
	void TaskManager::update(const GameTime& p_GameTime)
	{
		for(TaskList::iterator iter = m_Tasks.begin();iter != m_Tasks.end();++iter)
		{
			(*iter)->update(p_GameTime);
		}
	}
	/*********************************************************************************/
	void TaskManager::removeTask(Task* p_Task)
	{
		TaskList::iterator toDelete = m_Tasks.end();

		for(TaskList::iterator iter = m_Tasks.begin();iter != m_Tasks.end();++iter)
		{
			if((*iter) == p_Task)
			{
				toDelete = iter;
				break;
			}
		}

		if(toDelete != m_Tasks.end())
		{
			m_Tasks.erase(toDelete);
		}

		m_Tasks.sort();
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace