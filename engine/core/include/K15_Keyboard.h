/**
 * @file K15_Keyboard.h
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

#ifndef _K15Engine_Core_InputDevice_Keyboard_h_
#define _K15Engine_Core_InputDevice_Keyboard_h_

#include "K15_Prerequisites.h"
#include "K15_InputTriggerBase.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	class K15_CORE_API Keyboard
	{
	public:
		/*********************************************************************************/
		typedef HashMap(ObjectName,Enum) InputStringToEnumMap;
		static InputStringToEnumMap InputStringToEnum;
		/*********************************************************************************/
		class K15_CORE_API InputTrigger : public InputTriggerBase
		{
		public:
			InputTrigger(Enum p_Key);

			virtual float getValue() OVERRIDE;
		private:
			Enum m_Key;
		};// end of K15_Engine::InputTrigger class declaraton
		/*********************************************************************************/
		static EventName EventKeyPress;
		static EventName EventKeyRelease;
		/*********************************************************************************/
		enum eKeyboardKeys
		{
			//numpad
			KEY_NUMPAD_1 = 0x61,
			KEY_NUMPAD_2 = 0x62,
			KEY_NUMPAD_3 = 0x63,
			KEY_NUMPAD_4 = 0x64,
			KEY_NUMPAD_5 = 0x65,
			KEY_NUMPAD_6 = 0x66,
			KEY_NUMPAD_7 = 0x67,
			KEY_NUMPAD_8 = 0x68,
			KEY_NUMPAD_9 = 0x69,
			KEY_NUMPAD_0 = 0x60,
			//numbers
			KEY_1 = '1',
			KEY_2 = '2',
			KEY_3 = '3',
			KEY_4 = '4',
			KEY_5 = '5',
			KEY_6 = '6',
			KEY_7 = '7',
			KEY_8 = '8',
			KEY_9 = '9',
			KEY_0 = '0',

			KEY_BACKSPACE = 0x08,
			KEY_L_SHIFT = 0xA0,
			KEY_R_SHIFT = 0xA1,
			KEY_CAPSLOCK = 0x14,
			KEY_TABULATOR = 0x09,
			KEY_SPACE = 0x20,
			KEY_L_ALT,
			KEY_R_ALT,
			KEY_CTRL = 0x11,
			KEY_RETURN = 0x0D,
			KEY_ESCAPE = 0x1B,
			//letters
			KEY_A = 0x41,
			KEY_B = 0x42,
			KEY_C = 0x43,
			KEY_D = 0x44,
			KEY_E = 0x45,
			KEY_F = 0x46,
			KEY_G = 0x47,
			KEY_H = 0x48,
			KEY_I = 0x49,
			KEY_J = 0x4A,
			KEY_K = 0x4B,
			KEY_L = 0x4C,
			KEY_M = 0x4D,
			KEY_N = 0x4E,
			KEY_O = 0x4F,
			KEY_P = 0x50,
			KEY_Q = 0x51,
			KEY_R = 0x52,
			KEY_S = 0x53,
			KEY_T = 0x54,
			KEY_U = 0x55,
			KEY_V = 0x56,
			KEY_W = 0x57,
			KEY_X = 0x58,
			KEY_Y = 0x59,
			KEY_Z = 0x5A,
			//f keys
			KEY_F1 = 0x70,
			KEY_F2 = 0x71,
			KEY_F3 = 0x72,
			KEY_F4 = 0x73,
			KEY_F5 = 0x74,
			KEY_F6 = 0x75,
			KEY_F7 = 0x76,
			KEY_F8 = 0x77,
			KEY_F9 = 0x78,
			KEY_F10 = 0x79,
			KEY_F11 = 0x7A,
			KEY_F12 = 0x7B,
			//arrow keys
			KEY_UP = 0x26,
			KEY_DOWN = 0x28,
			KEY_LEFT = 0x25,
			KEY_RIGHT = 0x27
		};//KeyboardKeys
		/*********************************************************************************/
	public:
		static bool isPressed(Enum p_Key);

	private:
		static const InputStringToEnumMap& createStringToEnumMap();
	};// end of Keybard class declaration

}}} //end of K15_Engine::Core::InputDevices namespace

#endif //_K15Engine_Core_InputDevice_Keyboard_h_