/**
 * @file K15_EventManager.h
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
 * The EventManager class is responsible for handling events.
 * It'll hold a list of all EventListener implementations that listens to certain events.
 *
 * Once per frame the EventManager will go through all events that occurred during the frame 
 * to call the EventListener listening to that specific event.
 *
 * EventListener can also register and unregister themselves as listener for specific events.
 */
#pragma once

#ifndef __K15_EVENTMANAGER__
#define __K15_EVENTMANAGER__

#include "K15_StdInclude.h"
#include "K15_Event.h"
#include "K15_EventListener.h"

#include "K15_List.h"
#include "K15_Stack.h"

#include "K15_System.h"
#include "K15_HashMap.h"

//The EventManager object can get obtained using this macro.
//Example:	g_pEventManager->Update();
#define g_pEventManager EventManager::GetInstance()

namespace K15_EngineV2
{
	typedef HashMap<EventType,List<EventListener*>* > EventTypeListenerMap;
	typedef Stack<Event> EventStack;

	class K15ENGINE2_API EventManager : public Singleton<EventManager>
	{
	public:
		/**
		* Standard Constructor
		*/
		EventManager();

		/**
		* Standard Destructor
		*/
		~EventManager();

		/**
		* You can add a new EventListener that listens to Event objects of type EventType.
		*
		* @param  etType - type of the Event that the listener shall listen to.
		* @param  pListener - pointer to EventListener implementation.
		*/
		void AddListener(const EventType &etType,EventListener *pListener);
		/**
		* You can remove a previously added EventListener using this function.
		*
		* @param  etType - type of the Event that the listener should stop listen to.
		* @param  pListener - pointer to a previously added (AddListener()) EventListener.
		*/
		void RemoveListener(const EventType &etType,EventListener *pListener);

		/**
		* If you want to add a new Event to the event queue.
		* The queue gets processed each frame.
		*
		* @param  evGameEvent - reference to an Event object.
		*/
		void AddEventToQueue(const Event& evGameEvent);
		/**
		* If you want to trigger an Event immediately use this function
		* instead of AddEventToQueue.
		*
		* @param  evGameEvent - reference to an Event object. 
		*/
		void TriggerEvent(const Event& evGameEvent);

		/**
		* This function will get called once per Frame to process all events in the queue.
		* @note This function will be called internally. There's no reason for you 
		*	    to call this function.
		*/
		void Update();

	private:
		EventStack m_sEvents;				//Event Queue.
		EventTypeListenerMap m_hmListener;	//Map where the EventTypes are associated with EventListeners.
	};
}

#endif