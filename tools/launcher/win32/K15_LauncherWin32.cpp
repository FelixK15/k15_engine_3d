#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

//#include <K15_Application.h>

#include <K15_RenderContext.h>
#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <K15_OSLayer_SystemEvents.h>
#include <K15_OSLayer_Thread.h>
#include <K15_OSLayer_System.h>
#include <K15_Logging.h>


#include <K15_RenderBufferDesc.h>
#include <K15_RenderProgramDesc.h>

#ifdef K15_OS_WINDOWS

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

	K15_RenderProgramDesc shaderDesc = {0};
	K15_RenderProgramHandle shaderHandle;

	shaderDesc.file = "../../../../test/res/default.vert";
	shaderDesc.code = "code";
	shaderDesc.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
	shaderDesc.source = K15_RENDER_PROGRAM_SOURCE_FILE;

	K15_BeginRenderCommand(renderCommandQueue, K15_RENDER_COMMAND_CREATE_PROGRAM);
	K15_AddRenderProgramHandleParameter(renderCommandQueue, &shaderHandle);
	K15_AddRenderProgramDescParameter(renderCommandQueue, &shaderDesc);
	K15_EndRenderCommand(renderCommandQueue);

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
	
		K15_DispatchRenderCommandQueue(renderCommandQueue);

		K15_ProcessDispatchedRenderCommandQueues(renderContext);

		//K15_SleepThreadForSeconds(0.014f);
	}

// 	K15_Engine::Application application;
// 	application.initialize(__argc, __argv);
// 	application.run();
// 	application.shutdown();

	return 0;
}

#endif //K15_OS_WINDOWS