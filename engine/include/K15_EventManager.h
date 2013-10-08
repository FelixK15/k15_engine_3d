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
#ifndef _K15Engine_System_EventManager_h_
#define _K15Engine_System_EventManager_h_

#include "K15_Prerequisites.h"
#include "K15_Application.h"
#include "K15_AllocatedObject.h"
#include "K15_PageAllocator.h"
#include "K15_Singleton.h"

namespace K15_Engine { namespace System { 
	/*********************************************************************************/
  typedef K15_List(EventListener*) EventListenerList;
	typedef K15_HashMap(EventName,EventListenerList*) EventTypeListenerMap;
	typedef K15_Stack(GameEvent*) EventStack;
	/*********************************************************************************/
	class K15_API_EXPORT EventManager : public ApplicationAllocatedObject, public Singleton<EventManager>, public PageAllocator<>
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
		void addListener(const EventName& p_EventName,EventListener* p_Listener);
		/**
		* You can remove a previously added EventListener using this function.
		*
		* @param  etType - type of the Event that the listener should stop listen to.
		* @param  pListener - pointer to a previously added (AddListener()) EventListener.
		*/
		void removeListener(const EventName& p_EventName,EventListener* p_Listener);

		/**
		* If you want to add a new Event to the event queue.
		* The queue gets processed each frame.
		*
		* @param  evGameEvent - reference to an Event object.
		*/
		void addEventToQueue(GameEvent* p_Event);
		/**
		* If you want to trigger an Event immediately use this function
		* instead of addEventToQueue.
		*
		* @param  evGameEvent - reference to an Event object. 
		*/
		void triggerEvent(GameEvent* p_Event);

		/**
		* This function will get called once per Frame to process all events in the queue.
		* @note This function will be called internally. There's no reason for you 
		*	    to call this function.
		*/
		void update();

	private:
		EventStack m_Events; //Event Queue.
		EventTypeListenerMap m_Listener; //Map where the EventTypes are associated with EventListeners.
	};//end of EventManager class
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_EventManager_h_