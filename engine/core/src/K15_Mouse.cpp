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
	int g_LastMousePos_x = 0;
	int g_LastMousePos_y = 0;
	float g_MouseWheelDelta = 0.0f;
	Mouse::InputStringToEnumMap Mouse::InputStringToEnum = Mouse::createButtonToEnumMap();
	Mouse::AxisStringToEnumMap Mouse::AxisStringToEnum = Mouse::createAxisToEnumMap();
	EventName Mouse::EventDoubleClicked = _EN(EventDoubleClicked);
	EventName Mouse::EventMouseMoved	= _EN(EventMouseMoved);
	EventName Mouse::EventMousePressed	= _EN(EventMousePressed);
	EventName Mouse::EventMouseReleased = _EN(EventMouseReleased);
	EventName Mouse::EventMouseWheel	= _EN(EventMouseWheel);
	/*********************************************************************************/

	/*********************************************************************************/
	Mouse::InputTrigger::InputTrigger(Enum p_Button)
		: m_Button(p_Button)
	{

	}
	/*********************************************************************************/
	float Mouse::InputTrigger::getValue()
	{
		return Mouse::isPressed(m_Button) ? 1.0f : 0.0f;
	}
	/*********************************************************************************/

	/*********************************************************************************/
	Mouse::AxisTrigger::AxisTrigger(Enum p_Axis)
		: m_Axis(p_Axis)
	{

	}
	/*********************************************************************************/
	float Mouse::AxisTrigger::getValue()
	{
		int32 x_delta = 0, y_delta = 0;
		float x_deltaNDC = 0.0f, y_deltaNDC = 0.0f;
		getMousePosDelta(&x_delta, &y_delta);
		x_deltaNDC = (float)x_delta / (float)RenderWindow::getWidth();
		y_deltaNDC = (float)y_delta / (float)RenderWindow::getHeight();

		if((m_Axis == MA_VERTICAL_POSITIVE && y_deltaNDC > 0.0f) ||
		   (m_Axis == MA_VERTICAL_NEGATIVE && y_deltaNDC < 0.0f))
		{
			return y_deltaNDC;
		}
		else if((m_Axis == MA_HORIZONTAL_POSITIVE && x_deltaNDC > 0.0f) ||
			    (m_Axis == MA_HORIZONTAL_NEGATIVE && x_deltaNDC < 0.0f))
		{
			return x_deltaNDC;
		}

		return 0.0f;
	}
	/*********************************************************************************/

	/*********************************************************************************/
	Mouse::WheelTrigger::WheelTrigger()
	{

	}
	/*********************************************************************************/
	float Mouse::WheelTrigger::getValue()
	{
		return Mouse::getMouseWheelDelta();
	}
	/*********************************************************************************/

	/*********************************************************************************/
	void Mouse::setMouseWheelDelta(float p_Delta)
	{
		g_MouseWheelDelta = p_Delta;
	}
	/*********************************************************************************/
	float Mouse::getMouseWheelDelta()
	{
		return g_MouseWheelDelta;
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
				*y = g_LastMousePos_y - temp_y;
			}
		}
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

		return map;
	}
	/*********************************************************************************/
	const Mouse::AxisStringToEnumMap& Mouse::createAxisToEnumMap()
	{
		static AxisStringToEnumMap map;

		map.insert(Pair(ObjectName,Enum)(_ON(Horizontal+), MA_HORIZONTAL_POSITIVE));
		map.insert(Pair(ObjectName,Enum)(_ON(Horizontal-), MA_HORIZONTAL_NEGATIVE));
		map.insert(Pair(ObjectName,Enum)(_ON(Vertical+), MA_VERTICAL_POSITIVE));
		map.insert(Pair(ObjectName,Enum)(_ON(Vertical-), MA_VERTICAL_NEGATIVE));

		return map;
	}
	/*********************************************************************************/
}}}/// end of K15_Engine::Core::InputDevice namespace