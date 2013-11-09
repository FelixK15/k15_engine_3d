/**
 * @file K15_Mouse.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_PrecompiledHeader.h"

#include "K15_Mouse.h"
#include "K15_GameEvent.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	/*********************************************************************************/
	int g_LastMousePos_x = 0;
	int g_LastMousePos_y = 0;
	/*********************************************************************************/
	Mouse::InputTrigger::InputTrigger(Enum p_Button)
		: m_Button(p_Button)
	{

	}
	/*********************************************************************************/
	bool Mouse::InputTrigger::isActive()
	{
		return Mouse::isPressed(m_Button);
	}
	/*********************************************************************************/

	/*********************************************************************************/
	Mouse::Listener::Listener()
		: EventListener()
	{
		g_EventManager->addListener(_EN(onMousePressed),this);
		g_EventManager->addListener(_EN(onMouseReleased),this);
		g_EventManager->addListener(_EN(onMouseDoubleClicked),this);
		g_EventManager->addListener(_EN(onMouseMoved),this);
		g_EventManager->addListener(_EN(onMouseWheel),this);
	}
	/*********************************************************************************/
	Mouse::Listener::~Listener()
	{
		g_EventManager->removeListener(_EN(onMousePressed),this);
		g_EventManager->removeListener(_EN(onMouseReleased),this);
		g_EventManager->removeListener(_EN(onMouseDoubleClicked),this);
		g_EventManager->removeListener(_EN(onMouseMoved),this);
		g_EventManager->removeListener(_EN(onMouseWheel),this);
	}
	/*********************************************************************************/
	void Mouse::Listener::handleEvent(GameEvent* p_Event)
	{
		if(p_Event->getName() == _EN(onMousePressed))
		{
			uint32* argument = (uint32*)p_Event->getArgument();
			onMousePressed(argument[0],argument[1],argument[2]);
		}
		else if(p_Event->getName() == _EN(onMouseReleased))
		{
			uint32* argument = (uint32*)p_Event->getArgument();
			onMouseReleased(argument[0],argument[1],argument[2]);
		}
		else if(p_Event->getName() == _EN(onMouseDoubleClicked))
		{
			uint32* argument = (uint32*)p_Event->getArgument();
			onMouseDoubleClicked(argument[0],argument[1],argument[2]);
		}
		else if(p_Event->getName() == _EN(onMouseMoved))
		{
			uint32* argument = (uint32*)p_Event->getArgument();
			onMouseMoved(argument[0],argument[1]);
		}
		else if(p_Event->getName() == _EN(onMouseWheel))
		{
			uint32* argument = (uint32*)p_Event->getArgument();
			onMouseWheel(argument[0],argument[1],argument[2]);
		}
	}
	/*********************************************************************************/
	void Mouse::getMousePosDelta(int32 *x,int32 *y)
	{
		static int temp_x;
		static int temp_y;

		getMousePos(&temp_x,&temp_y);
		if(x)
		{
			if(temp_x > g_LastMousePos_x)
			{
				*x = temp_x - g_LastMousePos_x;
			}
			else
			{
				*x = g_LastMousePos_x - temp_x;
			}
		}

		if(y)
		{
			if(temp_y > g_LastMousePos_y)
			{
				*y = temp_y - g_LastMousePos_y;
			}
			else
			{
				*x = g_LastMousePos_y - temp_y;
			}
		}
	}
	/*********************************************************************************/
}}}/// end of K15_Engine::Core::InputDevice namespace