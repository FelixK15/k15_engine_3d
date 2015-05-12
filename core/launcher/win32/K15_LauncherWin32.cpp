#define _CRT_SECURE_NO_WARNINGS

#include <K15_RenderContext.h>
#include <K15_Logging.h>

#include <K15_Window.h>
#include <K15_OSContext.h>
#include <K15_SystemEvents.h>

#include <K15_FileSystem.h>

#include <win32/K15_OSContextWin32.h>

#include <K15_DefaultCLibraries.h>

/*********************************************************************************/
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int returnValue = 0;

	uint8 result = K15_Win32InitializeOSLayer(hInstance);

	if (result == K15_OS_ERROR_OUT_OF_MEMORY)
	{
		K15_LOG_ERROR_MESSAGE("Out of memory!");
	}
	else if(result == K15_OS_ERROR_SYSTEM)
	{
		char* errorMessage = K15_Win32GetError();
		K15_LOG_ERROR_MESSAGE(errorMessage);
		free(errorMessage);
	}

	K15_Window* window = K15_CreateWindow(0, 0);
	K15Context* osContext = K15_GetOSLayerContext();

	K15_SetWindowDimension(window, 1024, 768);
	
	K15_RenderContext* renderContext = K15_CreateRenderContext(osContext);

	bool running = true;

#ifdef K15_USE_DYNAMIC_GAME_LIBRARY
	K15_LoadGameLibrary();

#endif //K15_USE_DYNAMIC_GAME_LIBRARY


	K15_SystemEvent event = {};

	while (running)
	{
		K15_PumpSystemEvents();

		while (K15_GetSystemEventFromQueue(&event, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
			if (event.event == K15_APPLICATION_QUIT)
			{
				running = false;
			}
		}		

		K15_ProcessDispatchedRenderCommandBuffers(renderContext);
	}

	return 0;
}
