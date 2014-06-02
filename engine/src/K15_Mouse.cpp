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
	Mouse::InputStringToEnumMap Mouse::InputStringToEnum = Mouse::createStringToEnumMap();
	EventName Mouse::EventDoubleClicked = _EN(EventDoubleClicked);
	EventName Mouse::EventMouseMoved	= _EN(EventMouseMoved);
	EventName Mouse::EventMousePressed	= _EN(EventMousePressed);
	EventName Mouse::EventMouseReleased = _EN(EventMouseReleased);
	EventName Mouse::EventMouseWheel	= _EN(EventMouseWheel);
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
				*y = g_LastMousePos_y - temp_y;
			}
		}
	}
	/*********************************************************************************/
	const Mouse::InputStringToEnumMap& Mouse::createStringToEnumMap()
	{
		static InputStringToEnumMap map;

		map.insert(Pair(ObjectName,Enum)(_ON(LeftButton),BTN_LEFT));
		map.insert(Pair(ObjectName,Enum)(_ON(RightButton),BTN_RIGHT));
		map.insert(Pair(ObjectName,Enum)(_ON(MiddleButton),BTN_MIDDLE));
		map.insert(Pair(ObjectName,Enum)(_ON(SpecialButton1),BTN_SPECIAL1));
		map.insert(Pair(ObjectName,Enum)(_ON(SpecialButton2),BTN_SPECIAL2));

		return map;
	}
	/*********************************************************************************/
}}}/// end of K15_Engine::Core::InputDevice namespace