#ifndef _K15_OSLayer_Environment_Win32_h_
#define _K15_OSLayer_Environment_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

typedef void (*XInputEnableFnc)(BOOL);
typedef DWORD (*XInputGetCapabilitiesFnc)(DWORD, DWORD, XINPUT_CAPABILITIES*);
typedef DWORD (*XInputGetStateFnc)(DWORD, XINPUT_STATE*);
typedef DWORD (*XInputSetStateFnc)(DWORD, XINPUT_VIBRATION*);

const DWORD K15_WIN32_MAX_CONTROLLER = 4;

struct K15_Win32Context
{
	HINSTANCE hInstance;

	struct XInput
	{
		HMODULE module;

		XInputEnableFnc enable;
		XInputGetCapabilitiesFnc getCapabilities;
		XInputGetStateFnc getState;
		XInputSetStateFnc setState;

		XINPUT_STATE controllerStates[K15_WIN32_MAX_CONTROLLER];
		DWORD controllerConnectivity[K15_WIN32_MAX_CONTROLLER];
		DWORD connectedController;
	} XInput;
};

uint8 K15_Win32InitializeOSLayer(HINSTANCE p_hInstance);
void K15_Win32ShutdownOSLayer();

#endif //_K15_OSLayer_Environment_Win32_h_