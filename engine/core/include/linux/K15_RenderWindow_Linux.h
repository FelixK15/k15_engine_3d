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
 *
 *
 */

#ifndef _K15Engine_Core_RenderWindow_Linux_h_
#define _K15Engine_Core_RenderWindow_Linux_h_


#include "K15_Prerequisites.h"

#ifdef K15_OS_LINUX

namespace K15_Engine { namespace Core {
    class RenderWindow_Linux
    {
    public:
        static bool initialize();
        static void shutdown();

        static uint32 getWidth();
        static uint32 getHeight();

        static void setWindowTitle(const String& p_WindowTitle);
        static void setResolution(const Resolution& p_Resolution);
        static void setIsFullscreen(bool p_Fullscreen);

        static int getXAttributes(int* p_AttribArray, int p_AttribArraySize, bool p_FBConfig);
    public:
        static Display* ms_Display;
        static Window ms_Window;
        static GLXWindow ms_GLXWindow;
        static uint32 ms_NotifyFlags;
        static uint32 ms_Width;
        static uint32 ms_Height;
        static const int X_ATTRIB_ARRAY_SIZE = 17;
        static Atom ms_DeleteWindowID;
    };
}} //end of K15_Engine::Core namespace

#endif //K15_OS_LINUX

#endif //_K15Engine_Core_RenderWindow_Linux_h_
