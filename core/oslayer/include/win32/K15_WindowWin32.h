#ifndef _K15_Window_Win32_h_
#define _K15_Window_Win32_h_

#include "K15_Prerequisites.h"

#include "win32/K15_HeaderDefaultWin32.h"

struct K15_WindowSettings;

struct K15_Win32Window
{
	HWND hwnd;
	DWORD style;
}; // end of K15_Win32InternalWindowEnvironment struct declaration

uint8 K15_Win32CreateWindow(K15_OSContext* p_OSContext, K15_Window* p_Window, uint32 p_Flags, uint8 p_MonitorIndex);
uint8 K15_Win32SetWindowDimension(K15_OSContext* p_OSContext, K15_Window* p_Window, uint32 p_Width, uint32 p_Height);
uint8 K15_Win32SetWindowFullscreen(K15_OSContext* p_OSContext, K15_Window* p_Window, bool8 p_Fullscreen);
uint8 K15_Win32SetWindowTitle(K15_OSContext* p_OSContext, K15_Window* p_Window, char* p_Title);
uint8 K15_Win32CloseWindow(K15_OSContext* p_OSContext, K15_Window* p_Window);

#endif //_K15_Window_Win32_h_