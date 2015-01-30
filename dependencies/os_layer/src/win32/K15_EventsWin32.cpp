#include "win32/K15_EventsWin32.h"
#include "win32/K15_EnvironmentWin32.h"
#include "win32/K15_WindowWin32.h"

#include "K15_OSLayer_Window.h"
#include "K15_OSLayer_SystemEvents.h"
#include "K15_OSLayer_OSContext.h"

#ifdef K15_OS_WINDOWS

/*********************************************************************************/
internal void K15_Win32WindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.event = K15_WINDOW_CREATED;
	win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
internal void K15_Win32WindowClosed(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	K15_SystemEvent win32Event = {};

	win32Event.event = K15_APPLICATION_QUIT;
	win32Event.eventFlags = K15_WINDOW_EVENT_FLAG | K15_SYSTEM_EVENT_FLAG;
	
	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
internal void K15_Win32WindowActivated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//low order word = window activated or deactivated?
	WORD reason = LOWORD(lParam);
	K15_OSLayerContext* osLayerContext = K15_GetOSLayerContext();
	K15_Window* window = osLayerContext->window;

	if (window)
	{
		K15_SystemEvent win32Event = {};

		if (reason == WA_ACTIVE || reason == WA_CLICKACTIVE)
		{
			win32Event.event = K15_WINDOW_FOCUS_GAINED;
			window->flags |= K15_WINDOW_FLAG_HAS_FOCUS;

		}
		else if(reason == WA_INACTIVE)
		{
			win32Event.event = K15_WINDOW_FOCUS_LOST;
			window->flags %= ~K15_WINDOW_FLAG_HAS_FOCUS;
		}

		win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;

		K15_AddSystemEventToQueue(&win32Event);
	}
}
/*********************************************************************************/
internal void K15_Win32WindowResized(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == SIZE_RESTORED)
	{
		K15_SystemEvent win32Event = {};

		K15_OSLayerContext* osLayerContext = K15_GetOSLayerContext();
		K15_Window* window = osLayerContext->window;

		if (window)
		{
			WORD newWidth = (WORD)(lParam << 16);
			WORD newHeight = (WORD)(lParam);

			K15_SetWindowDimension(window, newHeight, newWidth);

			win32Event.event = K15_WINDOW_MOVED;
			win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;

			K15_AddSystemEventToQueue(&win32Event);
		}
	}
}
/*********************************************************************************/
internal void K15_Win32KeyInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	bool8 wasDown = ((lParam & (1 << 30)) != 0);
	bool8 isDown = ((lParam & (1 << 31)) == 0);
	bool8 isSystemKey = FALSE;
	
	if (isDown != wasDown)
	{
		if (uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP)
		{
			isSystemKey = TRUE;
			win32Event.eventFlags |= K15_SYSTEM_EVENT_FLAG;
		}

		if (isDown)
		{
			win32Event.event = K15_KEY_PRESSED;
		}
		else
		{
			win32Event.event = K15_KEY_RELEASED;
		}
	}

	win32Event.eventFlags |= K15_INPUT_EVENT_FLAG;
	win32Event.key = wParam;//K15_Win32ConvertKeyCode(wParam);

	if (win32Event.event > 0)
	{
		K15_AddSystemEventToQueue(&win32Event);
	}
}
/*********************************************************************************/
internal void K15_Win32MouseInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.button = wParam;//K15_Win32ConvertKeyCode(wParam);

	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			win32Event.event = K15_BUTTON_PRESSED;
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			win32Event.event = K15_BUTTON_RELEASED;
	}

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
internal void K15_Win32MouseMovementReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.position.x = GET_X_LPARAM(lParam);
	win32Event.position.y = GET_Y_LPARAM(lParam);
	win32Event.event = K15_MOUSE_MOVED;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
internal void K15_Win32MouseWheelInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
	win32Event.event = K15_WHEEL_MOVED;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
LRESULT CALLBACK K15_Win32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	uint8 messageHandled = FALSE;

	switch(uMsg)
	{
	case WM_CREATE:
		K15_Win32WindowCreated(hWnd, uMsg, wParam, lParam);
		break;

	case WM_CLOSE:
		K15_Win32WindowClosed(hWnd, uMsg, wParam, lParam);
		messageHandled = TRUE;
		break;
	
	case WM_ACTIVATE:
		K15_Win32WindowActivated(hWnd, uMsg, wParam, lParam);
		break;

	case WM_SIZE:
		K15_Win32WindowResized(hWnd, uMsg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		K15_Win32KeyInputReceived(hWnd, uMsg, wParam, lParam);
		break;
	
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
		K15_Win32MouseInputReceived(hWnd, uMsg, wParam, lParam);
		break;

	case WM_MOUSEMOVE:
		K15_Win32MouseMovementReceived(hWnd, uMsg, wParam, lParam);
		break;

	case WM_MOUSEWHEEL:
		K15_Win32MouseWheelInputReceived(hWnd, uMsg, wParam, lParam);
	}

	if (messageHandled == FALSE)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
/*********************************************************************************/
uint8 K15_Win32PumpSystemEvents(K15_OSLayerContext* p_OSContext)
{
	K15_Window* window = p_OSContext->window;
	K15_Win32Window* win32WindowData = (K15_Win32Window*)window->userData;
	HWND hwnd = win32WindowData->hwnd;
	
	MSG msg;

	while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/

#endif //K15_OS_WINDOWS