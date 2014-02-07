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
 *
 * @section DESCRIPTION
 *
 */

#ifndef _K15Engine_Core_EventTask_h_
#define _K15Engine_Core_EventTask_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_TaskBase.h"

namespace K15_Engine { namespace Core {
	class EventTask : public TaskBase
	{
	/*********************************************************************************/
	K15_DECLARE_RTTI;
	/*********************************************************************************/
	public:
		/*********************************************************************************/
		static const uint32 TaskPriority;
		/*********************************************************************************/
	public:
		/**
		* Standard Constructor
		*/
		EventTask();
		
		/**
		* Standard Destructor
		*/
		virtual ~EventTask();
		
		/**
		* update method (events will get processed)
		*
		* @param p_GameTime 		
		*/
		virtual void update(const GameTime& p_GameTime);

	protected:
		EventManager* m_EventManager;
	}; // end of EventTask class declaration
}}// end of K15_Engine::Core namespace
#endif //_K15Engine_Core_EventTask_h_