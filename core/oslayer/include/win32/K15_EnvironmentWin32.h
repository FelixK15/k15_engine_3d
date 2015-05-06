#ifndef _K15_OSLayer_Environment_Win32_h_
#define _K15_OSLayer_Environment_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

#define K15_WIN32_APITYPE_XINPUT 1
#define K15_WIN32_APITYPE_DIRECTINPUT 2
#define K15_WIN32_APITYPE_UNDEFINED 3 //basically non existing controller


#define K15_WIN32_CONTROLLER_STATE_CONNECTED 4
#define K15_WIN32_CONTROLLER_STATE_NOT_CONNECTED 5



//XINPUT
typedef void (WINAPI *XInputEnableFnc)(BOOL);
typedef DWORD (WINAPI *XInputGetCapabilitiesFnc)(DWORD, DWORD, XINPUT_CAPABILITIES*);
typedef DWORD (WINAPI *XInputGetStateFnc)(DWORD, XINPUT_STATE*);
typedef DWORD (WINAPI *XInputSetStateFnc)(DWORD, XINPUT_VIBRATION*);


//DirectInput
typedef HRESULT (WINAPI *DirectInput8CreateFnc)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

//DirectSound
typedef HRESULT (WINAPI* DirectSoundCreateFnc)(LPGUID, LPDIRECTSOUND*, LPUNKNOWN);

/*********************************************************************************/
struct K15_Win32Controller
{
	struct  
	{
		uint8 APIType; //Xinput or DirectInput
		uint8 controllerState; //connected or not
		uint8 controllerIndex;

		union 
		{
			XINPUT_STATE xInputState;
			LPDIRECTINPUTDEVICE directInputDevice;
		};
	};
};
/*********************************************************************************/
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
	} XInput;
	struct 
	{
		HMODULE module;
		LPDIRECTINPUT directInputHandle;
		DirectInput8CreateFnc directInput8Create;
	} DirectInput;

	struct 
	{
		HMODULE module;

		DirectSoundCreateFnc directSoundCreate;
	} DirectSound;

	K15_Win32Controller controller[K15_MAX_CONTROLLER];
	DWORD connectedController;
};

uint8 K15_Win32InitializeOSLayer(HINSTANCE p_hInstance);
char* K15_Win32GetError();
void K15_Win32Sleep(double p_SleepTimeInSeconds);
void K15_Win32ShutdownOSLayer();
double K15_Win32GetElapsedSeconds();

#endif //_K15_OSLayer_Environment_Win32_h_