/**
 * @file K15_Mouse_Win32.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_PrecompiledHeader.h"

#include "K15_Mouse.h"
#include "K15_Texture.h"
#include "K15_RendererBase.h"
#include "K15_LogManager.h"

#include "win32\K15_RenderWindow_Win32.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	/*********************************************************************************/
	HCURSOR g_Cursor = LoadCursor(0,IDC_ARROW);
	bool g_ReleaseResource = false;
	/*********************************************************************************/
	void Mouse::hideMouse()
	{
		ShowCursor(FALSE);
	}
	/*********************************************************************************/
	void Mouse::showMouse()
	{
		ShowCursor(TRUE);
	}
	/*********************************************************************************/
	void Mouse::setMousePos(int32 x, int32 y)
	{
		SetCursorPos(x,y);
	}
	/*********************************************************************************/
	void Mouse::getMousePos(int32 *x,int32 *y)
	{
		POINT pos;
		GetCursorPos(&pos);

		if(x)
		{
			*x = pos.x;
		}

		if(y)
		{
			*y = pos.y;
		}
	}
	/*********************************************************************************/
	bool Mouse::setMouseIcon(byte* p_IconData, int32 p_Width, int32 p_Height, uint32 p_ColorBits)
	{
		HCURSOR newCursor = 0;

		if((newCursor = (HCURSOR)CreateBitmap(p_Width,p_Height,1,p_ColorBits,p_IconData)) == 0)
		{
			_LogError("Could not load cursor image from memory. (%s)",g_Application->getLastError().c_str());
			return false;
		}

		if(g_ReleaseResource)
		{
			CloseHandle(g_Cursor);
			g_Cursor = 0;
		}

		g_Cursor = newCursor;
		g_ReleaseResource = true;

		return false;
	}
	/*********************************************************************************/
	bool Mouse::setMouseIcon(const String& p_IconPath)
	{
		HCURSOR newCursor = 0;
		RenderWindow_Win32* renderwindow = (RenderWindow_Win32*)g_Application->getRenderWindow();
		if((newCursor = (HCURSOR)LoadImage(renderwindow->getHandleInstance(),p_IconPath.c_str(),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_LOADFROMFILE)) == 0)
		{
			_LogError("Could not load cursor image from file \"%s\". (%s)",p_IconPath.c_str(),g_Application->getLastError().c_str());
			return false;
		}

		if(g_ReleaseResource)
		{
			CloseHandle(g_Cursor);
			g_Cursor = 0;
		}

		g_Cursor = newCursor;

		SetCursor(g_Cursor);

		g_ReleaseResource = true;
		return true;
	}
	/*********************************************************************************/
	bool Mouse::isPressed(Enum p_Button)
	{
		int32 key = 0;
		if(p_Button == BTN_LEFT)
		{
			key = VK_LBUTTON;
		}
		else if(p_Button == BTN_RIGHT)
		{
			key = VK_RBUTTON;
		}
		else if(p_Button == BTN_MIDDLE)
		{
			key = VK_MBUTTON;
		}
		else if(p_Button == BTN_SPECIAL1)
		{
			key = VK_XBUTTON1;
		}
		else if(p_Button == BTN_SPECIAL2)
		{
			key = VK_XBUTTON2;
		}

		if((GetKeyState(key) & 0x80))
		{
			return true;
		}

		return false;
	}
	/*********************************************************************************/
}}}/// end of K15_Engine::Core::InputDevice namespace