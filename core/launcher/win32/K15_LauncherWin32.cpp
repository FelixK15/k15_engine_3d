#include <K15_RenderContext.h>
#include <K15_Logging.h>

#include <K15_Window.h>
#include <K15_OSContext.h>
#include <K15_SystemEvents.h>
#include <K15_DynamicLibrary.h>
#include <K15_System.h>

#include <K15_FileSystem.h>

#include <win32/K15_OSContextWin32.h>

#include <K15_GameInit.h>
#include <K15_DefaultCLibraries.h>
#include <K15_ConfigFile.h>
#include <K15_Thread.h>

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

		K15_ASSERT_TEXT(false, errorMessage);
		exit(0);
	}

	K15_ConfigFileContext configFileContext = {};
	K15_LoadConfigFile("data/config.ini", &configFileContext);

	K15_Window* window = K15_CreateWindow(0, 0);
	K15_OSContext* osContext = K15_GetOSLayerContext();

	K15_SetWindowDimension(window, 1024, 768);
	
	K15_RenderContext* renderContext = K15_CreateRenderContext(osContext);

	K15_Semaphore* gameThreadSynchronizer = K15_CreateSemaphoreWithInitialValue(0);
	K15_Semaphore* mainThreadSynchronizer = K15_CreateSemaphoreWithInitialValue(0);
	
	bool running = true;

	K15_InitGameInputData inputData = {};
	K15_InitGameOutputData outputData = {};

	inputData.osContext = osContext;
	inputData.renderContext = renderContext;
	inputData.configContext = &configFileContext;

	K15_DynamicLibrary* gameLibrary = K15_LoadDynamicLibrary("data/testgame");
	K15_GameInitFnc K15_InitGame = (K15_GameInitFnc)K15_GetProcAddress(gameLibrary, "K15_InitGame");

	K15_ASSERT_TEXT(gameLibrary, "Could not load gamelib");

	K15_InitGame(inputData, &outputData);

	uint32 requestedMemorySize = outputData.requestedMemorySize;

#ifdef K15_USE_DETERMINISTIC_GAME_MEM_ADDRESS
	byte* memoryBlock = (byte*)VirtualAlloc((LPVOID)(0x0800000000), requestedMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	byte* memoryBlock = (byte*)K15_GAME_MALLOC(requestedMemorySize);
#endif //K15_USE_DETERMINISTIC_GAME_MEM_ADDRESS

	K15_GameContext gameContext = {};
	gameContext.gameMemory = memoryBlock;
	gameContext.configContext = &configFileContext;
	gameContext.renderContext = renderContext;
	gameContext.osContext = osContext;
	gameContext.gameThreadSynchronizer = gameThreadSynchronizer;
	gameContext.mainThreadSynchronizer = mainThreadSynchronizer;

	K15_Thread* gameThread = K15_CreateThread(K15_GameThreadMain, (void*)&gameContext, K15_THREAD_START_FLAG);

	K15_SystemEvent event = {};

	uint8 joinResult = 0;
	uint8 waitGameSyncResult = 0;

	while (running)
	{
		K15_PumpSystemEvents();

		//wait until semaphore is 0
		waitGameSyncResult = K15_TryWaitSemaphore(gameThreadSynchronizer, 0);

		if (waitGameSyncResult == K15_OS_TIMEOUT)
		{
			//cpu bound
			K15_WaitSemaphore(gameThreadSynchronizer);
		}
		else
		{
			//gpu bound
		}

		K15_PostSemaphore(mainThreadSynchronizer);

		K15_ProcessDispatchedRenderCommandBuffers(renderContext);

		joinResult = K15_TryJoinThread(gameThread, 0);

		running = joinResult != K15_SUCCESS;
	}

	return 0;
}
/*********************************************************************************/