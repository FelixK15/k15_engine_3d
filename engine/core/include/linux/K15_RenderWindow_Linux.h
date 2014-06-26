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

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core {
    class RenderWindow_Linux
    {
    public:
        static bool initialize();
        static void shutdown();

        static void setWindowTitle(const String& p_WindowTitle);
        static void setResolution(const Resolution& p_Resolution);
        static void setIsFullscreen(bool p_Fullscreen);

        static Display* getDisplay();
        static Window getWindow();
        static GLXWindow getGLXWindow();

        static setGLXWindow(GLXWindow p_glWindow);
    private:
        static Display* ms_Display;
        static Window ms_Window;
        static GLXWindow ms_GLXWindow;
    };
}} //end of K15_Engine::Core namespace
