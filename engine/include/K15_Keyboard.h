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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_EventListener.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_InputTriggerBase.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	class K15_CORE_API Keyboard
	{
	public:
		/*********************************************************************************/
		class K15_CORE_API InputTrigger : public InputTriggerBase
		{
		public:
			InputTrigger(Enum p_Key);

			virtual bool isActive() OVERRIDE;
		private:
			Enum m_Key;
		};// end of K15_Engine::InputTrigger class declaraton
		/*********************************************************************************/

		/*********************************************************************************/
		class K15_CORE_API Listener : public EventListener
		{
		public:
			virtual ~Listener();

			virtual void onKeyPress(Enum p_Key){}
			virtual void onKeyRelease(Enum p_Key){}

			virtual void handleEvent(GameEvent* p_Event) OVERRIDE;
		protected:
			Listener();
		};// end of Keyboard::Listener class declaration
		/*********************************************************************************/
		enum eKeyboardKeys
		{
			KEY_1 = 0x61,
			KEY_2 = 0x62,
			KEY_3 = 0x63,
			KEY_4 = 0x64,
			KEY_5 = 0x65,
			KEY_6 = 0x66,
			KEY_7 = 0x67,
			KEY_8 = 0x68,
			KEY_9 = 0x69,
			KEY_0 = 0x60,
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

			KEY_A = 'a',
			KEY_B = 'b',
			KEY_C = 'c',
			KEY_D = 'd',
			KEY_E = 'e',
			KEY_F = 'f',
			KEY_G = 'g',
			KEY_H = 'h',
			KEY_I = 'i',
			KEY_J = 'j',
			KEY_K = 'k',
			KEY_L = 'l',
			KEY_M = 'm',
			KEY_N = 'n',
			KEY_O = 'o',
			KEY_P = 'p',
			KEY_Q = 'q',
			KEY_R = 'r',
			KEY_S = 's',
			KEY_T = 't',
			KEY_U = 'u',
			KEY_V = 'v',
			KEY_W = 'w',
			KEY_X = 'x',
			KEY_Y = 'y',
			KEY_Z = 'z',
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
	};// end of Keybard class declaration

}}} //end of K15_Engine::Core::InputDevices namespace

#endif //_K15Engine_Core_InputDevice_Keyboard_h_