#define INITGUID

#include "win32/K15_OSContextWin32.h"

#include "K15_OSContext.h"
#include "K15_Thread.h"
#include "K15_FileSystem.h"
#include "K15_DefaultCLibraries.h"

#include "win32/K15_WindowWin32.h"
#include "win32/K15_EventsWin32.h"
#include "win32/K15_FileWatchWin32.h"
#include "win32/K15_ThreadWin32.h"
#include "win32/K15_DynamicLibraryWin32.h"	
#include "win32/K15_HelperWin32.h"
#include "win32/K15_HeaderDefaultWin32.h"
#include "win32/K15_HeaderExtensionsWin32.h"

#include <K15_Logging.h>	

#include <clocale>

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
	//p_Win32Context->DirectInput.xInputGUIDFilter = {};
	p_Win32Context->DirectInput.xInputGUIDFilterCounter = 0;
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
intern void K15_InternalAllocateDebugConsole()
{
	AllocConsole();
	long stdHandle;
	int handleConsole;
	FILE* fp;

	stdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	handleConsole = _open_osfhandle(stdHandle, 0x4000); //_O_TEXT
	fp = _fdopen(handleConsole, "w");
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
}
/*********************************************************************************/
intern void K15_InternalGetSystemInfo(K15_OSContext* p_OSContext)
{
	SYSTEM_INFO sysInfo = {};
	GetSystemInfo(&sysInfo);

	p_OSContext->threading.numHardwareThreads = sysInfo.dwNumberOfProcessors;
}
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_Win32InitializeLightOSLayer()
{
	//unicode output on debug console
	setlocale(LC_ALL, "");

	//create context
	K15_OSContext win32OSContext;

	//threading
	win32OSContext.threading.createThread = K15_Win32CreateThread;
	win32OSContext.threading.startThread = K15_Win32StartThread;
	win32OSContext.threading.joinThread = K15_Win32JoinThread;
	win32OSContext.threading.tryJoinThread = K15_Win32TryJoinThread;
	win32OSContext.threading.interruptThread = K15_Win32InterruptThread;
	win32OSContext.threading.setThreadName = K15_Win32SetThreadName;
	win32OSContext.threading.getCurrentThread = K15_Win32GetCurrentThread;
	win32OSContext.threading.freeThread = K15_Win32FreeThread;
	win32OSContext.threading.setThreadAffinityMask = K15_Win32SetThreadAffinityMask;

	//create threading stretch buffer
	K15_ThreadStretchBuffer threadBuffer = {};
	K15_CreateThreadStretchBuffer(&threadBuffer, K15_DEFAULT_THREAD_SIZE);

	win32OSContext.threading.threads = threadBuffer;

	//system
	win32OSContext.system.systemIdentifier = OS_WINDOWS;
	win32OSContext.system.homeDir = K15_Win32GetWorkingDirectory();
	win32OSContext.system.getElapsedSeconds = K15_Win32GetElapsedSeconds;
	win32OSContext.system.loadDynamicLibrary = K15_Win32LoadDynamicLibrary;
	win32OSContext.system.unloadDynamicLibrary = K15_Win32UnloadDynamicLibrary;
	win32OSContext.system.getProcAddress = K15_Win32GetProcAddress;
	win32OSContext.system.registerFileWatch = K15_Win32RegisterFileWatch;

	//create dynamic library stretch buffer
	K15_DynamicLibraryStretchBuffer dynamicLibraryBuffer = {};
	K15_CreateDynamicLibraryStretchBuffer(&dynamicLibraryBuffer, K15_DEFAULT_DYNAMIC_LIBRARY_SIZE);

	win32OSContext.system.dynamicLibraries = dynamicLibraryBuffer;

	//create directory watch entry stretch buffer
	K15_DirectoryWatchEntryStretchBuffer directoryWatchBuffer = {};
	K15_CreateDirectoryWatchEntryStretchBuffer(&directoryWatchBuffer);

	win32OSContext.system.directoryWatchEntries = directoryWatchBuffer;

	//arguments
	win32OSContext.commandLineArgCount = __argc;
	win32OSContext.commandLineArgs = __argv;
	win32OSContext.userData = 0;

	K15_Win32Context* win32SpecificContext = (K15_Win32Context*)K15_OS_MALLOC(sizeof(K15_Win32Context));
	memset(win32SpecificContext, 0, sizeof(K15_Win32Context));

	//frequency of performance timer
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);

	win32SpecificContext->performanceFrequency = (float)performanceFrequency.QuadPart;

	//win32 context as userdata
	win32OSContext.userData = (void*)win32SpecificContext;

	//get time the os layer got initialized
	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);

	win32OSContext.timeStarted = (double)(counts.QuadPart / performanceFrequency.QuadPart);

	K15_InternalGetSystemInfo(&win32OSContext);
	K15_InternalSetOSLayerContext(win32OSContext);
		
	K15_RegisterDefaultLog(K15_LOG_FLAG_ADD_TIME | K15_LOG_FLAG_ADD_NEW_LINE);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32InitializeOSLayer(HINSTANCE p_hInstance)
{
	if (!p_hInstance)
	{
		p_hInstance = GetModuleHandle(NULL);
	}

	//unicode output on debug console
	setlocale(LC_ALL, "");

	//create context
	K15_OSContext win32OSContext;

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
	win32OSContext.threading.startThread = K15_Win32StartThread;
	win32OSContext.threading.joinThread = K15_Win32JoinThread;
	win32OSContext.threading.tryJoinThread = K15_Win32TryJoinThread;
	win32OSContext.threading.interruptThread = K15_Win32InterruptThread;
	win32OSContext.threading.setThreadName = K15_Win32SetThreadName;
	win32OSContext.threading.getCurrentThread = K15_Win32GetCurrentThread;
	win32OSContext.threading.freeThread = K15_Win32FreeThread;
	win32OSContext.threading.setThreadAffinityMask = K15_Win32SetThreadAffinityMask;

	//create threading stretch buffer
	K15_ThreadStretchBuffer threadBuffer = {};
	K15_CreateThreadStretchBuffer(&threadBuffer, K15_DEFAULT_THREAD_SIZE);

	win32OSContext.threading.threads = threadBuffer;

	//system
	win32OSContext.system.systemIdentifier = OS_WINDOWS;
	win32OSContext.system.homeDir = K15_Win32GetWorkingDirectory();
	win32OSContext.system.getElapsedSeconds = K15_Win32GetElapsedSeconds;
	win32OSContext.system.loadDynamicLibrary = K15_Win32LoadDynamicLibrary;
	win32OSContext.system.unloadDynamicLibrary = K15_Win32UnloadDynamicLibrary;
	win32OSContext.system.getProcAddress = K15_Win32GetProcAddress;
	win32OSContext.system.registerFileWatch = K15_Win32RegisterFileWatch;

	//create dynamic library stretch buffer
	K15_DynamicLibraryStretchBuffer dynamicLibraryBuffer = {};
	K15_CreateDynamicLibraryStretchBuffer(&dynamicLibraryBuffer, K15_DEFAULT_DYNAMIC_LIBRARY_SIZE);

	win32OSContext.system.dynamicLibraries = dynamicLibraryBuffer;

	//create directory watch entry stretch buffer
	K15_DirectoryWatchEntryStretchBuffer directoryWatchBuffer = {};
	K15_CreateDirectoryWatchEntryStretchBuffer(&directoryWatchBuffer);

	win32OSContext.system.directoryWatchEntries = directoryWatchBuffer;
	
	//arguments
	win32OSContext.commandLineArgCount = __argc;
	win32OSContext.commandLineArgs = __argv;
	win32OSContext.userData = 0;

	//Debug console
#ifdef K15_DEBUG
	K15_InternalAllocateDebugConsole();

	K15_RegisterDefaultLog(K15_LOG_FLAG_ADD_TIME);
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

	//get hardware information
	K15_InternalGetSystemInfo(&win32OSContext);

	//battery
	GetSystemPowerStatus(&win32SpecificContext->Battery.powerStatus);

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
char* K15_Win32GetError(char* p_OutputBuffer)
{
	DWORD errorNo = GetLastError();
	wchar_t* messageBuffer = (wchar_t*)alloca(256 * sizeof(wchar_t));

	if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorNo, 0, messageBuffer, 256, 0) == 0)
	{
		//K15_OS_FREE(messageBuffer);
		messageBuffer = L"Unkown Error";
	}

	uint32 messageBufferLength = (uint32)wcslen(messageBuffer) + 1; //+1 for 0 terminator

	K15_Win32ConvertWStringToString(messageBuffer, messageBufferLength, p_OutputBuffer);
	
	return p_OutputBuffer;
}
/*********************************************************************************/
void K15_Win32Sleep(double p_SleepTimeInSeconds)
{
	DWORD milliSeconds = (DWORD)(p_SleepTimeInSeconds * 1000.0);
	SleepEx(milliSeconds, K15_TRUE);
}
/*********************************************************************************/
void K15_Win32ShutdownOSLayer()
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
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
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osContext->userData;

	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);

	double seconds = counts.QuadPart / win32Context->performanceFrequency;

	return seconds;
}
/*********************************************************************************/