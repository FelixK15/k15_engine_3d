#include "win32/K15_EventsWin32.h"
#include "win32/K15_EnvironmentWin32.h"
#include "win32/K15_WindowWin32.h"

#include "K15_OSLayer_Window.h"
#include "K15_OSLayer_SystemEvents.h"
#include "K15_OSLayer_OSContext.h"

#ifdef K15_OS_WINDOWS

/*********************************************************************************/
intern void K15_Win32WindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.event = K15_WINDOW_CREATED;
	win32Event.eventFlags = K15_WINDOW_EVENT_FLAG;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern void K15_Win32WindowClosed(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	K15_SystemEvent win32Event = {};

	win32Event.event = K15_APPLICATION_QUIT;
	win32Event.eventFlags = K15_WINDOW_EVENT_FLAG | K15_SYSTEM_EVENT_FLAG;
	
	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern void K15_Win32WindowActivated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
intern void K15_Win32WindowResized(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
intern void K15_Win32KeyInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	win32Event.params.key = wParam;//K15_Win32ConvertKeyCode(wParam);

	if (win32Event.event > 0)
	{
		K15_AddSystemEventToQueue(&win32Event);
	}
}
/*********************************************************************************/
intern void K15_Win32MouseInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.mouseButton = wParam;//K15_Win32ConvertKeyCode(wParam);

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
intern void K15_Win32MouseMovementReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.position.x = GET_X_LPARAM(lParam);
	win32Event.params.position.y = GET_Y_LPARAM(lParam);
	win32Event.event = K15_MOUSE_MOVED;

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern void K15_Win32MouseWheelInputReceived(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	K15_SystemEvent win32Event = {};

	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
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
void K15_Win32EvaluateControllerConnectivity(DWORD* p_CurrentControllerStates, DWORD* p_PreviousControllerStates)
{
	for (DWORD controllerIndex = 0;
		 controllerIndex < K15_WIN32_MAX_CONTROLLER;
		 ++controllerIndex)
	{
		DWORD currentControllerState = p_CurrentControllerStates[controllerIndex];
		DWORD previousControllerState = p_PreviousControllerStates[controllerIndex];

		if (currentControllerState != previousControllerState)
		{
			K15_SystemEvent win32Event = {};
			win32Event.params.controllerIndex = controllerIndex;
			win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

			if (currentControllerState == ERROR_DEVICE_NOT_CONNECTED)
			{
				//device was previously connected
				win32Event.event = K15_CONTROLLER_DISCONNECTED;
			}
			else if(previousControllerState == ERROR_DEVICE_NOT_CONNECTED)
			{
				//device was previously not connected
				win32Event.event = K15_CONTROLLER_CONNECTED;
			}

			K15_AddSystemEventToQueue(&win32Event);
		}
	}
}
/*********************************************************************************/
void K15_Win32EvaluateControllerStates(XINPUT_STATE* p_CurrentControllerStates, XINPUT_STATE* p_PreviousControllerStates)
{
	for (DWORD controllerIndex = 0;
		 controllerIndex < K15_WIN32_MAX_CONTROLLER;
		 ++controllerIndex)
	{
		XINPUT_STATE currentState = p_CurrentControllerStates[controllerIndex];
		XINPUT_STATE previousState = p_PreviousControllerStates[controllerIndex];

		if (currentState.dwPacketNumber != previousState.dwPacketNumber)
		{
			//buttons 
			if (currentState.Gamepad.wButtons != previousState.Gamepad.wButtons)
			{
				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_A) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_A))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_A) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_FACE_DOWN_BUTTON;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_B) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_B))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_B) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_FACE_RIGHT_BUTTON;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_X) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_X))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_X) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_FACE_LEFT_BUTTON;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_Y))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_FACE_UP_BUTTON;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_DPAD_UP;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_DPAD_LEFT;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_DPAD_DOWN;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_DPAD_RIGHT;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_START) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_START))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_START) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_START;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_SELECT;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_LEFT_SHOULDER_BUTTON;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}

				if ((currentState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) !=
					(previousState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
				{
					K15_SystemEvent win32Event;
					//A pressed or released
					win32Event.event = (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0 ? K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
					win32Event.params.controllerButton = (controllerIndex << 16) | K15_CONTROLLER_RIGHT_SHOULDER_BUTTON;
					win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

					K15_AddSystemEventToQueue(&win32Event);
				}
			}

			//trigger
			if (currentState.Gamepad.bLeftTrigger != previousState.Gamepad.bLeftTrigger)
			{
				K15_SystemEvent win32Event;
				win32Event.event = K15_LEFT_SHOULDER_TRIGGER;
				win32Event.params.triggerValue = (float)currentState.Gamepad.bLeftTrigger / 255.f;
				win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

				K15_AddSystemEventToQueue(&win32Event);
			}

			if (currentState.Gamepad.bRightTrigger != previousState.Gamepad.bRightTrigger)
			{
				K15_SystemEvent win32Event;
				win32Event.event = K15_LEFT_SHOULDER_TRIGGER;
				win32Event.params.triggerValue = (float)currentState.Gamepad.bRightTrigger / 255.f;
				win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

				K15_AddSystemEventToQueue(&win32Event);
			}

			//left thumbstick
			if ((currentState.Gamepad.sThumbLX != previousState.Gamepad.sThumbLX)
				&& (abs(currentState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 
				|| abs(previousState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)))
			{
				K15_SystemEvent win32Event;
				win32Event.event = K15_LEFT_THUMB_X;
				win32Event.params.thumbValue = (float)currentState.Gamepad.sThumbLX / 32768.f; 
				win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

				K15_AddSystemEventToQueue(&win32Event);
			}

			if ((currentState.Gamepad.sThumbLY != previousState.Gamepad.sThumbLY)
				&& (abs(currentState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 
				|| abs(previousState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)))
			{
				K15_SystemEvent win32Event;
				win32Event.event = K15_LEFT_THUMB_Y;
				win32Event.params.thumbValue = (float)currentState.Gamepad.sThumbLY / 32768.f; 
				win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

				K15_AddSystemEventToQueue(&win32Event);
			}

			//right thumbstick
			if ((currentState.Gamepad.sThumbRX != previousState.Gamepad.sThumbRX)
				&& (abs(currentState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 
				|| abs(previousState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)))
			{
				K15_SystemEvent win32Event;
				win32Event.event = K15_RIGHT_THUMB_X;
				win32Event.params.thumbValue = (float)currentState.Gamepad.sThumbRX / 32768.f; 
				win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

				K15_AddSystemEventToQueue(&win32Event);
			}

			if ((currentState.Gamepad.sThumbRY != previousState.Gamepad.sThumbRY)
				&& (abs(currentState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 
				|| abs(previousState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)))
			{
				K15_SystemEvent win32Event;
				win32Event.event = K15_RIGHT_THUMB_Y;
				win32Event.params.thumbValue = (float)currentState.Gamepad.sThumbRY / 32768.f; 
				win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

				K15_AddSystemEventToQueue(&win32Event);
			}
		}
	}
}
/*********************************************************************************/
uint8 K15_Win32PumpSystemEvents(K15_OSLayerContext* p_OSContext)
{
	K15_Window* window = p_OSContext->window.window;
	K15_Win32Context* win32Context = (K15_Win32Context*)p_OSContext->userData;
	K15_Win32Window* win32WindowData = (K15_Win32Window*)window->userData;
	HWND hwnd = win32WindowData->hwnd;
	
	MSG msg;

	DWORD previousControllerConnectivity[K15_WIN32_MAX_CONTROLLER] = {};
	XINPUT_STATE previousControllerStates[K15_WIN32_MAX_CONTROLLER] = {};

	memcpy(previousControllerConnectivity, win32Context->XInput.controllerConnectivity, sizeof(DWORD) * K15_WIN32_MAX_CONTROLLER);
	memcpy(previousControllerStates, win32Context->XInput.controllerStates, sizeof(XINPUT_STATE) * K15_WIN32_MAX_CONTROLLER);


	while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	win32Context->XInput.connectedController = 0;

	for (DWORD controllerIndex = 0;
		 controllerIndex < K15_WIN32_MAX_CONTROLLER;
		 ++controllerIndex)
	{
		XINPUT_CAPABILITIES controllerCapabilites = {};
		XINPUT_STATE controllerState = {};

		DWORD controllerStatus = win32Context->XInput.getCapabilities(controllerIndex, 0, &controllerCapabilites);
		win32Context->XInput.getState(controllerIndex, &controllerState);

		win32Context->XInput.controllerStates[controllerIndex] = controllerState;
		win32Context->XInput.controllerConnectivity[controllerIndex] = controllerStatus;
		win32Context->XInput.connectedController += controllerStatus == ERROR_SUCCESS ? 1 : 0;
	}

	K15_Win32EvaluateControllerConnectivity(win32Context->XInput.controllerConnectivity, 
									  previousControllerConnectivity);

	K15_Win32EvaluateControllerStates(win32Context->XInput.controllerStates,
									  previousControllerStates);

	return K15_SUCCESS;
}
/*********************************************************************************/

#endif //K15_OS_WINDOWS