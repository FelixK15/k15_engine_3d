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
  EventHandlerArrayEntry::EventHandlerArrayEntry(const EventName& p_EventName)
    : eventName(p_EventName),
    handlerList()
  {

  }
  /*********************************************************************************/

	/*********************************************************************************/
	EventManager::EventManager()
		: AllocatedObject()
	{

	}
	/*********************************************************************************/
	EventManager::~EventManager()
	{
		for(EventHandlerArray::iterator iter = m_Listener.begin();iter != m_Listener.end();++iter)
		{
			K15_DELETE_SIZE(Allocators[AC_GAMEVENTS],iter->second,sizeof(EventHandlerList));
		}

		while(m_Events.size() > 0)
		{
			K15_DELETE m_Events.top();
			m_Events.pop();
		}

		m_Listener.clear();
	}
	/*********************************************************************************/
	void EventManager::addHandler(const EventName& p_EventName, const EventHandler& p_Handler)
	{
		/* If there are already listener for the EventType, just add the new listener
		 * to an EventListener list. If there hasn't been any listeners for this
		 * EventType, just add a new entry to the map.	
		 */
		EventHandlerArray::iterator iter = Find(m_Listener.begin(), m_Listener.end(), p_EventName);

    if(iter == m_Listener.end())
    {
      m_Listener.push_back(EventHandlerArrayEntry(p_EventName));
      addHandler(p_EventName, p_Handler);
    }

    iter->handlerList.push_back(p_Handler);
	}
	/*********************************************************************************/
	void EventManager::removeListener(const EventName& p_EventName, const EventHandler& p_Handler)
	{
    EventHandlerArray::iterator iter = Find(m_Listener.begin(), m_Listener.end(), p_EventName);

    if(iter == m_Listener.end())
    {
      _LogError("No handler for event \"%s\".",p_EventName.c_str());
      return;
    }

    EventHandlerList& handlers = iter->handlerList;

    for(EventHandlerList::iterator handlerIter = handlers.begin(); handlerIter != handlers.end(); ++handlerIter)
    {
      if(p_Handler == (*handlerIter))
      {
        handlers.erase(handlerIter);
        break;
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
    K15_ASSERT(p_Event,"Event is NULL!");

		EventHandlerArray::iterator iter = Find(m_Listener.begin(), m_Listener.end(), p_Event->getName());

    EventHandlerList& handlers = iter->handlerList;

    for(EventHandlerList::iterator handlerIter = handlers.begin(); handlerIter != handlers.end(); ++handlerIter)
    {
      (*handlerIter)(p_Event);
    }

		//delete event after it has been processed
		K15_DELETE p_Event;
	}
	/*********************************************************************************/
	void EventManager::update()
	{
		K15_PROFILE_BLOCK("EventManager::update",
			if(m_Events.size() > 0)
			{
				GameEvent* gameEvent = m_Events.top();
				triggerEvent(gameEvent);
				m_Events.pop();
			}
		);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace