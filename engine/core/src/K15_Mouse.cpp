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
#include "K15_RenderWindow.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	/*********************************************************************************/
	namespace internal
	{
		int lastMousePos_x = 0;
		int lastMousePos_y = 0;
		float mouseWheelDelta = 0.0f;
		bool mouseLocked = false;
	}
	/*********************************************************************************/
	Mouse::InputStringToEnumMap Mouse::InputStringToEnum = Mouse::createButtonToEnumMap();
	EventName Mouse::EventDoubleClicked = _EN(EventDoubleClicked);
	EventName Mouse::EventMouseMoved	= _EN(EventMouseMoved);
	EventName Mouse::EventMousePressed	= _EN(EventMousePressed);
	EventName Mouse::EventMouseReleased = _EN(EventMouseReleased);
	EventName Mouse::EventMouseWheel	= _EN(EventMouseWheel);
	/*********************************************************************************/

	/*********************************************************************************/
	void Mouse::setMouseWheelDelta(float p_Delta)
	{
		internal::mouseWheelDelta = p_Delta;
	}
	/*********************************************************************************/
	float Mouse::getMouseWheelDelta()
	{
		return internal::mouseWheelDelta;
	}
	/*********************************************************************************/
	void Mouse::getMousePosDelta(int32 *x,int32 *y)
	{
		static int temp_x;
		static int temp_y;

		getMousePos(&temp_x,&temp_y);
		if(x)
		{
			if(temp_x > internal::lastMousePos_x)
			{
				*x = temp_x - internal::lastMousePos_x;
			}
			else
			{
				*x = internal::lastMousePos_x - temp_x;
			}
		}

		if(y)
		{
			if(temp_y > internal::lastMousePos_y)
			{
				*y = temp_y - internal::lastMousePos_y;
			}
			else
			{
				*y = internal::lastMousePos_y - temp_y;
			}
		}
	}
	/*********************************************************************************/
	void Mouse::lockMouse(bool p_MouseLocked)
	{
		internal::mouseLocked = p_MouseLocked;
	}
	/*********************************************************************************/
	bool Mouse::isMouseLocked()
	{
		return internal::mouseLocked;
	}
	/*********************************************************************************/
	const Mouse::InputStringToEnumMap& Mouse::createButtonToEnumMap()
	{
		static InputStringToEnumMap map;

		map.insert(Pair(ObjectName,Enum)(_ON(LeftButton), BTN_LEFT));
		map.insert(Pair(ObjectName,Enum)(_ON(RightButton), BTN_RIGHT));
		map.insert(Pair(ObjectName,Enum)(_ON(MiddleButton), BTN_MIDDLE));
		map.insert(Pair(ObjectName,Enum)(_ON(SpecialButton1), BTN_SPECIAL1));
		map.insert(Pair(ObjectName,Enum)(_ON(SpecialButton2), BTN_SPECIAL2));
		map.insert(Pair(ObjectName,Enum)(_ON(Horizontal), MA_HORIZONTAL));
		map.insert(Pair(ObjectName,Enum)(_ON(Vertical), MA_VERTICAL));

		return map;
	}
	/*********************************************************************************/
}}}/// end of K15_Engine::Core::InputDevice namespace