/**
 * @file K15_EventManager.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/11
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

#include "K15_EventManager.h"
#include "K15_EventListener.h"
#include "K15_GameEvent.h"
#include "K15_TaskBase.h"
#include "K15_GameTime.h"
#include "K15_ProfilingManager.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	EventManager::EventManager()
		: AllocatedObject(),
		  PageAllocator(ApplicationAllocator)
	{

	}
	/*********************************************************************************/
	EventManager::~EventManager()
	{
		m_Listener.clear();
	}
	/*********************************************************************************/
	void EventManager::addListener(const EventName& p_EventName,EventListener* p_Listener)
	{
		/* If there are already listener for the EventType, just add the new listener
		 * to an EventListener list. If there hasn't been any listeners for this
		 * EventType, just add a new entry to the map.	
		 */
		List(EventListener*) *list = 0;

		EventTypeListenerMap::iterator iter = m_Listener.find(p_EventName);
		if(iter == m_Listener.end())
		{
#if defined (K15_DEBUG)
		list = new(MemoryAllocator->allocateDebug(sizeof(EventListenerList),__FILE__,__LINE__,false,__FUNCTION__)) EventListenerList();
#else
		list = new(MemoryAllocator->allocate(sizeof(EventListenerList))) EventListenerList();
#endif
		m_Listener.insert(Pair(EventName,EventListenerList*)(p_EventName,list));
		}
		else
		{
			list = iter->second;
		}

		list->push_back(p_Listener);
	}
	/*********************************************************************************/
	void EventManager::removeListener(const EventName& p_EventName,EventListener* p_Listener)
	{
    EventTypeListenerMap::iterator iter = m_Listener.find(p_EventName);
    if(iter != m_Listener.end())
    {
      EventListenerList* list = iter->second;
      EventListenerList::iterator list_iter = list->begin();

      for(list_iter;list_iter != list->end();++list_iter)
      {
        EventListener* currentListener = *list_iter;

        if(currentListener == p_Listener){
          list->remove(p_Listener);
          break;
        }
      }
    }
	}
	/*********************************************************************************/
	void EventManager::addEventToQueue(GameEvent* p_Event)
	{
		m_Events.push(p_Event);
	}
	/*********************************************************************************/
	void EventManager::triggerEvent(GameEvent* p_Event)
	{
		EventTypeListenerMap::iterator iter = m_Listener.find(p_Event->getName());
		if(iter != m_Listener.end())
		{
			EventListenerList* list = iter->second;
			for(EventListenerList::iterator list_iter = list->begin();list_iter != list->end();++list_iter)
			{
				EventListener *currentListener = *list_iter;
				currentListener->handleEvent(p_Event);
			}
		}

    //delete event after it has been processed
    K15_DELETE p_Event;
	}
	/*********************************************************************************/
	void EventManager::update()
	{
		K15_PROFILE(EventManager_Update);

		if(m_Events.size() > 0)
		{
			GameEvent* gameEvent = m_Events.top();
			triggerEvent(gameEvent);
			m_Events.pop();
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace