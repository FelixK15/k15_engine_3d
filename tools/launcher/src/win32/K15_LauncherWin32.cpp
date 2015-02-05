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

	K15_Window* window = K15_CreateWindow(K15_WINDOW_FLAG_BORDERLESS | K15_WINDOW_FLAG_NO_BUTTONS | K15_WINDOW_FLAG_DEFAULT_DIMENISON, 1);
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	K15_SetWindowDimension(window, 1024, 768);
	
	K15_InitializeCore(osContext);
	K15_RenderContext* renderContext = K15_InitializeRenderer();

	K15_RenderCommandQueue* renderCommandQueue = K15_CreateRenderCommandQueue(renderContext);

	K15_BeginRenderCommand(renderCommandQueue, K15_COMMAND_DRAW_INSTANCES);
	K15_AddRenderInt32Parameter(renderCommandQueue, 32);
	K15_EndRenderCommand(renderCommandQueue);

	bool running = true;

	K15_SystemEvent event = {};

	while (running)
	{
		K15_PumpSystemEvents();

		while (K15_GetSystemEventFromQueue(&event, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
			switch(event.event)
			{
			case K15_APPLICATION_QUIT:
				{
					running = false;
					break;
				}

			case K15_KEY_PRESSED:
				{
					K15_KeyPressedCore();
					break;
				}

			case K15_KEY_RELEASED:
				{
					K15_KeyReleasedCore();
					break;
				}

			case K15_MOUSE_MOVED:
				{
					K15_MouseMoveCore();
					break;
				}

			case K15_MOUSE_BUTTON_PRESSED:
				{
					K15_MousePressedCore();
					break;
				}

			case K15_WINDOW_FOCUS_GAINED:
				{
					K15_WindowFocusGainedCore();
					break;
				}

			case K15_WINDOW_FOCUS_LOST:
				{
					//K15_WindowFocusLost
				}
			}
		}

		K15_TickCore();
	}

	K15_ShutdownCore();

// 	K15_Engine::Application application;
// 	application.initialize(__argc, __argv);
// 	application.run();
// 	application.shutdown();

	return 0;
}
