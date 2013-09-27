/**
 * @file K15_Task.h
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

#ifndef _K15Engine_System_Task_h_
#define _K15Engine_System_Task_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_TaskManager.h"
#include "K15_Functor.h"

namespace K15_Engine { namespace System {
	class Task : public TaskManagerAllocatedObject
	{
	public:
		/*********************************************************************************/
		typedef Functor1<void,const GameTime&> FunctorType;
		/*********************************************************************************/
	public:
		Task();
		Task(FunctorType p_Functor,bool p_Threaded = false,uint32 p_Priority = 0);
		~Task();

		inline void update(const GameTime& p_GameTime);
		inline void setUpdateFunc(FunctorType p_Functor);

		inline bool isThreaded();
		inline void setThreaded(bool p_Threaded);

		inline uint32 getPriority() const;
		inline void setPriority(uint32 p_Priority);

		bool operator<(const Task& p_Other);
	protected:
		uint32 m_Priority;
		FunctorType m_Functor;
		bool m_Threaded;
	};// end of Task class
#include "K15_Task.inl"
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_Task_h_