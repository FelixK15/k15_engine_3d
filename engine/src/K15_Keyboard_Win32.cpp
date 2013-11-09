/**
 * @file K15_Keyboard_Win32.cpp
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
 * 
 */

#include "K15_PrecompiledHeader.h"

#include "K15_Keyboard.h"

namespace K15_Engine { namespace Core { namespace InputDevices{
	/*********************************************************************************/
	bool Keyboard::isPressed(Enum p_Key)
	{
		return (GetKeyState(p_Key) & 0x80) != 0;
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Core::InputDevices