/**
 * @file K15_TaskManager.h
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
 * 
 */

#ifndef _K15Engine_System_TaskManager_h_
#define _K15Engine_System_TaskManager_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#endif// K15_USE_PRECOMPILED_HEADER

#include "K15_AllocatedObject.h"
#include "K15_PageAllocator.h"
#include "K15_Singleton.h"
#include "K15_TaskBase.h"

namespace K15_Engine { namespace System {

	class TaskManager : public ApplicationAllocatedObject,
						public Singleton<TaskManager>,
						public PageAllocator<>
	{
	public:
		/*********************************************************************************/
		typedef K15_List(TaskBase*) TaskList;
		/*********************************************************************************/
	public:
		TaskManager();
		virtual ~TaskManager();

		void update(const GameTime&);

		inline void addTask(TaskBase*);
		void removeTask(TaskBase*);

	protected:
		TaskList m_Tasks;
	};//end of TaskManager class
#include "K15_TaskManager.inl"
}} //end of K15_Engine::System

#endif //_K15Engine_System_TaskManager_h_