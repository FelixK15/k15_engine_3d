#include "K15_GameInit.h"

#include <K15_SystemEvents.h>
#include <K15_Thread.h>

/*********************************************************************************/
uint8 K15_GameThreadMain(void* p_Parameter)
{
	K15_GameContext* gameContext = (K15_GameContext*)p_Parameter;
	byte* memory = gameContext->gameMemory;
	K15_Semaphore* mainThreadSynchronizer = gameContext->mainThreadSynchronizer;
	K15_Semaphore* gameThreadSynchronizer = gameContext->gameThreadSynchronizer;

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


		K15_PostSemaphore(gameThreadSynchronizer);
		K15_WaitSemaphore(mainThreadSynchronizer);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/