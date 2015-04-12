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
	win32OSContext.threading.setThreadName = K15_Win32SetThreadName;
	win32OSContext.threading.getCurrentThread = K15_Win32GetCurrentThread;
	win32OSContext.threading.threads = (K15_Thread**)malloc(sizeof(K15_Thread*) * K15_MAX_THREADS);

	memset(win32OSContext.threading.threads, 0, sizeof(K15_Thread*) * K15_MAX_THREADS);

	//get current dir
	LPSTR currentDirectoryBuffer = (LPSTR)malloc(MAX_PATH);
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

	K15_Win32Context* win32SpecificContext = (K15_Win32Context*)malloc(sizeof(K15_Win32Context));
	memset(win32SpecificContext, 0, sizeof(K15_Win32Context));

	if (!win32SpecificContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	//try to load xinput (1.4)
	HMODULE xinput = LoadLibraryA("xinput1_4");

	if (!xinput)
	{
		//try to load xinput (1.3)
		xinput = LoadLibraryA("xinput1_3");
	}

	if (!xinput)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	win32SpecificContext->XInput.module = xinput;

	//get xinput functions
	win32SpecificContext->XInput.enable = (XInputEnableFnc)GetProcAddress(xinput, "XInputEnable");
	win32SpecificContext->XInput.getCapabilities = (XInputGetCapabilitiesFnc)GetProcAddress(xinput, "XInputGetCapabilities");
	win32SpecificContext->XInput.getState = (XInputGetStateFnc)GetProcAddress(xinput, "XInputGetState");
	win32SpecificContext->XInput.setState = (XInputSetStateFnc)GetProcAddress(xinput, "XInputSetState");

	if (!win32SpecificContext->XInput.enable || !win32SpecificContext->XInput.getCapabilities 
		|| !win32SpecificContext->XInput.getState || !win32SpecificContext->XInput.setState)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	//enable XInput
	win32SpecificContext->XInput.enable(TRUE);

	//win32 context as userdata
	win32SpecificContext->hInstance = p_hInstance;

	//frequency of performance timer
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	
	win32SpecificContext->performanceFrequency = (float)performanceFrequency.QuadPart;

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

	win32OSContext.timeStarted = (float)(counts.QuadPart / performanceFrequency.QuadPart);

	K15_InternalSetOSLayerContext(win32OSContext);

	return K15_SUCCESS;
}
/*********************************************************************************/
char* K15_Win32GetError()
{
	DWORD errorNo = GetLastError();
	char* messageBuffer = (char*)malloc(256);

	if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorNo, LANG_ENGLISH, messageBuffer, 256, 0) == 0)
	{
		free(messageBuffer);
		messageBuffer = "Unkown Error";
	}
	
	return messageBuffer;
}
/*********************************************************************************/
void K15_Win32Sleep(float p_SleepTimeInSeconds)
{
	DWORD milliSeconds = (DWORD)(p_SleepTimeInSeconds * 1000.f);
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
}
/*********************************************************************************/
float K15_Win32GetElapsedSeconds()
{
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osContext->userData;

	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);

	float seconds = counts.QuadPart / win32Context->performanceFrequency;

	return seconds;
}
/*********************************************************************************/

#endif //K15_OS_WINDOWS