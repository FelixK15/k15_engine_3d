#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

//#include <K15_Application.h>

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <K15_OSLayer_SystemEvents.h>
#include <K15_Logging.h>
#include <win32/K15_EnvironmentWin32.h>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int returnValue = 0;

	if (K15_Win32InitializeOSLayer(hInstance) != K15_SUCCESS)
	{

	}

	K15_Window* window = K15_CreateWindow();

	K15_SetWindowDimension(window, 1024, 768);
	
	while(true)
	{
		K15_PumpSystemEvents();
		K15_SystemEvent event;

		while(K15_GetSystemEventFromQueue(&event, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
			K15_LOG_NORMAL_MESSAGE("%u\n", event.event);
		}

		K15_SynchronizeSystemEventQueues();
	}



// 	K15_Engine::Application application;
// 	application.initialize(__argc, __argv);
// 	application.run();
// 	application.shutdown();

	return 0;
}
