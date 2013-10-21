/**
 * @file K15_TaskBase.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 *
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

#ifndef _K15Engine_System_TaskBase_h_
#define _K15Engine_System_TaskBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#	include "K15_TaskManager.h"
#endif// K15_USE_PRECOMPILED_HEADER


#include "K15_Functor.h"
#include "K15_GameTime.h"

namespace K15_Engine { namespace System {
	class TaskBase : public TaskManagerAllocatedObject
	{
	public:
		TaskBase(uint32 p_Priority = 0);
		~TaskBase();

		virtual void update(const GameTime& p_GameTime) = 0;
		
		inline uint32 getPriority() const;
		inline void setPriority(uint32 p_Priority);

		bool operator<(const TaskBase& p_Other);
	protected:
		uint32 m_Priority;
	};// end of TaskBase class
#include "K15_TaskBase.inl"
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_TaskBase_h_