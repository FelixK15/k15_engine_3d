/**
 * @file K15_RenderWindow_Win32.h
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

#ifndef _K15Engine_Core_RenderWindow_Win32_h_
#define _K15Engine_Core_RenderWindow_Win32_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core {
	class RenderWindow_Win32
	{
	public:
		static bool initialize();
		static void shutdown();

		static void setWindowTitle(const String& p_WindowTitle);
		static void setResolution(const Resolution& p_Resolution); 
		static void setIsFullscreen(bool p_Fullscreen);

		static HDC getDeviceContext();
		static HWND getHandleWindow();
		static HINSTANCE getHandleInstance();

	private:
		static HDC ms_DeviceContext;
		static HWND ms_HandleWindow;
		static HINSTANCE ms_Instance;
	};// end of RenderWindow_Win32 class declaration
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_RenderWindow_Win32_h_