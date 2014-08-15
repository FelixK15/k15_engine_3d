/**
 * @file K15_RenderWindow_Win32.cpp
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
 */

#include "K15_PrecompiledHeader.h"
#include "K15_RenderWindow.h"

#ifdef K15_OS_WINDOWS

namespace K15_Engine { namespace Core {
   /*********************************************************************************/
   HDC       RenderWindow_Win32::ms_DeviceContext  = 0;
   HWND      RenderWindow_Win32::ms_HandleWindow   = 0;
   HINSTANCE RenderWindow_Win32::ms_Instance       = 0;
   /*********************************************************************************/

   /*********************************************************************************/
   bool RenderWindow_Win32::initialize()
   {
      ms_Instance = GetModuleHandle(0);

      WNDCLASS wc = {0};
      wc.lpszClassName = "K15_RenderWindowClass";
      wc.hInstance = ms_Instance;
      wc.lpfnWndProc = OSLayer::windowProc;
      wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
      wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hCursor = LoadCursor(0,IDC_ARROW);

      if(RegisterClass(&wc) == FALSE) {
         K15_LOG_ERROR("Could not register WNDCLASS \"K15_RenderWindowClass\" Error:%s", OSLayer::getError().c_str());
         return false;
      }

      int width  = RenderWindow::getWidth() == 0 ? CW_USEDEFAULT : RenderWindow::getWidth();
      int height = RenderWindow::getHeight() == 0 ? CW_USEDEFAULT : RenderWindow::getHeight();

      if((ms_HandleWindow = CreateWindow("K15_RenderWindowClass",
                                         "", WS_OVERLAPPEDWINDOW,
                                         CW_USEDEFAULT,CW_USEDEFAULT, width, height,
                                         0,0,ms_Instance,0)) == INVALID_HANDLE_VALUE) {
         K15_LOG_ERROR("Could not create window. Error:%s", OSLayer::getError().c_str());
         return false;
      }

      RECT windowrect = {0};
      GetWindowRect(ms_HandleWindow,&windowrect);

      RenderWindow::setResolution(windowrect.right - windowrect.left, windowrect.bottom - windowrect.top);
      setWindowTitle(RenderWindow::getWindowTitle());

      ms_DeviceContext = GetDC(ms_HandleWindow);

      ShowWindow(ms_HandleWindow,SW_SHOW);
      return true;
   }
   /*********************************************************************************/
   void RenderWindow_Win32::shutdown()
   {
      CloseWindow(ms_HandleWindow);
      DestroyWindow(ms_HandleWindow);
      UnregisterClass("K15_RenderWindowClass",ms_Instance);
   }
   /*********************************************************************************/
   void RenderWindow_Win32::setWindowTitle(const String& p_WindowTitle)
   {
      SetWindowText(ms_HandleWindow, p_WindowTitle.c_str());
   }
   /*********************************************************************************/
   void RenderWindow_Win32::setResolution(const Resolution& p_Resolution)
   {
     RECT screenRect = {0};
     screenRect.bottom = p_Resolution.height;
     screenRect.right = p_Resolution.width;
     AdjustWindowRect(&screenRect, WS_OVERLAPPEDWINDOW, FALSE);

     SetWindowPos(ms_HandleWindow, HWND_TOP, 0, 0,
                  screenRect.right - screenRect.left,
                  screenRect.bottom - screenRect.top,
                  SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
   }
   /*********************************************************************************/
   void RenderWindow_Win32::setIsFullscreen(bool p_Fullscreen)
   {
      const Resolution& currentResolution = RenderWindow::getResolution();

      if(p_Fullscreen) {
//          DEVMODE dm = {0};
//          dm.dmSize = sizeof(dm);
//          //dm.dmBitsPerPel = g_Application->getRenderTask()->getRenderer() ? RendererBase::PixelFormatSize[g_Application->getRenderTask()->getRenderer()->getFrameBufferPixelFormat()] : 32;
//          dm.dmPelsHeight = currentResolution.height;
//          dm.dmPelsWidth = currentResolution.width;
//          dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
//          DWORD flags = CDS_RESET | CDS_FULLSCREEN;
//          DWORD result = 0;
// 
//          if((result = ChangeDisplaySettings(&dm,flags)) != DISP_CHANGE_SUCCESSFUL) {
//             char* error = 0;
//             if(result == DISP_CHANGE_BADMODE) {
//                error = "Graphics mode not supported";
//             } else if(result == DISP_CHANGE_FAILED) {
//                error = "The display driver failed the specified graphics mode.";
//             } else if(result == DISP_CHANGE_NOTUPDATED) {
//                error = "Unable to write settings to registry";
//             } else if(result == DISP_CHANGE_BADFLAGS) {
//                error = "An invalid set of flags was passed in";
//             }
// 
//             K15_LOG_ERROR("Could not change fullscreen resolution to \"%ix%i\" Error:%s",currentResolution.width, currentResolution.height,error);
//             return;
//          }

        if(SetWindowPos(ms_HandleWindow,HWND_TOP,0,0,currentResolution.width, currentResolution.height,
          SWP_DRAWFRAME) == FALSE) {
             K15_LOG_ERROR("Could not change resolution to \"%ix%i\" Error:%s",
               currentResolution.width, 
               currentResolution.height, 
               OSLayer::getError().c_str());
             return;
         }
      }
   }
   /*********************************************************************************/
   HDC RenderWindow_Win32::getDeviceContext()
   {
      return ms_DeviceContext;
   }
   /*********************************************************************************/
   HWND RenderWindow_Win32::getHandleWindow()
   {
      return ms_HandleWindow;
   }
   /*********************************************************************************/
   HINSTANCE RenderWindow_Win32::getHandleInstance()
   {
      return ms_Instance;
   }
   /*********************************************************************************/
   uint32 RenderWindow_Win32::getWidth()
   {
      RECT clientRect = {0};
      GetClientRect(ms_HandleWindow, &clientRect);

      return clientRect.right - clientRect.left;
   }
   /*********************************************************************************/
   uint32 RenderWindow_Win32::getHeight()
   {
      RECT clientRect = {0};
      GetClientRect(ms_HandleWindow, &clientRect);

      return clientRect.bottom - clientRect.top;
   }
   /*********************************************************************************/
}}//end of K15_Engine::Core namespace

#endif //K15_OS_WINDOWS