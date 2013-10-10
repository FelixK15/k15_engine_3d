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

#include "K15_RenderWindow_Win32.h"
#include "K15_Application.h"
#include "K15_LogManager.h"
#include "K15_StringUtil.h"
#include "K15_EventManager.h"

namespace K15_Engine { namespace System {
	LRESULT CALLBACK K15_WindowProc(HWND p_HandleWindow,UINT p_MSG,WPARAM p_wParam,LPARAM p_lParam)
	{
		//if(p_MSG == WM_)

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
			_LogError(StringUtil::format("Could not register WNDCLASS \"K15_RenderWindowClass\" Error:%s",Application::getInstance()->getLastError()));
			return false;
		}

		if((m_HandleWindow = CreateWindow("K15_RenderWindowClass",
			m_WindowTitle.c_str(),WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			0,0,m_Instance,0)) != INVALID_HANDLE_VALUE)
		{
			_LogError(StringUtil::format("Could not create window. Error:%s",Application::getInstance()->getLastError()));
			return false;
		}

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
		DWORD flags = 0;
		if(isFullscreen())
		{
			flags |= CDS_FULLSCREEN;
		}

		DWORD result = 0;
		if((result = ChangeDisplaySettings(&dm,flags)) != DISP_CHANGE_SUCCESSFUL)
		{
			_LogError(StringUtil::format("Could not change resolution to \"%ix%i\" Error:%s",p_Resolution.width,p_Resolution.height,Application::getInstance()->getLastError()));
		}
	}
	/*********************************************************************************/
	void RenderWindow_Win32::setIsFullscreen(bool p_Fullscreen)
	{
		m_IsFullscreen = p_Fullscreen;
		setResolution(m_CurrentResolution);
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace