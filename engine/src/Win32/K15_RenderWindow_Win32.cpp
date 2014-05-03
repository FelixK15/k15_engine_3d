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

#include "Win32\K15_RenderWindow_Win32.h"
#include "K15_Mouse.h"
#include "K15_RenderTask.h"
#include "K15_RendererBase.h"
#include "K15_Application.h"

namespace K15_Engine { namespace Core {
	LRESULT CALLBACK K15_WindowProc(HWND p_HandleWindow,UINT p_MSG,WPARAM p_wParam,LPARAM p_lParam)
	{
	    RenderWindowBase* window = (RenderWindowBase*)GetWindowLong(p_HandleWindow,GWL_USERDATA);
		if(p_MSG == WM_CREATE)
		{
			//setup raw input for mouse and gamepads
			RAWINPUTDEVICE devices[2];
			devices[0].usUsagePage = 0x01;
			devices[0].usUsage = 0x05; //gamepad
			devices[0].dwFlags = RIDEV_DEVNOTIFY; //notify us about device removal
			devices[0].hwndTarget = p_HandleWindow;

			//raw mouse input is only used for mouse move events
			devices[1].usUsage = 0x01;
			devices[1].usUsage = 0x02; //mouse
			devices[1].dwFlags = 0;
			devices[1].hwndTarget = p_HandleWindow;

			if(RegisterRawInputDevices(devices,2,sizeof(devices[0]) == FALSE))
			{
				_LogError("Could not create raw input devices. %s.",g_Application->getLastError().c_str());
			}

      //return 0;
		}
		else if(p_MSG == WM_CLOSE)
		{
			PostQuitMessage(0);
			return 0;
		}
		else if(p_MSG == WM_ACTIVATE)
		{
			bool hasFocus = p_wParam > 0;
			window->setHasFocus(hasFocus);

      //return 0;
		}
		else if(p_MSG == WM_INPUT)
		{
			uint32 size = 0;
			GetRawInputData((HRAWINPUT)p_lParam,RID_INPUT,0,&size,sizeof(RAWINPUTHEADER));
			byte* buffer = (byte*)alloca(size);

			if(GetRawInputData((HRAWINPUT)p_lParam,RID_INPUT,buffer,&size,sizeof(RAWINPUTHEADER)) == size)
			{
				EventName name;
				RAWINPUT *raw = (RAWINPUT*)buffer;
				if(raw->header.dwType == RIM_TYPEMOUSE && raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					// mouse input
					int x = raw->data.mouse.lLastX;
					int y = raw->data.mouse.lLastY;

					name = _EN(onMouseMoved);

					uint32 eventArguments[2] = {x,y};

					GameEvent* mouseMoveEvent = K15_NEW GameEvent(name,(void*)eventArguments,64);
					g_EventManager->triggerEvent(mouseMoveEvent);
				}
				else if(raw->header.dwType == RIM_TYPEHID)
				{
					if(p_wParam == GIDC_ARRIVAL)
					{
						name = _EN(onInputDeviceConnected);
					}
					else if(p_wParam == GIDC_REMOVAL)
					{
						name = _EN(onInputDeviceDisconnected);
					}

					//todo get raw input device informations.

					GameEvent* deviceEvent = K15_NEW GameEvent(name,0,0);
					g_EventManager->triggerEvent(deviceEvent);
				}
			}

      return 0;
		}
		else if(p_MSG == WM_INPUT_DEVICE_CHANGE)
		{
      return 0;
		}
		else if(p_MSG == WM_MBUTTONDOWN || p_MSG == WM_MBUTTONUP || p_MSG == WM_MBUTTONDBLCLK ||
			    p_MSG == WM_LBUTTONDOWN || p_MSG == WM_LBUTTONUP || p_MSG == WM_LBUTTONDBLCLK ||
				p_MSG == WM_RBUTTONDOWN || p_MSG == WM_RBUTTONUP || p_MSG == WM_RBUTTONDBLCLK ||
				p_MSG == WM_XBUTTONDOWN || p_MSG == WM_XBUTTONUP || p_MSG == WM_XBUTTONDBLCLK ||
				p_MSG == WM_MOUSEMOVE || p_MSG == WM_MOUSEWHEEL)
		{
			int x,y;
			y = GET_Y_LPARAM(p_lParam);
			x = GET_X_LPARAM(p_lParam);

			EventName name;
			if(p_MSG == WM_MBUTTONDOWN || p_MSG == WM_LBUTTONDOWN || p_MSG == WM_RBUTTONDOWN || p_MSG == WM_XBUTTONDOWN)
			{
				name = _EN(onMousePressed);
			}
			else if(p_MSG == WM_MBUTTONUP || p_MSG == WM_LBUTTONUP || p_MSG == WM_RBUTTONUP || p_MSG == WM_XBUTTONUP)
			{
				name = _EN(onMouseReleased);
			}
			else if(p_MSG == WM_MBUTTONDBLCLK || p_MSG == WM_LBUTTONDBLCLK || p_MSG == WM_RBUTTONDBLCLK || p_MSG == WM_XBUTTONDBLCLK)
			{
				name = _EN(onMouseDoubleClicked);
			}
			else if(p_MSG == WM_MOUSEWHEEL)
			{
				name = _EN(onMouseWheel);

				uint32 eventArguments[3] = {(uint32)(p_wParam & 0xFFFF0000),x,y};
				
				GameEvent* mouseWheelEvent = K15_NEW GameEvent(name,(void*)eventArguments,96);
				g_EventManager->triggerEvent(mouseWheelEvent);
			}

			MouseActionArguments args;

			args.xPx = x;
			args.yPx = y;

			args.xNDC = (float)x / (float)window->getResolution().width;
			args.yNDC = (float)y / (float)window->getResolution().height;

			//ndc are now from 0 to 1.

			args.xNDC -= 0.5f;
			args.yNDC -= 0.5f;

			args.xNDC *= 2;
			args.yNDC *= 2;

			//ndc are now from -1 to +1;

			if(p_wParam & MK_LBUTTON)
			{
				args.button = InputDevices::Mouse::BTN_LEFT;
			}
			else if(p_wParam & MK_RBUTTON)
			{
				args.button = InputDevices::Mouse::BTN_RIGHT;
			}
			else if(p_wParam & MK_MBUTTON)
			{
				args.button = InputDevices::Mouse::BTN_MIDDLE;
			}
			else if(p_wParam & MK_XBUTTON1)
			{
				args.button = InputDevices::Mouse::BTN_SPECIAL1;
			}
			else if(p_wParam & MK_XBUTTON2)
			{
				args.button = InputDevices::Mouse::BTN_SPECIAL2;
			}

			GameEvent* mouseEvent = K15_NEW GameEvent(name,(void*)&args,sizeof(MouseActionArguments));
			g_EventManager->triggerEvent(mouseEvent);
		}
		else if(p_MSG == WM_KEYDOWN || p_MSG == WM_KEYUP)
		{

			EventName name;
			if(p_MSG == WM_KEYDOWN)
			{
				name = _EN(onKeyPressed);
			}
			else if(p_MSG == WM_KEYUP)
			{
				name = _EN(onKeyReleased);
			}
			uint32 key = p_wParam;
			GameEvent* keyEvent = K15_NEW GameEvent(name,(void*)&key,32);
			g_EventManager->triggerEvent(keyEvent);
		}
		else if(WM_SIZE)
		{
			if(p_wParam == SIZE_MAXIMIZED || p_wParam == SIZE_MINIMIZED || p_wParam == SIZE_RESTORED)
			{
				RECT clientRect;
				GetClientRect(p_HandleWindow,&clientRect);

				Resolution res;
				res.width = clientRect.right - clientRect.left;
				res.height = clientRect.bottom - clientRect.top;

				if(res.width != 0 && res.height != 0)
				{
					if(g_Application->getRenderTask()->getRenderer())
					{
						g_Application->getRenderTask()->getRenderer()->onResolutionChanged(res);
					}
				}
			}
		}

		return DefWindowProc(p_HandleWindow,p_MSG,p_wParam,p_lParam);
	}
	/*********************************************************************************/
	RenderWindow_Win32::RenderWindow_Win32()
		: RenderWindowBase()
	{

	}
	/*********************************************************************************/
	RenderWindow_Win32::~RenderWindow_Win32()
	{

	}
	/*********************************************************************************/
	bool RenderWindow_Win32::initialize()
	{
		m_Instance = GetModuleHandle(0);

		WNDCLASS wc = {0};
		wc.lpszClassName = "K15_RenderWindowClass";
		wc.hInstance = m_Instance;
		wc.lpfnWndProc = K15_WindowProc;
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		wc.hCursor = LoadCursor(0,IDC_ARROW);
		
		if(RegisterClass(&wc) == FALSE)
		{
			_LogError("Could not register WNDCLASS \"K15_RenderWindowClass\" Error:%s",Application::getInstance()->getLastError().c_str());
			return false;
		}

		if((m_HandleWindow = CreateWindow("K15_RenderWindowClass",
			m_WindowTitle.c_str(),WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			0,0,m_Instance,0)) == INVALID_HANDLE_VALUE)
		{
			_LogError("Could not create window. Error:%s",Application::getInstance()->getLastError().c_str());
			return false;
		}
		
		SetWindowLong(m_HandleWindow,GWL_USERDATA,(LONG)this);

		RECT windowrect = {0};
		GetWindowRect(m_HandleWindow,&windowrect);

		m_CurrentResolution.height = windowrect.bottom - windowrect.top;
		m_CurrentResolution.width = windowrect.right - windowrect.left;

		m_DeviceContext = GetDC(m_HandleWindow);

		ShowWindow(m_HandleWindow,SW_SHOW);
		return true;
	}
	/*********************************************************************************/
	void RenderWindow_Win32::shutdown()
	{
		CloseWindow(m_HandleWindow);
		DestroyWindow(m_HandleWindow);
		UnregisterClass("K15_RenderWindowClass",m_Instance);
	}
	/*********************************************************************************/
	void RenderWindow_Win32::setWindowTitle(const String& p_WindowTitle)
	{
		SetWindowText(m_HandleWindow,p_WindowTitle.c_str());
	}
	/*********************************************************************************/
	void RenderWindow_Win32::setResolution(const Resolution& p_Resolution, bool p_ForceChange)
	{
		RenderWindowBase::setResolution(p_Resolution);

		if(p_ForceChange || (p_Resolution.width != m_CurrentResolution.width ||
			p_Resolution.height != m_CurrentResolution.height))
		{
			if(m_IsFullscreen)
			{
				DEVMODE dm = {0};
				dm.dmSize = sizeof(dm);
				//dm.dmBitsPerPel = g_Application->getRenderTask()->getRenderer() ? RendererBase::PixelFormatSize[g_Application->getRenderTask()->getRenderer()->getFrameBufferPixelFormat()] : 32;
				dm.dmPelsHeight = p_Resolution.height;
				dm.dmPelsWidth = p_Resolution.width;
				dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
				DWORD flags = CDS_RESET;
				if(isFullscreen())
				{
					flags |= CDS_FULLSCREEN;
				}

				DWORD result = 0;
				if((result = ChangeDisplaySettings(&dm,flags)) != DISP_CHANGE_SUCCESSFUL)
				{
					char* error = 0;
					if(result == DISP_CHANGE_BADMODE)
					{
						error = "Graphics mode not supported";
					}
					else if(result == DISP_CHANGE_FAILED)
					{
						error = "The display driver failed the specified graphics mode.";
					}
					else if(result == DISP_CHANGE_NOTUPDATED)
					{
						error = "Unable to write settings to registry";
					}
					else if(result == DISP_CHANGE_BADFLAGS)
					{
						error = "An invalid set of flags was passed in";
					}

					_LogError("Could not change fullscreen resolution to \"%ix%i\" Error:%s",p_Resolution.width,p_Resolution.height,error);
					return;
				}
			}
			else
			{
				if(SetWindowPos(m_HandleWindow,HWND_TOP,0,0,p_Resolution.width,p_Resolution.height,
					SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOZORDER | SWP_NOSENDCHANGING) == FALSE)
				{
					_LogError("Could not change resolution to \"%ix%i\" Error:%s",p_Resolution.width,p_Resolution.height,g_Application->getLastError().c_str());
					return;
				}
			}
		}
	}
	/*********************************************************************************/
	void RenderWindow_Win32::setIsFullscreen(bool p_Fullscreen)
	{
		m_IsFullscreen = p_Fullscreen;
		setResolution(m_CurrentResolution,true);
	}
	/*********************************************************************************/
	HDC RenderWindow_Win32::getDeviceContext() const
	{
		return m_DeviceContext;
	}
	/*********************************************************************************/
	HWND RenderWindow_Win32::getHandleWindow() const
	{
		return m_HandleWindow;
	}
	/*********************************************************************************/
	HINSTANCE RenderWindow_Win32::getHandleInstance() const
	{
		return m_Instance;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace