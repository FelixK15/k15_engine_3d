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
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

namespace K15_Engine { namespace Core {
    /*********************************************************************************/
    Display* RenderWindow_Linux::ms_Display = 0;
    Window RenderWindow_Linux::ms_Window = 0;
    GLXWindow RenderWindow_Linux::ms_GLXWindow = 0;
    Atom RenderWindow_Linux::ms_DeleteWindowID = 0;
    uint32 RenderWindow_Linux::ms_NotifyFlags = ButtonPressMask | ButtonReleaseMask
                                                | KeyPressMask  | KeyReleaseMask
                                                | ExposureMask  | ResizeRedirectMask
                                                | FocusChangeMask;
    /*********************************************************************************/

    /*********************************************************************************/
    bool RenderWindow_Linux::initialize()
    {
        if(ms_Display = XOpenDisplay(getenv("DISPLAY")))
        {
            int screen = XDefaultScreen(ms_Display);
            Window rootwin = XRootWindow(ms_Display, screen);

            uint32 width = RenderWindow::getWidth() == 0 ? 800 : RenderWindow::getWidth();
            uint32 height = RenderWindow::getHeight() == 0 ? 600 : RenderWindow::getHeight();

            ms_Window = XCreateSimpleWindow(ms_Display, rootwin, 0, 0,
                                            width, height, 0,
                                            XBlackPixel(ms_Display, screen),
                                            XBlackPixel(ms_Display, screen));

            XSelectInput(ms_Display, ms_Window, ms_NotifyFlags);

            XMapWindow(ms_Display, ms_Window);

            //we want to get informed when the window manager wants to delete our window
            ms_DeleteWindowID = XInternAtom(ms_Display, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(ms_Display, ms_Window, &ms_DeleteWindowID, 1);
        }

        if(!ms_Display ||
          (ms_Window == BadMatch || ms_Window == BadValue || ms_Window == BadWindow))
        {
            K15_LOG_ERROR("Could not create X11 window. Error:\"%s\" (%d)",
                          OSLayer::getError().c_str(), errno);

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
        ms_DeleteWindowID = 0;
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
