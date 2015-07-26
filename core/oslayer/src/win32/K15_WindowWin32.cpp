#include "K15_Window.h"
#include "K15_OSContext.h"
#include "K15_DefaultCLibraries.h"
#include "K15_String.h"

#include "win32/K15_WindowWin32.h"
#include "win32/K15_HelperWin32.h"
#include "win32/K15_EventsWin32.h"
#include "win32/K15_OSContextWin32.h"

#include "win32/K15_HeaderDefaultWin32.h"

#include <K15_Logging.h>

/*********************************************************************************/
intern BOOL CALLBACK K15_Win32EnumMonitorProc(HMONITOR p_HMONITOR, HDC p_HDC, LPRECT p_Rect, LPARAM p_lParam)
{
	static int index = 0;
	HMONITOR* hm = (HMONITOR*)p_lParam;

	hm[index++] = p_HMONITOR;

	return K15_TRUE;
}
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_Win32CreateWindow(K15_OSContext* p_OSContext, K15_Window* p_Window, uint32 p_Flags, uint8 p_MonitorIndex)
{
	K15_Win32Context* Win32Context = (K15_Win32Context*)p_OSContext->userData;

	HINSTANCE hInstance = Win32Context->hInstance;
	int width = 800;
	int height = 600;
	int monitorID = p_MonitorIndex;

	//get monitors
	int amountMonitors = GetSystemMetrics(SM_CMONITORS);
	HMONITOR* monitorHandles = (HMONITOR*)alloca(sizeof(HMONITOR) * amountMonitors);
	
	//clamp monitorindex if needed
	if (monitorID > amountMonitors-1)
	{
		monitorID = 0;
	}

	//get HMONITOR values
	EnumDisplayMonitors(0, 0, K15_Win32EnumMonitorProc, (LPARAM)monitorHandles);

	//get monitor informations
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	if (GetMonitorInfoA(monitorHandles[monitorID], &monitorInfo) == 0)
	{
		return K15_OS_ERROR_MONITOR_NOT_FOUND;
	}

	DWORD style = WS_VISIBLE;

	if (p_Flags == 0)
	{
		style |= WS_OVERLAPPEDWINDOW;
	}
	else
	{
		if ((p_Flags & K15_WINDOW_FLAG_BORDERLESS) == 0)
		{
			style |= WS_BORDER;
		}
		else
		{
			style |= WS_POPUP;
		}

		if ((p_Flags & K15_WINDOW_FLAG_NO_BUTTONS) == 0)
		{
			style |= WS_CAPTION;
			style |= WS_SYSMENU;
		}
	}

	RECT winRect = {monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, width + monitorInfo.rcMonitor.left, height + monitorInfo.rcMonitor.top};
	AdjustWindowRect(&winRect, style, K15_FALSE);

	//create window with default settings
	HWND handle = CreateWindowExA(0, "K15_Win32WindowClass", "", style,
								winRect.left + (width / 2), winRect.top + (height / 2), 
								winRect.right - winRect.left, winRect.bottom - winRect.top,
								0, 0, hInstance, 0);

	if (handle == INVALID_HANDLE_VALUE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_Win32Window* win32Window = (K15_Win32Window*)K15_OS_MALLOC(sizeof(K15_Win32Window));

	win32Window->hwnd = handle;
	win32Window->style = style;

	p_Window->userData = (void*)win32Window;
	p_Window->height = height;
	p_Window->width = width;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32SetWindowDimension(K15_OSContext* p_OSContext, K15_Window* p_Window, uint32 p_Height, uint32 p_Width)
{
	K15_Win32Window* win32WindowData = (K15_Win32Window*)p_Window->userData;
	HWND hwnd = win32WindowData->hwnd;

	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICE);

	if (EnumDisplayDevices(0, p_Window->monitorIndex, &displayDevice, 0) == 0)
	{
		return K15_OS_ERROR_MONITOR_NOT_FOUND;
	}

	DEVMODE deviceSettings;
	if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceSettings) == 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	deviceSettings.dmPelsWidth = p_Width;
	deviceSettings.dmPelsHeight = p_Height;
	deviceSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	
	//CDS_RESET = always change settings
	if (ChangeDisplaySettings(&deviceSettings, CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
	{
		return K15_OS_ERROR_RESOLUTION_NOT_SUPPORTED;
	}

	ChangeDisplaySettings(&deviceSettings, CDS_RESET);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32SetWindowFullscreen(K15_OSContext* p_OSContext, K15_Window* p_Window, bool8 p_Fullscreen)
{
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32SetWindowTitle(K15_OSContext* p_OSContext, K15_Window* p_Window, char* p_Title)
{
	K15_Win32Window* win32WindowData = (K15_Win32Window*)p_Window->userData;
	HWND hwnd = win32WindowData->hwnd;
	uint32 titleLength= (uint32)strlen(p_Title) + 1; //+1 for 0 terminator
	wchar_t* titleWide = (wchar_t*)alloca(titleLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_Title, titleLength, titleWide);

	if (SetWindowTextW(hwnd, titleWide) != 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32CloseWindow(K15_OSContext* p_OSContext, K15_Window* p_Window)
{
	return K15_SUCCESS;
}
/*********************************************************************************/