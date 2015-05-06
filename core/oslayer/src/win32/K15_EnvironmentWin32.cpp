#define INITGUID

#include "win32/K15_EnvironmentWin32.h"

#ifdef K15_OS_WINDOWS 

#include "K15_OSLayer_OSContext.h"
#include "K15_OSLayer_Thread.h"

#include "win32/K15_WindowWin32.h"
#include "win32/K15_EventsWin32.h"
#include "win32/K15_ThreadWin32.h"

#include <K15_Logging.h>	

#ifdef K15_DEBUG
/*********************************************************************************/
void K15_Win32LogVisualStudio(const char* p_Message, LogPriority p_Priority)
{
	OutputDebugStringA(p_Message);
}
/*********************************************************************************/
void K15_Win32LogConsole(const char* p_Message, LogPriority p_Priority)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorCode = 0;

	switch (p_Priority)
	{
	case K15_LOG_PRIORITY_NORMAL:
		{
			colorCode = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
			break;
		}

	case K15_LOG_PRIORITY_ERROR:
		{
			colorCode = FOREGROUND_RED;
			break;
		}
	
	case K15_LOG_PRIORITY_WARNING:
		{
			colorCode = FOREGROUND_GREEN | FOREGROUND_RED;
			break;
		}

	case K15_LOG_PRIORITY_SUCCESS:
		{
			colorCode = FOREGROUND_GREEN;
			break;
		}
	
	case K15_LOG_PRIORITY_DEBUG:
		{
			colorCode = FOREGROUND_RED | FOREGROUND_BLUE;
			break;
		}
	}

	//set output color
	SetConsoleTextAttribute(consoleHandle,colorCode);
	printf("%s\n", p_Message);

	//set color back to white
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); 
}
/*********************************************************************************/
#endif //K15_DEBUG

// /*********************************************************************************/

// /*********************************************************************************/
// intern inline K15_Win32DirectInputController K15_InternalCreateInputDevice(LPDIRECTINPUTDEVICE p_DeviceInstance)
// {
// 	K15_Win32DirectInputController win32DirectInputController = {};
// 	HRESULT result = p_DeviceInstance->EnumObjects(K15_InternalEnumDeviceObjectsCallback, &win32DirectInputController, DIDFT_AXIS | DIDFT_BUTTON);
// 
// 	return win32DirectInputController;
// }
/*********************************************************************************/
intern inline uint8 K15_InternalTryLoadXInput(K15_Win32Context* p_Win32Context)
{
	//try to load xinput (1.4)
	HMODULE xinput = LoadLibraryA("xinput1_4.dll");

	if (!xinput)
	{
		//try to load xinput (1.3)
		xinput = LoadLibraryA("xinput1_3.dll");
	}

	if (!xinput)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	p_Win32Context->XInput.module = xinput;

	//get xinput functions
	p_Win32Context->XInput.enable = (XInputEnableFnc)GetProcAddress(xinput, "XInputEnable");
	p_Win32Context->XInput.getCapabilities = (XInputGetCapabilitiesFnc)GetProcAddress(xinput, "XInputGetCapabilities");
	p_Win32Context->XInput.getState = (XInputGetStateFnc)GetProcAddress(xinput, "XInputGetState");
	p_Win32Context->XInput.setState = (XInputSetStateFnc)GetProcAddress(xinput, "XInputSetState");

	if (!p_Win32Context->XInput.enable || !p_Win32Context->XInput.getCapabilities 
		|| !p_Win32Context->XInput.getState || !p_Win32Context->XInput.setState)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	//enable XInput
	p_Win32Context->XInput.enable(TRUE);

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_InternalTryLoadDirectInput(K15_Win32Context* p_Win32Context)
{
	HINSTANCE appInstance = p_Win32Context->hInstance;
	HMODULE directInput = LoadLibraryA("dinput8.dll");

	if (!directInput)
	{
		//TODO: add hint (DX missing)
		return K15_OS_ERROR_SYSTEM;
	}

	LPDIRECTINPUT directInputHandle = 0;

	p_Win32Context->DirectInput.module = directInput;
	p_Win32Context->DirectInput.directInput8Create = (DirectInput8CreateFnc)GetProcAddress(directInput, "DirectInput8Create");

	assert(p_Win32Context->DirectInput.directInput8Create);
	
	HRESULT result = p_Win32Context->DirectInput.directInput8Create(appInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&directInputHandle, 0);

	if (SUCCEEDED(result))
	{
		p_Win32Context->DirectInput.directInputHandle = directInputHandle;
	}
	else
	{
		//Add Hint
		return K15_OS_ERROR_SYSTEM;
	}


	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_InternalTryLoadDirectSound(K15_Win32Context* p_Win32Context)
{
	HMODULE directSound = LoadLibraryA("dsound.dll");

	if (!directSound)
	{
		//TODO: add hint (DX missing)
		return K15_OS_ERROR_SYSTEM;
	}

	p_Win32Context->DirectSound.module = directSound;

	p_Win32Context->DirectSound.directSoundCreate = (DirectSoundCreateFnc)GetProcAddress(directSound, "DirectSoundCreate");

	assert(p_Win32Context->DirectSound.directSoundCreate);

	return K15_SUCCESS;
}
/*********************************************************************************/
void K15_Win32AllocateDebugConsole()
{
	AllocConsole();
	long stdHandle;
	int handleConsole;
	FILE* fp;

	stdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	handleConsole = _open_osfhandle(stdHandle, _O_TEXT);
	fp = _fdopen(handleConsole, "w");
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
}
/*********************************************************************************/
uint8 K15_Win32InitializeOSLayer(HINSTANCE p_hInstance)
{
	K15_OSLayerContext win32OSContext;

	//window
	win32OSContext.window.createWindow = K15_Win32CreateWindow;
	win32OSContext.window.setWindowDimension = K15_Win32SetWindowDimension;
	win32OSContext.window.setWindowFullscreen = K15_Win32SetWindowFullscreen;
	win32OSContext.window.setWindowTitle = K15_Win32SetWindowTitle;
	win32OSContext.window.closeWindow = K15_Win32CloseWindow;
	win32OSContext.window.window = 0;

	//events
	win32OSContext.events.pumpSystemEvents = K15_Win32PumpSystemEvents;

	//threading
	win32OSContext.threading.createThread = K15_Win32CreateThread;
	win32OSContext.threading.joinThread = K15_Win32JoinThread;
	win32OSContext.threading.tryJoinThread = K15_Win32TryJoinThread;
	win32OSContext.threading.interruptThread = K15_Win32InterruptThread;
	win32OSContext.threading.setThreadName = K15_Win32SetThreadName;
	win32OSContext.threading.getCurrentThread = K15_Win32GetCurrentThread;
	win32OSContext.threading.freeThread = K15_Win32FreeThread;
	win32OSContext.threading.threads = (K15_Thread**)K15_OS_MALLOC(sizeof(K15_Thread*) * K15_MAX_THREADS);

	memset(win32OSContext.threading.threads, 0, sizeof(K15_Thread*) * K15_MAX_THREADS);

	//get current dir
	LPSTR currentDirectoryBuffer = (LPSTR)K15_OS_MALLOC(MAX_PATH);
	GetCurrentDirectoryA(MAX_PATH, currentDirectoryBuffer);

	//system
	win32OSContext.system.systemIdentifier = OS_WINDOWS;
	win32OSContext.system.sleep = K15_Win32Sleep;
	win32OSContext.system.getError = K15_Win32GetError;
	win32OSContext.system.homeDir = currentDirectoryBuffer;
	win32OSContext.system.getElapsedSeconds = K15_Win32GetElapsedSeconds;

	win32OSContext.commandLineArgCount = __argc;
	win32OSContext.commandLineArgs = __argv;
	win32OSContext.userData = 0;

	//Debug console
#ifdef K15_DEBUG
	K15_Win32AllocateDebugConsole();

	K15_LogRegisterLogFnc(K15_Win32LogVisualStudio, K15_LOG_PRIORITY_DEFAULT);
	K15_LogRegisterLogFnc(K15_Win32LogConsole, K15_LOG_PRIORITY_DEFAULT);
#endif //K15_DEBUG

	K15_Win32Context* win32SpecificContext = (K15_Win32Context*)K15_OS_MALLOC(sizeof(K15_Win32Context));
	memset(win32SpecificContext, 0, sizeof(K15_Win32Context));

	if (!win32SpecificContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	win32SpecificContext->hInstance = p_hInstance;
	win32SpecificContext->connectedController = 0;

	//assign controller index
	for (uint32 controllerIndex = 0;
		 controllerIndex < K15_MAX_CONTROLLER;
		 ++controllerIndex)
	{
		win32SpecificContext->controller[controllerIndex].controllerIndex = controllerIndex;
		win32SpecificContext->controller[controllerIndex].controllerState = K15_WIN32_CONTROLLER_STATE_NOT_CONNECTED;
		win32SpecificContext->controller[controllerIndex].APIType = K15_WIN32_APITYPE_UNDEFINED;
	}

	uint8 xInputLoaded = K15_InternalTryLoadXInput(win32SpecificContext);
	uint8 directInputLoaded = K15_InternalTryLoadDirectInput(win32SpecificContext);
	uint8 directSoundLoaded = K15_InternalTryLoadDirectSound(win32SpecificContext);

	if (xInputLoaded != K15_SUCCESS || 
		directInputLoaded != K15_SUCCESS ||
		directSoundLoaded != K15_SUCCESS)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	//frequency of performance timer
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	
	win32SpecificContext->performanceFrequency = (float)performanceFrequency.QuadPart;

	//win32 context as userdata
	win32OSContext.userData = (void*)win32SpecificContext;

	//Register Window Class
	WNDCLASS win32Class = {};
	win32Class.lpszClassName = "K15_Win32WindowClass";
	win32Class.hInstance = p_hInstance;
	win32Class.lpfnWndProc = K15_Win32WindowProc;
	win32Class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	win32Class.hCursor = LoadCursor(0,IDC_ARROW);

	if (RegisterClass(&win32Class) == 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	//get time the os layer got initialized
	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);

	win32OSContext.timeStarted = (double)(counts.QuadPart / performanceFrequency.QuadPart);

	K15_InternalSetOSLayerContext(win32OSContext);

	return K15_SUCCESS;
}
/*********************************************************************************/
char* K15_Win32GetError()
{
	DWORD errorNo = GetLastError();
	char* messageBuffer = (char*)K15_OS_MALLOC(256);

	if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorNo, LANG_ENGLISH, messageBuffer, 256, 0) == 0)
	{
		K15_OS_FREE(messageBuffer);
		messageBuffer = "Unkown Error";
	}
	
	return messageBuffer;
}
/*********************************************************************************/
void K15_Win32Sleep(double p_SleepTimeInSeconds)
{
	DWORD milliSeconds = (DWORD)(p_SleepTimeInSeconds * 1000.0);
	SleepEx(milliSeconds, TRUE);
}
/*********************************************************************************/
void K15_Win32ShutdownOSLayer()
{
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osContext->userData;

	if (win32Context->XInput.module)
	{
		FreeLibrary(win32Context->XInput.module);
		win32Context->XInput.module = 0;
	}

	if (win32Context->DirectInput.module)
	{
		FreeLibrary(win32Context->DirectInput.module);
		win32Context->DirectInput.module = 0;
	}

	if (win32Context->DirectSound.module)
	{
		FreeLibrary(win32Context->DirectSound.module);
		win32Context->DirectSound.module = 0;
	}
}
/*********************************************************************************/
double K15_Win32GetElapsedSeconds()
{
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osContext->userData;

	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);

	double seconds = counts.QuadPart / win32Context->performanceFrequency;

	return seconds;
}
/*********************************************************************************/

#endif //K15_OS_WINDOWS