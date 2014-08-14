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
    uint32 RenderWindow_Linux::ms_Width = 0;
    uint32 RenderWindow_Linux::ms_Height = 0;
    uint32 RenderWindow_Linux::ms_NotifyFlags = ButtonPressMask | ButtonReleaseMask
                                                | KeyPressMask  | KeyReleaseMask
                                                | ExposureMask  | ResizeRedirectMask
                                                | FocusChangeMask | PointerMotionMask;
    /*********************************************************************************/

    /*********************************************************************************/
    bool RenderWindow_Linux::initialize()
    {
        if(ms_Display = XOpenDisplay(getenv("DISPLAY")))
        {
            int screen = XDefaultScreen(ms_Display);
            Window rootwin = XRootWindow(ms_Display, screen);

            ms_Width = RenderWindow::getWidth() == 0 ? 800 : RenderWindow::getWidth();
            ms_Height = RenderWindow::getHeight() == 0 ? 600 : RenderWindow::getHeight();

            XVisualInfo* vi = 0;
            XSetWindowAttributes windowAttribs;
            int attribList[X_ATTRIB_ARRAY_SIZE] = {0};

            //Get VisualInfo struct for our attributes
            int size = getXAttributes(attribList, X_ATTRIB_ARRAY_SIZE, false);
            vi = glXChooseVisual(ms_Display, screen, attribList);

            //we just need the visual and depth. Free VisualInfo afterwards
            Visual* visual = vi->visual;
            int depth = vi->depth;
            XFree(vi); vi = 0;

            if(visual->c_class == DirectColor)
            {
                printf("directcolor");
            }

            windowAttribs.override_redirect = False;
            windowAttribs.background_pixmap = None;
            windowAttribs.border_pixel = 0;
            windowAttribs.colormap = XCreateColormap(ms_Display, rootwin, visual, AllocNone);

            ms_Window = XCreateWindow(ms_Display, rootwin, 0, 0, ms_Width, ms_Height, 0,
                                      depth, InputOutput, visual,
                                      CWOverrideRedirect | CWBackPixmap |
                                      CWBorderPixel | CWColormap, &windowAttribs);

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

        XFlush(ms_Display);
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
    uint32 RenderWindow_Linux::getWidth()
    {
        return ms_Width;
    }
    /*********************************************************************************/
    uint32 RenderWindow_Linux::getHeight()
    {
        return ms_Height;
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
    int RenderWindow_Linux::getXAttributes(int* p_AttribArray, int p_AttribArraySize, bool p_FBConfig)
    {
        K15_ASSERT(p_AttribArraySize >= X_ATTRIB_ARRAY_SIZE,
          StringUtil::format("Array size for RenderWindow::getXAttributs of %d is insufficient. %d is required.",
          p_AttribArraySize, X_ATTRIB_ARRAY_SIZE));

        int i = 0;
        int colorComponentSize = 8; //TODO: get size from config
        int depthSize = 24;
        int stencilSize = 8;
        if(p_FBConfig)
        {
            p_AttribArray[i++] = GLX_RENDER_TYPE;
            p_AttribArray[i++] = GLX_RGBA_BIT;
        }
        else
        {
            p_AttribArray[i++] = GLX_RGBA;
        }

        p_AttribArray[i++] = GLX_RED_SIZE;
        p_AttribArray[i++] = colorComponentSize;

        p_AttribArray[i++] = GLX_GREEN_SIZE;
        p_AttribArray[i++] = colorComponentSize;

        p_AttribArray[i++] = GLX_BLUE_SIZE;
        p_AttribArray[i++] = colorComponentSize;

        p_AttribArray[i++] = GLX_ALPHA_SIZE;
        p_AttribArray[i++] = colorComponentSize;

        p_AttribArray[i++] = GLX_DOUBLEBUFFER;

        if(p_FBConfig)
        {
            p_AttribArray[i++] = True;
        }

        p_AttribArray[i++] = GLX_DEPTH_SIZE;
        p_AttribArray[i++] = depthSize;

        p_AttribArray[i++] = GLX_STENCIL_SIZE;
        p_AttribArray[i++] = stencilSize;

        p_AttribArray[i] = None;

        return i;
    }
    /*********************************************************************************/
}} //end of K15_Engine::Core namespace

#endif //K15_OS_LINUX
