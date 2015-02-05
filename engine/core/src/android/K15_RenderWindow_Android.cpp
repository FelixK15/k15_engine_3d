/**
 * @file K15_RenderWindow_Android.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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

#include "Android\K15_RenderWindow_Android.h"
#include "Android\K15_OSLayer_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	RenderWindow_Android::RenderWindow_Android()
		: RenderWindow(),
		m_Window(0)
	{
		
	}
	/*********************************************************************************/
	RenderWindow_Android::~RenderWindow_Android()
	{
		
	}
	/*********************************************************************************/
	bool RenderWindow_Android::initialize()
	{
		if((m_Window = ((OSLayer)g_Application->getOSLayer()).getAndroidApp()->window) == 0)
		{
			K15_LOG_ERROR_MESSAGE("android_app::window is NULL");
			return false;
		}
		else
		{
			K15_LOG_SUCCESS_MESSAGE("Successfully retrieved android_app::window.");
		}
		return true;
	}
	/*********************************************************************************/
	void RenderWindow_Android::shutdown()
	{
		
	}
	/*********************************************************************************/
	void RenderWindow_Android::setWindowTitle(const String& p_WindowTitle)
	{

	}
	/*********************************************************************************/
	void RenderWindow_Android::setResolution(const Resolution& p_Resolution, bool p_ForceChange)
	{
		RenderWindow::setResolution(p_Resolution,p_ForceChange);
	}
	/*********************************************************************************/
	void RenderWindow_Android::setIsFullscreen(bool p_Fullscreen)
	{
		RenderWindow::setIsFullscreen(p_Fullscreen);
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace

#endif //K15_OS_ANDROID