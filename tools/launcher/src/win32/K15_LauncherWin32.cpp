#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

//#include <K15_Application.h>

#include <K15_RenderContext.h>
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

	uint8 result = K15_Win32InitializeOSLayer(hInstance);

	if (result == K15_ERROR_OUT_OF_MEMORY)
	{
		K15_LOG_ERROR_MESSAGE("Out of memory!");
	}
	else if(result == K15_ERROR_SYSTEM)
	{
		char* errorMessage = K15_Win32GetError();
		K15_LOG_ERROR_MESSAGE(errorMessage);
		free(errorMessage);
	}

	K15_Window* window = K15_CreateWindow(0, 0);
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	K15_SetWindowDimension(window, 1024, 768);
	
	K15_RenderContext* renderContext = K15_CreateRenderContext(osContext);
	K15_RenderCommandQueue* renderCommandQueue = K15_CreateRenderCommandQueue(renderContext);

	//std::thread renderThread(renderThreadFunc, &renderContext, &renderCommandQueue, osContext);

	bool running = true;

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

		K15_BeginRenderCommand(renderCommandQueue, K15_RENDER_COMMAND_CLEAR_SCREEN);
		K15_EndRenderCommand(renderCommandQueue);


		K15_UnlockRenderCommandQueue(renderCommandQueue);
	
		osContext->system.sleep(0.016f);
	}

// 	K15_Engine::Application application;
// 	application.initialize(__argc, __argv);
// 	application.run();
// 	application.shutdown();

	return 0;
}
