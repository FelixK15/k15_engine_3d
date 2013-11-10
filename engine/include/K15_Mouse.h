/**
 * @file K15_Mouse.h
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

#ifndef _K15Engine_Core_InputDevice_Mouse_h_
#define _K15Engine_Core_InputDevice_Mouse_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_InputTriggerBase.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	class K15_CORE_API Mouse
	{
	public:
		/*********************************************************************************/
		class K15_CORE_API InputTrigger : public InputTriggerBase
		{
		public:
			InputTrigger(Enum p_Button);

			virtual bool isActive() OVERRIDE;
		private:
			Enum m_Button;
		};// end of Mouse::InputTrigger class declaration
		/*********************************************************************************/
		class K15_CORE_API Listener : public EventListener
		{
		public:
			virtual ~Listener();

			virtual void onMousePressed(Enum p_Button, int32 x, int32 y){}
			virtual void onMouseReleased(Enum p_Button, int32 x, int32 y){}
			virtual void onMouseDoubleClicked(Enum p_Button, int32 x, int32 y){}
			virtual void onMouseMoved(int32 x, int32 y){}
			virtual void onMouseWheel(int32 p_Delta, int32 x, int32 y){}

			virtual void handleEvent(GameEvent* p_Event) OVERRIDE;
		protected:
			Listener();
		};// end of Mouse::Listener class declaration
		/*********************************************************************************/
		enum eButton
		{
			BTN_LEFT,		//<! Left Mouse Button
			BTN_RIGHT,		//<! Right Mouse Button
			BTN_MIDDLE,		//<! Middle Mouse Button

			BTN_SPECIAL1,	//<! Special Mouse Button 1 (To support mice with more than 3 buttons)
			BTN_SPECIAL2,	//<! Special Mouse Button 2 (To support mice with more than 3 buttons)
		};//Button
		/*********************************************************************************/

	public:
		static void hideMouse();
		static void showMouse();

		static void setMousePos(int32 x, int32 y);
		static void getMousePos(int32 *x,int32 *y);

		static void getMousePosDelta(int32 *x,int32 *y);

		static bool setMouseIcon(byte* p_IconData, int32 p_Width = 32, int32 p_Height = 32, uint32 p_ColorBits = 32);
		static bool setMouseIcon(const String& p_IconPath);

		static bool isPressed(Enum p_Button);
	};// end of Mouse class declaration
}}}// end of K15_Engine::Core::InputDevice namespace

#endif //_K15Engine_Core_InputDevice_Mouse_h_