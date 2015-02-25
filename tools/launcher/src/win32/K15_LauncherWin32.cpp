#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

//#include <K15_Application.h>

#include <K15_RenderContext.h>
#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <K15_OSLayer_SystemEvents.h>
#include <K15_Logging.h>
#include <win32/K15_EnvironmentWin32.h>

#include <thread>

void renderThreadFunc(K15_RenderContext* renderContext, K15_RenderCommandQueue* renderCommandQueue, K15_OSLayerContext* p_OSContext)
{
	K15_CreateRenderContext(renderContext, p_OSContext);
	K15_CreateRenderCommandQueue(renderCommandQueue, renderContext);

	K15_GpuBufferHandle vbo, ibo;
	int32 vboType = K15_GPU_BUFFER_TYPE_VERTEX;
	int32 iboType = K15_GPU_BUFFER_TYPE_INDEX;

	uint32 vboSize = size_megabyte(2);

	K15_BeginRenderCommand(renderCommandQueue, K15_RENDER_COMMAND_CREATE_BUFFER);
	K15_AddRenderBufferHandleParameter(renderCommandQueue, &vbo);
	K15_AddRenderInt32Parameter(renderCommandQueue, &vboType);
	K15_AddRenderUInt32Parameter(renderCommandQueue, &vboSize);
	K15_EndRenderCommand(renderCommandQueue);

	K15_BeginRenderCommand(renderCommandQueue, K15_RENDER_COMMAND_CREATE_BUFFER);
	K15_AddRenderBufferHandleParameter(renderCommandQueue, &ibo);
	K15_AddRenderInt32Parameter(renderCommandQueue, &iboType);
	K15_AddRenderUInt32Parameter(renderCommandQueue, &vboSize);
	K15_EndRenderCommand(renderCommandQueue);

	while(true)
	{
		K15_ProcessRenderCommandQueue(renderContext, renderCommandQueue);
	}
}

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

	K15_Window* window = K15_CreateWindow(K15_WINDOW_FLAG_BORDERLESS | K15_WINDOW_FLAG_NO_BUTTONS | K15_WINDOW_FLAG_DEFAULT_DIMENISON, 0);
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	K15_SetWindowDimension(window, 1024, 768);
	
	K15_RenderContext renderContext;
	K15_RenderCommandQueue renderCommandQueue;

	std::thread renderThread(renderThreadFunc, &renderContext, &renderCommandQueue, osContext);

	bool running = true;

	K15_SystemEvent event = {};

	Sleep(2000);

	while (running)
	{
		K15_PumpSystemEvents();

		while (K15_GetSystemEventFromQueue(&event, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
	
		}

		K15_BeginRenderCommand(&renderCommandQueue, K15_RENDER_COMMAND_CLEAR_SCREEN);
		K15_EndRenderCommand(&renderCommandQueue);


		K15_UnlockRenderCommandQueue(&renderCommandQueue);

		printf("bla\n");
	}

// 	K15_Engine::Application application;
// 	application.initialize(__argc, __argv);
// 	application.run();
// 	application.shutdown();

	return 0;
}
