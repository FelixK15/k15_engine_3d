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

#ifndef _K15Engine_Core_Mouse_h_
#define _K15Engine_Core_Mouse_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core { namespace InputDevice {
	class K15_CORE_API Mouse
	{
	public:
		/*********************************************************************************/
		class Listener : public EventListener
		{
		public:
			virtual ~Listener();

			virtual void onMouseClicked(Enum p_Button, int32 x, int32 y) = 0;
			virtual void onMouseDoubleClicked(Enum p_Button, int32 x, int32 y) = 0;
			virtual void onMouseMoved(int32 x, int32 y) = 0;
			virtual void onMouseWheel(int32 p_Delta, int32 x, int32 y) = 0;
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
			BTN_SPECIAL3,	//<! Special Mouse Button 3 (To support mice with more than 3 buttons)
			BTN_SPECIAL4	//<! Special Mouse Button 4 (To support mice with more than 3 buttons)
		};//Button
		/*********************************************************************************/

	public:
		static void setMousePos(int32 x, int32 y);
		static void getMousePos(int32 *x,int32 *y);

		static void setMouseIcon(Rendering::Texture* p_IconTexture);
		static void setMouseIcon(byte* p_IconData);
		static void setMouseIcon(const String& p_IconPath);

		static bool isPressed(Enum p_Button);
	};// end of Mouse class declaration

}}}// end of K15_Engine::Core::InputDevice namespace

#endif //_K15Engine_Core_Mouse_h_