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

#include "K15_EventManager.h"

namespace K15_Engine { namespace System { 
	/*********************************************************************************/
	EventManager::EventManager()
		: PageAllocator<Application>(Application::getInstance())
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
		List<EventListener*> *pList = 0;

		if(!m_hmListener.HasItem(etType)){
			pList = new List<EventListener*>();
			m_hmListener.Insert(etType,pList);
		}else{
			pList = m_hmListener.Get(etType)->GetValue();
		}

		pList->PushBack(pListener);
	}
	/*********************************************************************************/
	void EventManager::removeListener(const EventName& p_EventName,EventListener* p_Listener)
	{
		if(m_hmListener.HasItem(etType)){
			List<EventListener*> *pList = m_hmListener.Get(etType)->GetValue();
			for(ListNode<EventListener*> *pNode = pList->GetFirstElement();pNode;pNode = pNode->GetNext()){
				EventListener *pCurrentListener = pNode->GetElement();

				if(pCurrentListener == pListener){
					pList->Erase(pNode);
					break;
				}
			}
		}
	}
	/*********************************************************************************/
	void EventManager::addEventToQueue(const GameEvent& p_Event)
	{
		m_sEvents.Push(evGameEvent);
	}
	/*********************************************************************************/
	void EventManager::triggerEvent(const GameEvent& p_Event)
	{
		if(m_hmListener.HasItem(evGameEvent.GetType())){
			List<EventListener*> *pList = m_hmListener.Get(evGameEvent.GetType())->GetValue();
			for(ListNode<EventListener*> *pNode = pList->GetFirstElement();pNode;pNode = pNode->GetNext()){
				EventListener *pCurrentListener = pNode->GetElement();
				pCurrentListener->HandleEvent(evGameEvent);
			}
		}
	}
	/*********************************************************************************/
	void EventManager::update()
	{
		double start = g_pSystem->TimeSinceStart();
		double diff = 0;

		if(m_sEvents.Size() > 0){
			GameEvent &gameEvent = m_sEvents.Top();
			triggerEvent(gameEvent);
			m_sEvents.Pop();
		}

		diff = g_pSystem->TimeSinceStart() - start;
	}
	/*********************************************************************************/
}}// end of K15_Engine::System namespace