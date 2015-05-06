#include "win32/K15_EventsWin32.h"
#include "win32/K15_EnvironmentWin32.h"
#include "win32/K15_WindowWin32.h"

#include "K15_OSLayer_Window.h"
#include "K15_OSLayer_SystemEvents.h"
#include "K15_OSLayer_OSContext.h"

#include "win32/K15_XInputWin32.cpp"
#include "win32/K15_DirectInputWin32.cpp"

/*********************************************************************************/
intern inline void K15_Win32WindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.event = K15_WINDOW_CREATED;
	win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;

	//Call once on window creation to get default values
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osContext->userData;

	K15_Win32InitializeXInputDevices(win32Context);
	K15_Win32InitializeDirectInputDevices(win32Context, hWnd);

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern inline void K15_Win32WindowClosed(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	K15_SystemEvent win32Event = {};

	win32Event.event = K15_APPLICATION_QUIT;
	win32Event.eventFlags = K15_WINDOW_EVENT_FLAG | K15_SYSTEM_EVENT_FLAG;
	
	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern inline void K15_Win32WindowActivated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//low order word = window activated or deactivated?
	WORD reason = LOWORD(wParam);
	K15_OSLayerContext* osLayerContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osLayerContext->userData;
	K15_Window* window = osLayerContext->window.window;

	if (window)
	{
		K15_SystemEvent win32Event = {};

		if (reason == WA_ACTIVE || reason == WA_CLICKACTIVE)
		{
			win32Event.event = K15_WINDOW_FOCUS_GAINED;
			window->flags |= K15_WINDOW_FLAG_HAS_FOCUS;
			win32Context->XInput.enable(TRUE);
		}
		else if(reason == WA_INACTIVE)
		{
			win32Event.event = K15_WINDOW_FOCUS_LOST;
			window->flags %= ~K15_WINDOW_FLAG_HAS_FOCUS;
			win32Context->XInput.enable(FALSE);
		}

		win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;

		K15_AddSystemEventToQueue(&win32Event);
	}
}
/*********************************************************************************/
intern inline void K15_Win32WindowResized(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == SIZE_RESTORED)
	{
		K15_SystemEvent win32Event = {};

		K15_OSLayerContext* osLayerContext = K15_GetOSLayerContext();
		K15_Window* window = osLayerContext->window.window;

		if (window)
		{
			WORD newWidth = (WORD)(lParam << 16);
			WORD newHeight = (WORD)(lParam);

			K15_SetWindowDimension(window, newHeight, newWidth);

			win32Event.event = K15_WINDOW_RESIZED;
			win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;
			win32Event.params.size.width = newWidth;
			win32Event.params.size.height = newHeight;

			K15_AddSystemEventToQueue(&win32Event);
		}
	}
}
/*********************************************************************************/
intern inline void K15_Win32KeyInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	win32Event.params.key = (uint32)wParam;//K15_Win32ConvertKeyCode(wParam);

	if (win32Event.event > 0)
	{
		K15_AddSystemEventToQueue(&win32Event);
	}
}
/*********************************************************************************/
intern inline void K15_Win32MouseInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.mouseButton = (uint32)wParam;//K15_Win32ConvertKeyCode(wParam);

	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			win32Event.event = K15_MOUSE_BUTTON_PRESSED;
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			win32Event.event = K15_MOUSE_BUTTON_RELEASED;
	}

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern inline void K15_Win32MouseMovementReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.position.x = GET_X_LPARAM(lParam);
	win32Event.params.position.y = GET_Y_LPARAM(lParam);
	win32Event.event = K15_MOUSE_MOVED;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern inline void K15_Win32MouseWheelInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
	win32Event.event = K15_WHEEL_MOVED;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern inline DWORD K15_Win32CheckControllerConnectivity(K15_Win32Context* p_Win32Context, K15_Win32Controller* p_Controller, uint32 p_NumController)
{
	DWORD connectedController = 0;

	for (uint32 controllerIndex = 0;
		 controllerIndex < p_NumController;
		 ++controllerIndex)
	{
		K15_Win32Controller* currentController = &p_Controller[controllerIndex];
		uint8 previousState = currentController->controllerState;
		uint8 currentState = 0;

		if (K15_Win32CheckXInputConnectivity(p_Win32Context, currentController) == TRUE)
		{
			p_Controller->APIType = K15_WIN32_APITYPE_XINPUT;
			currentState = K15_WIN32_CONTROLLER_STATE_CONNECTED;
			connectedController += 1;
		}
		else if (K15_Win32CheckDirectInputConnectivity(p_Win32Context, currentController) == TRUE)
		{
			p_Controller->APIType = K15_WIN32_APITYPE_DIRECTINPUT;
			currentState = K15_WIN32_CONTROLLER_STATE_CONNECTED;
			connectedController += 1;
		}
		else
		{
			p_Controller->APIType = K15_WIN32_APITYPE_UNDEFINED;
			currentState = K15_WIN32_CONTROLLER_STATE_NOT_CONNECTED;

			continue;
		}

		p_Controller->controllerState = currentState;

		//Fire messages when controller got disconnected/connected
		if (previousState != currentState &&
			currentState != K15_WIN32_CONTROLLER_STATE_NOT_CONNECTED)
		{
			K15_SystemEvent controllerStateEvent = {};
			controllerStateEvent.params.controllerIndex = controllerIndex;
			controllerStateEvent.eventFlags = K15_INPUT_EVENT_FLAG;

			if (previousState == K15_WIN32_CONTROLLER_STATE_CONNECTED)
			{
				controllerStateEvent.event = K15_CONTROLLER_DISCONNECTED;
			}
			else
			{
				controllerStateEvent.event = K15_CONTROLLER_CONNECTED;
			}

			K15_AddSystemEventToQueue(&controllerStateEvent);
		}
	}

	return connectedController;
}
/*********************************************************************************/
intern inline void K15_Win32PumpControllerEvents(K15_Win32Context* p_Win32Context, K15_Win32Controller* p_Controller, uint32 p_NumController)
{
	for (uint32 controllerIndex = 0;
		controllerIndex < p_NumController;
		++controllerIndex)
	{
		K15_Win32Controller* currentController = &p_Controller[controllerIndex];

		if (currentController->controllerState == K15_WIN32_CONTROLLER_STATE_CONNECTED
			&& currentController->APIType == K15_WIN32_APITYPE_XINPUT)
		{
			K15_Win32PumpXInputControllerEvents(p_Win32Context, currentController);
		}
		else if (currentController->controllerState == K15_WIN32_CONTROLLER_STATE_CONNECTED
			&& currentController->APIType == K15_WIN32_APITYPE_DIRECTINPUT)
		{
			K15_Win32PumpDirectInputControllerEvents(p_Win32Context, currentController);
		}
	}
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
		break;
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
	K15_Window* window = p_OSContext->window.window;
	K15_Win32Context* win32Context = (K15_Win32Context*)p_OSContext->userData;
	K15_Win32Window* win32WindowData = (K15_Win32Window*)window->userData;
	HWND hwnd = win32WindowData->hwnd;
	
	MSG msg;

	while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	K15_Win32Controller* controllers = win32Context->controller;

	//Check connectivity
	DWORD numConnectedController = K15_Win32CheckControllerConnectivity(win32Context, controllers, K15_MAX_CONTROLLER);
	K15_Win32PumpControllerEvents(win32Context, controllers, K15_MAX_CONTROLLER);

	win32Context->connectedController = numConnectedController;

	/*DWORD previousControllerConnectivity[K15_MAX_CONTROLLER] = {};
	XINPUT_STATE previousControllerStates[K15_MAX_CONTROLLER] = {};

	memcpy(previousControllerConnectivity, win32Context->XInput.controllerConnectivity, sizeof(DWORD) * K15_MAX_CONTROLLER);
	memcpy(previousControllerStates, win32Context->XInput.controllerStates, sizeof(XINPUT_STATE) * K15_MAX_CONTROLLER);

	

	K15_Win32GetXInputControllerConnectivity(win32Context);

	K15_Win32EvaluateXInputControllerConnectivity(win32Context->controllerConnectivity, 
									  previousControllerConnectivity);

	K15_Win32EvaluateXInputControllerStates(win32Context->XInput.controllerStates,
									  previousControllerStates);

	K15_Win32EvaluateDirectInputControllerStates(win32Context->DirectInput.devices, win32Context->DirectInput.numDevices);*/

	return K15_SUCCESS;
}
/*********************************************************************************/