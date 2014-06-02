/**
 * @file K15_Mouse_Android.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#ifdef K15_OS_ANDROID

#include "K15_Mouse.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	/*********************************************************************************/
	void Mouse::hideMouse()
	{
		
	}
	/*********************************************************************************/
	void Mouse::showMouse()
	{
		
	}
	/*********************************************************************************/
	void Mouse::setMousePos(int32 x, int32 y)
	{
		
	}
	/*********************************************************************************/
	void Mouse::getMousePos(int32 *x,int32 *y)
	{

	}
	/*********************************************************************************/
	bool Mouse::setMouseIcon(byte* p_IconData, int32 p_Width, int32 p_Height, uint32 p_ColorBits)
	{
		return false;
	}
	/*********************************************************************************/
	bool Mouse::setMouseIcon(const String& p_IconPath)
	{
		return false;
	}
	/*********************************************************************************/
	bool Mouse::isPressed(Enum p_Button)
	{
		return false;
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Core::InputDevices namespace

#endif //K15_OS_ANDROID