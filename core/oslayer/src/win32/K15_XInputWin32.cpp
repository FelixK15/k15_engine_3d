/*********************************************************************************/
intern inline void K15_Win32InitializeXInputDevices(K15_Win32Context* p_Win32Context, uint32* p_NumInputDevices)
{
	uint32 connectedController = 0;
	
	//will be called first so we can start to fill the win32 controller array from pos 0
	p_Win32Context->XInput.enable(TRUE);

	for (uint32 controllerIndex = 0;
		 controllerIndex < K15_MAX_CONTROLLER;
		 ++controllerIndex)
	{
		K15_Win32Controller* currentController = &p_Win32Context->controller[controllerIndex];
		XINPUT_CAPABILITIES controllerCapabilites = {};
		DWORD controllerStatus = p_Win32Context->XInput.getCapabilities(controllerIndex, 0, &controllerCapabilites);
		
		if (controllerStatus == ERROR_SUCCESS)
		{
			currentController->APIType = K15_WIN32_APITYPE_XINPUT;
			currentController->controllerIndex = controllerIndex;
			currentController->controllerState = K15_WIN32_CONTROLLER_STATE_CONNECTED;
			
			p_Win32Context->XInput.getState(controllerIndex, &currentController->xInputState);

			++connectedController;
		}
	}

	*p_NumInputDevices = connectedController;
}
/*********************************************************************************/
intern inline BOOL K15_Win32CheckXInputConnectivity(K15_Win32Context* p_Win32Context, K15_Win32Controller* p_Controller)
{
	XINPUT_CAPABILITIES controllerCapabilites = {};
	DWORD controllerIndex = p_Controller->controllerIndex;
	DWORD controllerStatus = p_Win32Context->XInput.getCapabilities(controllerIndex, 0, &controllerCapabilites);

	BOOL connected = (controllerStatus == ERROR_SUCCESS);
	
	return connected;
}
/*********************************************************************************/
intern inline void K15_Win32PumpXInputControllerEvents(K15_Win32Context* p_Win32Context, K15_Win32Controller* p_Controller)
{
	XINPUT_STATE previousState = p_Controller->xInputState;
	XINPUT_STATE currentState = {};

	DWORD controllerIndex = p_Controller->controllerIndex;

	p_Win32Context->XInput.getState(controllerIndex, &currentState);

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
/*********************************************************************************/
// intern inline void K15_Win32EvaluateXInputControllerConnectivity(DWORD* p_CurrentControllerStates, DWORD* p_PreviousControllerStates)
// {
// 	for (DWORD controllerIndex = 0;
// 		controllerIndex < K15_MAX_CONTROLLER;
// 		++controllerIndex)
// 	{
// 		DWORD currentControllerState = p_CurrentControllerStates[controllerIndex];
// 		DWORD previousControllerState = p_PreviousControllerStates[controllerIndex];
// 
// 		if (currentControllerState != previousControllerState)
// 		{
// 			K15_SystemEvent win32Event = {};
// 			win32Event.params.controllerIndex = controllerIndex;
// 			win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
// 
// 			if (currentControllerState == ERROR_DEVICE_NOT_CONNECTED)
// 			{
// 				//device was previously connected
// 				win32Event.event = K15_CONTROLLER_DISCONNECTED;
// 			}
// 			else if(previousControllerState == ERROR_DEVICE_NOT_CONNECTED)
// 			{
// 				//device was previously not connected
// 				win32Event.event = K15_CONTROLLER_CONNECTED;
// 			}
// 
// 			K15_AddSystemEventToQueue(&win32Event);
// 		}
// 	}
// }
/*********************************************************************************/