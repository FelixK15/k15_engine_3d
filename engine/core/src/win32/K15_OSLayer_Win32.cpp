/**
 * @file K15_ApplicationOSLayer_Win32.cpp
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

#ifdef K15_OS_WINDOWS

#include "Win32\K15_OSLayer_Win32.h"
#include "K15_Mouse.h"
#include "K15_Keyboard.h"
#include "K15_RenderWindow.h"

#include "K15_Application.h"
#include "K15_StringUtil.h"

namespace K15_Engine { namespace Core {
   /*********************************************************************************/
   const String OSLayer_Win32::OSName = "Microsoft Windows";
   const String OSLayer_Win32::PluginExtension = "dll";
   LARGE_INTEGER OSLayer_Win32::ms_PCF = {0};
   float OSLayer_Win32::ms_Frequency = 0.0f;
   /*********************************************************************************/

   /*********************************************************************************/
   void* OSLayer_Win32::os_malloc(uint32 p_Size)
   {
      static HANDLE processHeap = GetProcessHeap();
   #		if defined K15_DEBUG
      static DWORD flags = HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS;
   #		else
      static DWORD flags = HEAP_ZERO_MEMORY;
   #		endif //K15_DEBUG

      void* memory = HeapAlloc(processHeap,flags,p_Size);

      K15_ASSERT(memory,"Out of memory.");

      return memory;
   }
   /*********************************************************************************/
   void OSLayer_Win32::os_free(void* p_Pointer)
   {
      static HANDLE processHeap = GetProcessHeap();

      if(!p_Pointer) {
         return;
      }

      HeapFree(processHeap,0,p_Pointer);
   }
   /*********************************************************************************/
   bool OSLayer_Win32::initialize()
   {
      if(QueryPerformanceFrequency(&ms_PCF) == FALSE) {
         K15_LOG_ERROR_MESSAGE("Could not get performance counter frequency. Error:%s",Application::getInstance()->getLastError().c_str());
         return false;
      }

      ms_Frequency = float(ms_PCF.QuadPart);

      return true;
   }
   /*********************************************************************************/
   void OSLayer_Win32::shutdown()
   {

   }
   /*********************************************************************************/
   const String& OSLayer_Win32::getError()
   {
      static const uint32 ErrorBufferSize = 768;
      static String errorMsg;

      char* errorBuffer = 0;
      DWORD lastError = GetLastError();
      DWORD writtenChars = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,0,
                                         lastError,MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_UK),(LPSTR)&errorBuffer,0,0);

      if(writtenChars == 0 && !errorBuffer) {
         errorBuffer = (char*)alloca(ErrorBufferSize);
         sprintf(errorBuffer,"Could not retrieve last error from OS.");
      }

      errorMsg = errorBuffer;

      if(writtenChars) {
         LocalFree(errorBuffer);
      }

      return errorMsg;
   }
   /*********************************************************************************/
   void OSLayer_Win32::getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet)
   {
      DEVMODE dm = {0};
      dm.dmSize = sizeof(dm);

      for(int i = 0; EnumDisplaySettings(0,i,&dm) != 0; ++i) {
         Resolution currentResolution = {dm.dmPelsWidth,dm.dmPelsHeight};
         p_ResolutionSet->push_back(currentResolution);
      }
   }
   /*********************************************************************************/
   float OSLayer_Win32::getTime()
   {
      LARGE_INTEGER counts;
      QueryPerformanceCounter(&counts);

      return counts.QuadPart / ms_Frequency;
   }
   /*********************************************************************************/
   void OSLayer_Win32::sleep(float p_TimeInSeconds)
   {
      static DWORD msecs = 0;
      msecs = (DWORD)(p_TimeInSeconds * 1000);
      Sleep(msecs);
   }
   /*********************************************************************************/
   void OSLayer_Win32::onPreTick()
   {
      static MSG msg;
      static HWND hwnd = (HWND)INVALID_HANDLE_VALUE;

      hwnd = RenderWindowImpl::getHandleWindow();

      if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
         TranslateMessage(&msg);
         DispatchMessage(&msg);

         if(msg.message == WM_QUIT) {
            g_Application->setRunning(false);
         }
      }
   }
   /*********************************************************************************/
   void OSLayer_Win32::onPostTick()
   {

   }
   /*********************************************************************************/
   LRESULT CALLBACK OSLayer_Win32::windowProc(HWND p_HandleWindow, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      switch(p_MSG) {
      case WM_CREATE:
         _onWindowCreated(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_CLOSE:
         _onWindowClosed(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         return 0;

      case WM_ACTIVATE:
         _onWindowActivated(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_SIZE:
         _onResize(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;
      }

      return DefWindowProc(p_HandleWindow, p_MSG, p_wParam, p_lParam);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onWindowCreated(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
   //     //setup raw input for mouse and gamepads
   //     RAWINPUTDEVICE devices[2];
   //     devices[0].usUsagePage = 0x01;
   //     devices[0].usUsage = 0x05; //gamepad
   //     devices[0].dwFlags = RIDEV_DEVNOTIFY; //notify us about device removal
   //     devices[0].hwndTarget = p_Window;
   //
   //     //raw mouse input is only used for mouse move events
   //     devices[1].usUsage = 0x01;
   //     devices[1].usUsage = 0x02; //mouse
   //     devices[1].dwFlags = 0;
   //     devices[1].hwndTarget = p_Window;
   //
   //     if(RegisterRawInputDevices(devices,2,sizeof(devices[0]) == FALSE))
   //     {
   //       K15_LOG_ERROR("Could not create raw input devices. %s.",g_Application->getLastError().c_str());
   //     }
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onWindowClosed(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      PostQuitMessage(0);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onWindowActivated(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      bool activated = p_wParam > 0;

      RenderWindow::setHasFocus(activated);

      GameEvent* focusEvent = K15_NEW GameEvent(activated ? RenderWindow::EventFocusReceived : RenderWindow::EventFocusLost);

      g_EventManager->triggerEvent(focusEvent);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onResize(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      //         RECT clientRect;
      //         GetClientRect(p_HandleWindow,&clientRect);
      //
      //         Resolution res;
      //         res.width = clientRect.right - clientRect.left;
      //         res.height = clientRect.bottom - clientRect.top;
      ResizeEventArguments args;

      RECT clientRect = {0};
      GetClientRect(p_Window, &clientRect);

      args.oldHeight = RenderWindow::getHeight();
      args.oldWidth  = RenderWindow::getWidth();

      args.newHeight = clientRect.bottom - clientRect.top;
      args.newWidth  = clientRect.right - clientRect.left;

      GameEvent* resize = K15_NEW GameEvent(RenderWindow::EventResolutionChanged,
                                            (void*)&args, sizeof(ResizeEventArguments));

      g_EventManager->triggerEvent(resize);
   }
   /*********************************************************************************/
}}//end of K15_Engine::Core namespace

#endif //K15_OS_WINDOWS