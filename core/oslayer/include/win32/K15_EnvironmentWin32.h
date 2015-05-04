#ifndef _K15_OSLayer_Environment_Win32_h_
#define _K15_OSLayer_Environment_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS

//XINPUT
typedef void (WINAPI *XInputEnableFnc)(BOOL);
typedef DWORD (WINAPI *XInputGetCapabilitiesFnc)(DWORD, DWORD, XINPUT_CAPABILITIES*);
typedef DWORD (WINAPI *XInputGetStateFnc)(DWORD, XINPUT_STATE*);
typedef DWORD (WINAPI *XInputSetStateFnc)(DWORD, XINPUT_VIBRATION*);


//DirectInput
typedef HRESULT (WINAPI *DirectInputCreateExFnc)(HINSTANCE, DWORD, GUID, LPVOID*, LPUNKNOWN);
//DirectSound
typedef HRESULT (WINAPI* DirectSoundCreateFnc)(LPGUID, LPDIRECTSOUND*, LPUNKNOWN);

const DWORD K15_WIN32_MAX_CONTROLLER = 4;

struct K15_Win32Context
{
	HINSTANCE hInstance;
	float performanceFrequency;
	
	struct /*XInput*/
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

	struct 
	{
		HMODULE module;
		LPDIRECTINPUT directInputHandle;
		DirectInputCreateExFnc directInputCreateEx;
	} DirectInput;

	struct 
	{
		HMODULE module;

		DirectSoundCreateFnc directSoundCreate;
	} DirectSound;
};

uint8 K15_Win32InitializeOSLayer(HINSTANCE p_hInstance);
char* K15_Win32GetError();
void K15_Win32Sleep(double p_SleepTimeInSeconds);
void K15_Win32ShutdownOSLayer();
double K15_Win32GetElapsedSeconds();

#endif //K15_OS_WINDOWS

#endif //_K15_OSLayer_Environment_Win32_h_