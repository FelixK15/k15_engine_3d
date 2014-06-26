/**
 * @file K15_RenderWindowBase.h
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

#ifndef _K15Engine_Core_RenderWindow_h_
#define _K15Engine_Core_RenderWindow_h_

#include "K15_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_RenderWindow_Win32.h"
#elif defined K15_OS_LINUX
    #include "linux/K15_RenderWindow_Linux.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	#ifdef K15_OS_WINDOWS
		typedef RenderWindow_Win32 RenderWindowImpl;
    #elif defined K15_OS_LINUX
        typedef RenderWindow_Linux RenderWindowImpl;
	#endif //K15_OS_WINDOWS
	/*********************************************************************************/

	/*********************************************************************************/
	struct Resolution
	{
		uint32 width;
		uint32 height;

		INLINE float getAspectRatio() const;
	};
	/*********************************************************************************/
	class RenderWindow
	{
	public:
		static EventName EventFocusLost;
		static EventName EventFocusReceived;
		static EventName EventResolutionChanged;
		static EventName EventInitialized;

	public:
		static bool initialize();
		static void shutdown();

		static void setWindowTitle(const String& p_WindowTitle);
		INLINE static const String& getWindowTitle();

		static void setResolution(const Resolution& p_Resolution);
		INLINE static void setResolution(uint32 width, uint32 height);
		INLINE static const Resolution& getResolution();

		static void setIsFullscreen(bool p_Fullscreen);
		INLINE static bool isFullscreen();

		static void setHasFocus(bool p_HasFocus);
		INLINE static bool getHasFocus();

		INLINE static uint32 getWidth();
		INLINE static uint32 getHeight();
		INLINE static float getAspectRatio();

	protected:
		static String ms_WindowTitle;
		static Resolution ms_CurrentResolution;
		static bool ms_IsInitialized;
		static bool ms_IsFullscreen;
		static bool ms_HasFocus;
	};// end of RenderWindowBase class
	#include "K15_RenderWindow.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_RenderWindow_h_
