#include <K15_RenderContext.h>
#include <K15_Logging.h>

#include <K15_Window.h>
#include <K15_OSContext.h>
#include <K15_SystemEvents.h>
#include <K15_DynamicLibrary.h>
#include <K15_FileWatch.h>
#include <K15_System.h>

#include <K15_FrameCounter.h>

#include <K15_FileSystem.h>

#include <win32/K15_OSContextWin32.h>

#include <K15_AsyncOperation.h>
#include <K15_MemoryBuffer.h>
#include <K15_GameInit.h>
#include <K15_DefaultCLibraries.h>
#include <K15_ConfigFile.h>
#include <K15_Thread.h>

#ifndef K15_LOAD_GAME_LIB_DYNAMIC
	K15_EXPORT_SYMBOL void K15_InitGame(K15_InitGameInputData, K15_InitGameOutputData*);
	K15_EXPORT_SYMBOL void K15_TickGame(K15_GameContext*);
	K15_EXPORT_SYMBOL void K15_QuitGame(void);
	K15_EXPORT_SYMBOL void K15_OnSystemEvent(K15_GameContext*, K15_SystemEvent*);
	K15_EXPORT_SYMBOL void K15_OnWindowEvent(K15_GameContext*, K15_SystemEvent*);
	K15_EXPORT_SYMBOL void K15_OnInputEvent(K15_GameContext*, K15_SystemEvent*);
#endif //K15_LOAD_GAME_LIB_DYNAMIC

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
/*********************************************************************************/
intern void K15_InternalOnGameLibraryReload(void* p_UserData)
{
	K15_GameContext* gameContext = (K15_GameContext*)p_UserData;
	K15_DynamicLibrary* gameLibrary = gameContext->gameLibrary;
	K15_Mutex* gameLibrarySynchronizer = gameContext->gameLibrarySynchronizer;

	K15_LockMutex(gameLibrarySynchronizer);

	K15_ReloadDynamicLibrary(gameLibrary);

	gameContext->tickFnc = (K15_TickGameFnc)K15_GetProcAddress(gameLibrary, "K15_TickGame");
	gameContext->onSystemEventFnc = (K15_TickGameFnc)K15_GetProcAddress(gameLibrary, "K15_OnSystemEvent");
	gameContext->onWindowEventFnc = (K15_TickGameFnc)K15_GetProcAddress(gameLibrary, "K15_OnWindowEvent");
	gameContext->onInputEventFnc = (K15_TickGameFnc)K15_GetProcAddress(gameLibrary, "K15_OnInputEvent");

	K15_UnlockMutex(gameLibrarySynchronizer);
}
/*********************************************************************************/
#endif //K15_LOAD_GAME_LIB_DYNAMIC

/*********************************************************************************/
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int returnValue = 0;

	uint8 result = K15_Win32InitializeOSLayer(hInstance);
	K15_ParseCommandLineArguments();

	if (result == K15_OS_ERROR_OUT_OF_MEMORY)
	{
		K15_LOG_ERROR_MESSAGE("Out of memory!");
	}
	else if(result == K15_OS_ERROR_SYSTEM)
	{
		char* errorMessage = K15_Win32GetError((char*)alloca(K15_ERROR_MESSAGE_LENGTH));
		K15_LOG_ERROR_MESSAGE(errorMessage);
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

	K15_GameContext gameContext = {};
	K15_InitGameInputData inputData = {};
	K15_InitGameOutputData outputData = {};

	inputData.osContext = osContext;
	inputData.renderContext = renderContext;
	inputData.configContext = &configFileContext;

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
	const char* gameLibraryName = "data/testgame";
	K15_DynamicLibrary* gameLibrary = K15_LoadDynamicLibrary(gameLibraryName, K15_RELOADABLE_LIBRARY);
	K15_InitGameFnc K15_InitGame = (K15_InitGameFnc)K15_GetProcAddress(gameLibrary, "K15_InitGame");
	K15_TickGameFnc K15_TickGame = (K15_TickGameFnc)K15_GetProcAddress(gameLibrary, "K15_TickGame");
	K15_QuitGameFnc K15_QuitGame = (K15_QuitGameFnc)K15_GetProcAddress(gameLibrary, "K15_QuitGame");
	K15_OnSystemEventFnc K15_OnSystemEvent = (K15_OnSystemEventFnc)K15_GetProcAddress(gameLibrary, "K15_OnSystemEvent");
	K15_OnInputEventFnc K15_OnInputEvent = (K15_OnInputEventFnc)K15_GetProcAddress(gameLibrary, "K15_OnInputEvent");
	K15_OnWindowEventFnc K15_OnWindowEvent = (K15_OnWindowEventFnc)K15_GetProcAddress(gameLibrary, "K15_OnWindowEvent");

	K15_Mutex* gameLibrarySynchronizer = K15_CreateMutex();

	K15_AddFileWatch(gameLibrary->originalSystemPath, K15_InternalOnGameLibraryReload, &gameContext);
#endif //K15_LOAD_GAME_LIB_DYNAMIC

	K15_InitGame(inputData, &outputData);

	uint32 requestedMemorySize = outputData.requestedMemorySize;

#ifdef K15_USE_DETERMINISTIC_GAME_MEM_ADDRESS
	byte* memoryBlock = (byte*)VirtualAlloc((LPVOID)(0x0800000000), requestedMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	byte* memoryBlock = (byte*)malloc(requestedMemorySize);
#endif //K15_USE_DETERMINISTIC_GAME_MEM_ADDRESS

	K15_MemoryBuffer gameMemory = {};
	K15_InitializePreallocatedMemoryBuffer(&gameMemory, memoryBlock, requestedMemorySize, 0);

	gameContext.asyncContext = K15_CreateAsyncContext(osContext);
	gameContext.gameMemory = &gameMemory;
	gameContext.logContexts = K15_GetLogContexts(&gameContext.logContextCount);
	gameContext.configContext = &configFileContext;
	gameContext.renderContext = renderContext;
	gameContext.osContext = osContext;
	gameContext.gameThreadSynchronizer = gameThreadSynchronizer;
	gameContext.mainThreadSynchronizer = mainThreadSynchronizer;
	gameContext.tickFnc = K15_TickGame;
	gameContext.onInputEventFnc = K15_OnInputEvent;
	gameContext.onSystemEventFnc = K15_OnSystemEvent;
	gameContext.onWindowEventFnc = K15_OnWindowEvent;

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
	gameContext.gameLibrary = gameLibrary;
	gameContext.gameLibrarySynchronizer = gameLibrarySynchronizer;
#endif //K15_LOAD_GAME_LIB_DYNAMIC

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

		//let game run for 1 frame
		K15_PostSemaphore(mainThreadSynchronizer);

		//TODO
		//start processing dispatched render buffer
		K15_ProcessDispatchedRenderCommandQueues(renderContext);

		joinResult = K15_TryJoinThread(gameThread, 0);

		running = joinResult != K15_SUCCESS;
	}

	return 0;
}
/*********************************************************************************/