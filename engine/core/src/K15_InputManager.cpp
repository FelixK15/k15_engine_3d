/**
 * @file K15_InputManager.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/05
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

#include "K15_InputManager.h"
#include "K15_Mouse.h"
#include "K15_Keyboard.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	InputManager::InputManager()
		:  m_InputMap()
	{
	
	}
	/*********************************************************************************/
	InputManager::~InputManager()
	{
		for(InputMapping::iterator iter = m_InputMap.begin();iter != m_InputMap.end();++iter)
		{
			if(iter->second)
			{
				K15_DELETE iter->second;
			}
		}

		m_InputMap.clear();
	}
	/*********************************************************************************/
	void InputManager::addInputBinding(const ObjectName& p_ActionName,InputTriggerBase* p_Trigger)
	{
		InputMapping::iterator iter = m_InputMap.find(p_ActionName);

		if(iter == m_InputMap.end())
		{
			m_InputMap.insert(Pair(ObjectName,InputTriggerBase*)(p_ActionName,p_Trigger));
			return;
		}
	}
	/*********************************************************************************/
// 	void InputManager::addInputBinding(const ObjectName& p_ActonName,InputTriggerBase** p_Triggers,uint8 p_TriggerCount)
// 	{
// 		if(p_TriggerCount > MaxMapCount)
// 		{
// 			_LogWarning("Can't add %u trigger for action \"%s\". Clamping to %u.",p_TriggerCount,p_ActonName.c_str(),MaxMapCount);
// 			p_TriggerCount = MaxMapCount;
// 		}
// 
// 		InputMapping::iterator iter = m_InputMap.find(p_ActonName);
// 
// 		if(iter == m_InputMap.end())
// 		{
// 			InputTriggerBase* trigger[MaxMapCount];
// 			memset(trigger,0,sizeof(InputTriggerBase) * MaxMapCount);
// 			for(uint8 i = 0;i < p_TriggerCount;++i)
// 			{
// 				trigger[i] = p_Triggers[i];
// 			}
// 
// 			m_InputMap.insert(Pair(ObjectName,InputTriggerBase*[MaxMapCount])(p_ActonName,trigger));
// 
// 			return;
// 		}
// 		else
// 		{
// 			uint8 counter = 0;
// 			for(uint8 i = 0;i < MaxMapCount;++i)
// 			{
// 				if(counter == p_TriggerCount)
// 				{
// 					_LogError("Can't add another trigger for action \"%s\".",p_ActonName.c_str());
// 					return;
// 				}
// 
// 				if(iter->second[i] == 0)
// 				{
// 					iter->second[i] = p_Triggers[counter++];
// 				}
// 			}
// 		}
// 	}
	/*********************************************************************************/
	void InputManager::removeInputBinding(const ObjectName& p_ActionName)
	{
		InputMapping::iterator iter = m_InputMap.find(p_ActionName);

		if(iter != m_InputMap.end())
		{
			if(iter->second)
			{
				K15_DELETE iter->second;
				iter->second = 0;
			}
		}
	}
	/*********************************************************************************/
	float InputManager::getValue(const ObjectName& p_ActionName)
	{
		InputMapping::iterator iter = m_InputMap.find(p_ActionName);
		bool active = false;

		if(iter != m_InputMap.end())
		{
			if(iter->second)
			{
				return iter->second->getValue();;
			}
		}

		return 0.0f;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace