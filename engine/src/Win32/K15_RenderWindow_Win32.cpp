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

namespace K15_Engine { namespace Core {
	LRESULT CALLBACK K15_WindowProc(HWND p_HandleWindow,UINT p_MSG,WPARAM p_wParam,LPARAM p_lParam)
	{
	    RenderWindowBase* window = (RenderWindowBase*)GetWindowLong(p_HandleWindow,GWL_USERDATA);

		if(p_MSG == WM_CLOSE)
		{
			PostQuitMessage(0);
			return 0;
		}
		else if(p_MSG == WM_ACTIVATE)
		{
			bool hasFocus = p_wParam > 0;
			window->setHasFocus(hasFocus);
		}
		else if(p_MSG == WM_MBUTTONDOWN || p_MSG == WM_MBUTTONUP || p_MSG == WM_MBUTTONDBLCLK ||
			    p_MSG == WM_LBUTTONDOWN || p_MSG == WM_LBUTTONUP || p_MSG == WM_LBUTTONDBLCLK ||
				p_MSG == WM_RBUTTONDOWN || p_MSG == WM_RBUTTONUP || p_MSG == WM_RBUTTONDBLCLK ||
				p_MSG == WM_XBUTTONDOWN || p_MSG == WM_XBUTTONUP || p_MSG == WM_XBUTTONDBLCLK ||
				p_MSG == WM_MOUSEMOVE || p_MSG == WM_MOUSEWHEEL)
		{
			int x,y;
			y = p_lParam & 0xFFFF0000;
			x = p_lParam & 0x0000FFFF;

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
			else if(p_MSG == WM_MOUSEMOVE)
			{
				name = _EN(onMouseMove);

				uint32 eventArguments[2] = {x,y};

				GameEvent* mouseMoveEvent = K15_NEW GameEvent(name,(void*)eventArguments,64);
				g_EventManager->triggerEvent(mouseMoveEvent);
			}
			else if(p_MSG == WM_MOUSEWHEEL)
			{
				name = _EN(onMouseWheel);

				uint32 eventArguments[3] = {(uint32)(p_wParam & 0xFFFF0000),x,y};
				
				GameEvent* mouseWheelEvent = K15_NEW GameEvent(name,(void*)eventArguments,96);
				g_EventManager->triggerEvent(mouseWheelEvent);
			}

			uint32 eventArguments[3] = {0};

			eventArguments[1] = x;
			eventArguments[2] = y;

			if(p_wParam & MK_LBUTTON)
			{
				eventArguments[0] = InputDevices::Mouse::BTN_LEFT;
			}
			else if(p_wParam & MK_RBUTTON)
			{
				eventArguments[0] = InputDevices::Mouse::BTN_RIGHT;
			}
			else if(p_wParam & MK_MBUTTON)
			{
				eventArguments[0] = InputDevices::Mouse::BTN_MIDDLE;
			}
			else if(p_wParam & MK_XBUTTON1)
			{
				eventArguments[0] = InputDevices::Mouse::BTN_SPECIAL1;
			}
			else if(p_wParam & MK_XBUTTON2)
			{
				eventArguments[0] = InputDevices::Mouse::BTN_SPECIAL2;
			}

			GameEvent* mouseEvent = K15_NEW GameEvent(name,(void*)eventArguments,96);
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
	void RenderWindow_Win32::setResolution(const Resolution& p_Resolution)
	{
		DEVMODE dm = {0};
		dm.dmSize = sizeof(dm);
		dm.dmBitsPerPel = 32;
		dm.dmPelsHeight = p_Resolution.height;
		dm.dmPelsWidth = p_Resolution.width;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		DWORD flags = CDS_RESET;
		if(isFullscreen())
		{
			flags |= CDS_FULLSCREEN;
		}

		DWORD result = 0;
		if((result = ChangeDisplaySettings(&dm,flags)) != DISP_CHANGE_SUCCESSFUL)
		{
			_LogError("Could not change resolution to \"%ix%i\" Error:%s",p_Resolution.width,p_Resolution.height,Application::getInstance()->getLastError().c_str());
			return;
		}
	}
	/*********************************************************************************/
	void RenderWindow_Win32::setIsFullscreen(bool p_Fullscreen)
	{
		m_IsFullscreen = p_Fullscreen;
		setResolution(m_CurrentResolution);
	}
	/*********************************************************************************/
	HDC RenderWindow_Win32::getDeviceContext()
	{
		return m_DeviceContext;
	}
	/*********************************************************************************/
	HWND RenderWindow_Win32::getHandleWindow()
	{
		return m_HandleWindow;
	}
	/*********************************************************************************/
	HINSTANCE RenderWindow_Win32::getHandleInstance()
	{
		return m_Instance;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace