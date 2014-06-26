/**
 * @file K15_RenderWindow_Linux.h
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
 *
 * @section DESCRIPTION
 */

#include "K15_PrecompiledHeader.h"
#include "K15_RenderWindow.h"

#ifdef K15_OS_LINUX

#include "linux/K15_RenderWindow_Linux.h"

namespace K15_Engine { namespace Core {
    /*********************************************************************************/
    Display* RenderWindow_Linux::ms_Display = 0;
    Window* RenderWindow_Linux::ms_Window = 0;
    /*********************************************************************************/

    /*********************************************************************************/
    bool RenderWindow_Linux::initialize()
    {
        if(ms_Display = XOpenDisplay(getenv("DISPLAY")))
        {
            ms_Window = XCreateSimpleWindow(ms_Display, 0, 0, 0,
                                            RenderWindow::getWidth(), RenderWindow::getHeight(),
                                            0, 0, 0);
        }

        if(ms_Display && ms_Window != BadMatch || ms_Window != BadValue || ms_Window != BadWindow)
        {
            K15_LOG_ERROR("Could not create X11 window. Error:\"%s\" (%d)",
                          OSLayer::getLastError().c_str(), errno);

            return false;
        }

        K15_LOG_SUCCESS("Succesfully created X11 window.");

        return true;
    }
    /*********************************************************************************/
    void RenderWindow_Linux::shutdown()
    {
        if(ms_GLXWindow) glXDestroyWindow(ms_Display, ms_GLXWindow);
        if(ms_Window) XDestroyWindow(ms_Display, ms_Window);
        if(ms_Display) XCloseDisplay(ms_Display);

        ms_Display = 0;
        ms_Window = 0;
        ms_GLXWindow = 0;
    }
    /*********************************************************************************/
    void RenderWindow_Linux::setWindowTitle(const String& p_WindowTitle)
    {
        XStoreName(ms_Display, ms_Window, p_WindowTitle.c_str());
    }
    /*********************************************************************************/
    void RenderWindow_Linux::setResolution(const Resolution& p_Resolution)
    {
        XResizeWindow(ms_Display, ms_Window, p_Resolution.width, p_Resolution.height);
    }
    /*********************************************************************************/
    void RenderWindow_Linux::setIsFullscreen(bool p_Fullscreen)
    {

    }
    /*********************************************************************************/
}} //end of K15_Engine::Core namespace

#endif //K15_OS_LINUX
