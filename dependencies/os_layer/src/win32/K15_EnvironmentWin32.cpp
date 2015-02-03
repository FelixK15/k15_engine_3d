#include "win32/K15_EnvironmentWin32.h"

#include "K15_OSLayer_OSContext.h"

#include "win32/K15_WindowWin32.h"
#include "win32/K15_EventsWin32.h"

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
	win32OSContext.createWindow = K15_Win32CreateWindow;
	win32OSContext.setWindowDimension = K15_Win32SetWindowDimension;
	win32OSContext.setWindowFullscreen = K15_Win32SetWindowFullscreen;
	win32OSContext.setWindowTitle = K15_Win32SetWindowTitle;
	win32OSContext.closeWindow = K15_Win32CloseWindow;

	//events
	win32OSContext.pumpSystemEvents = K15_Win32PumpSystemEvents;


	win32OSContext.window = 0;
	win32OSContext.userData = 0;

	//Debug console
#ifdef K15_DEBUG
	K15_Win32AllocateDebugConsole();
#endif //K15_DEBUG

	K15_Win32Context* win32SpecificContext = (K15_Win32Context*)malloc(sizeof(K15_Win32Context));
	memset(win32SpecificContext, 0, sizeof(K15_Win32Context));

	if (!win32SpecificContext)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	//try to load xinput (1.3 should be supported for all necessary windows versions including windows xp)
	HMODULE xinput = LoadLibraryA("xinput1_3");

	if (!xinput)
	{
		K15_Win32ShutdownOSLayer();
		return K15_ERROR_SYSTEM;
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
		K15_Win32ShutdownOSLayer();
		return K15_ERROR_SYSTEM;
	}

	//enable XInput
	win32SpecificContext->XInput.enable(TRUE);

	//win32 context as userdata
	win32SpecificContext->hInstance = p_hInstance;

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
		return K15_ERROR_SYSTEM;
	}

	K15_InternalSetOSLayerContext(win32OSContext);

	return K15_SUCCESS;
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