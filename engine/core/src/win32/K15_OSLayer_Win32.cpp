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
#include "K15_LogManager.h"
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
         K15_LOG_ERROR("Could not get performance counter frequency. Error:%s",Application::getInstance()->getLastError().c_str());
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

      case WM_MBUTTONDOWN:
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
         _onMousePress(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_MBUTTONUP:
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
         _onMouseRelease(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_MOUSEWHEEL:
         _onMouseWheel(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_MOUSEMOVE:
         _onMouseMove(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_KEYDOWN:
         _onKeyPress(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_KEYUP:
         _onKeyRelease(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;

      case WM_SIZE:
         _onResize(p_HandleWindow, p_MSG, p_wParam, p_lParam);
         break;
      }

      return DefWindowProc(p_HandleWindow, p_MSG, p_wParam, p_lParam);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_getMouseArgs(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam, MouseEventArguments* const p_MouseArgs)
   {
      if(p_wParam & MK_LBUTTON) {
         p_MouseArgs->button = InputDevices::Mouse::BTN_LEFT;
      } else if(p_wParam & MK_RBUTTON) {
         p_MouseArgs->button = InputDevices::Mouse::BTN_RIGHT;
      } else if(p_wParam & MK_MBUTTON) {
         p_MouseArgs->button = InputDevices::Mouse::BTN_MIDDLE;
      } else if(p_wParam & MK_XBUTTON1) {
         p_MouseArgs->button = InputDevices::Mouse::BTN_SPECIAL1;
      } else if(p_wParam & MK_XBUTTON2) {
         p_MouseArgs->button = InputDevices::Mouse::BTN_SPECIAL2;
      }

      int x,y;
      y = GET_Y_LPARAM(p_lParam);
      x = GET_X_LPARAM(p_lParam);

      p_MouseArgs->xPx = x;
      p_MouseArgs->yPx = y;
      p_MouseArgs->xNDC = K15_PX_TO_NDC(x, RenderWindow::getWidth());
      p_MouseArgs->yNDC = K15_PX_TO_NDC(y, RenderWindow::getHeight());
      p_MouseArgs->pressed = false;
      p_MouseArgs->wheelDelta = 0.f;

      if(p_MSG == WM_LBUTTONDOWN || p_MSG == WM_RBUTTONDOWN || p_MSG == WM_MBUTTONDOWN || p_MSG == WM_XBUTTONDOWN) {
         p_MouseArgs->pressed = true;
      }

      if(p_MSG == WM_MOUSEWHEEL) {
         int wheelDelta = GET_WHEEL_DELTA_WPARAM(p_wParam);
         p_MouseArgs->wheelDelta = ((float)wheelDelta / (float)WHEEL_DELTA);
      }
   }
   /*********************************************************************************/
   void OSLayer_Win32::_getKeyArgs(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam, KeyboardEventArguments* const p_KeyArgs)
   {
      p_KeyArgs->pressed = false;

      if(p_MSG == WM_KEYDOWN) {
         p_KeyArgs->pressed = true;
      }

      p_KeyArgs->key = p_wParam;
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

      g_EventManager->addEventToQueue(focusEvent);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onMousePress(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      MouseEventArguments args;
      _getMouseArgs(p_Window, p_MSG, p_wParam, p_lParam, &args);

      GameEvent* mousePress = K15_NEW GameEvent(InputDevices::Mouse::EventMousePressed,
                              (void*)&args, sizeof(MouseEventArguments));

      g_EventManager->addEventToQueue(mousePress);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onMouseRelease(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      MouseEventArguments args;
      _getMouseArgs(p_Window, p_MSG, p_wParam, p_lParam, &args);

      GameEvent* mouseRelease = K15_NEW GameEvent(InputDevices::Mouse::EventMouseReleased,
                                (void*)&args, sizeof(MouseEventArguments));

      g_EventManager->addEventToQueue(mouseRelease);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onMouseMove(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      MouseEventArguments args;
      _getMouseArgs(p_Window, p_MSG, p_wParam, p_lParam, &args);

      GameEvent* mouseMove = K15_NEW GameEvent(InputDevices::Mouse::EventMouseMoved,
                             (void*)&args, sizeof(MouseEventArguments));

      g_EventManager->addEventToQueue(mouseMove);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onMouseWheel(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      MouseEventArguments args;
      _getMouseArgs(p_Window, p_MSG, p_wParam, p_lParam, &args);

      GameEvent* mouseWheel = K15_NEW GameEvent(InputDevices::Mouse::EventMouseWheel,
                              (void*)&args, sizeof(MouseEventArguments));

      g_EventManager->addEventToQueue(mouseWheel);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onKeyPress(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      KeyboardEventArguments args;
      _getKeyArgs(p_Window, p_MSG, p_wParam, p_lParam, &args);

      GameEvent* keyPress = K15_NEW GameEvent(InputDevices::Keyboard::EventKeyPress,
                                              (void*)&args, sizeof(KeyboardEventArguments));

      g_EventManager->addEventToQueue(keyPress);
   }
   /*********************************************************************************/
   void OSLayer_Win32::_onKeyRelease(HWND p_Window, UINT p_MSG, WPARAM p_wParam, LPARAM p_lParam)
   {
      KeyboardEventArguments args;
      _getKeyArgs(p_Window, p_MSG, p_wParam, p_lParam, &args);

      GameEvent* keyRelease = K15_NEW GameEvent(InputDevices::Keyboard::EventKeyRelease,
                              (void*)&args, sizeof(KeyboardEventArguments));

      g_EventManager->addEventToQueue(keyRelease);
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

      g_EventManager->addEventToQueue(resize);
   }
   /*********************************************************************************/
}}//end of K15_Engine::Core namespace

#endif //K15_OS_WINDOWS