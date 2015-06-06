#include "K15_GameInit.h"

#include <K15_DynamicLibrary.h>
#include <K15_SystemEvents.h>
#include <K15_Thread.h>

///*********************************************************************************/
uint8 K15_GameThreadMain(void* p_Parameter)
{
	K15_GameContext* gameContext = (K15_GameContext*)p_Parameter;
	K15_MemoryBuffer* memory = gameContext->gameMemory;
	K15_Semaphore* mainThreadSynchronizer = gameContext->mainThreadSynchronizer;
	K15_Semaphore* gameThreadSynchronizer = gameContext->gameThreadSynchronizer;

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
	K15_Mutex* gameLibrarySynchronizer = gameContext->gameLibrarySynchronizer;
#endif //K15_LOAD_GAME_LIB_DYNAMIC

	K15_TickGameFnc K15_TickGame = gameContext->tickFnc;

	bool running = true;

	K15_SystemEvent event = {};

	while(running)
	{
		while (K15_GetSystemEventFromQueue(&event, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
			if (event.event == K15_APPLICATION_QUIT)
			{
				running = false;
			}
		}

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
		K15_LockMutex(gameLibrarySynchronizer);
		K15_TickGame = gameContext->tickFnc;
#endif //K15_LOAD_GAME_LIB_DYNAMIC

		K15_TickGame(gameContext);

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
		K15_UnlockMutex(gameLibrarySynchronizer);
#endif //K15_LOAD_GAME_LIB_DYNAMIC

		K15_PostSemaphore(gameThreadSynchronizer);
		K15_WaitSemaphore(mainThreadSynchronizer);
	}

	//increment semaphore on exit. Some systems close the thread deferred :(
	K15_PostSemaphore(gameThreadSynchronizer);

	return K15_SUCCESS;
}
/*********************************************************************************/