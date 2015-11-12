#ifndef _K15_Runtime_Game_Init_h_
#define _K15_Runtime_Game_Init_h_

#include "K15_RuntimePrerequisites.h"
#include "K15_MemoryBuffer.h"
<<<<<<< HEAD
#include "K15_FrameCounter.h"
=======
>>>>>>> 3630a7e93c86249517bb3dc3fffae82c960d0f0d

struct K15_RenderContext;
struct K15_ConfigFileContext;
struct K15_InitGameInputData;
struct K15_InitGameOutputData;
struct K15_GameContext;
struct K15_LogContext;
struct K15_SystemEvent;
struct K15_AsyncContext;

typedef void(*K15_InitGameFnc)(K15_InitGameInputData, K15_InitGameOutputData*);
typedef void(*K15_TickGameFnc)(K15_GameContext*);
typedef void(*K15_OnSystemEventFnc)(K15_GameContext*, K15_SystemEvent*);
typedef void(*K15_OnInputEventFnc)(K15_GameContext*, K15_SystemEvent*);
typedef void(*K15_OnWindowEventFnc)(K15_GameContext*, K15_SystemEvent*);

typedef void(*K15_QuitGameFnc)(K15_GameContext*);

/*********************************************************************************/
struct K15_InitGameInputData
{
	K15_OSContext* osContext;
	K15_RenderContext* renderContext;
	K15_ConfigFileContext* configContext;
};
/*********************************************************************************/
struct K15_InitGameOutputData
{
	uint32 requestedMemorySize;
};
/*********************************************************************************/
struct K15_GameContext
{
	uint32 logContextCount;

	K15_MemoryBuffer gameMemory;
	K15_AsyncContext* asyncContext;
	K15_TickGameFnc tickFnc;
	K15_OnSystemEventFnc onSystemEventFnc;
	K15_OnInputEventFnc onInputEventFnc;
	K15_OnWindowEventFnc onWindowEventFnc;
	K15_LogContext* logContexts;
	K15_OSContext* osContext;
	K15_RenderContext* renderContext;
	K15_ConfigFileContext* configContext;
	K15_Semaphore* gameThreadSynchronizer;
	K15_Semaphore* mainThreadSynchronizer;
	K15_FrameCounter frameCounter;

	void* userData;

	void* userData;

#ifdef K15_LOAD_GAME_LIB_DYNAMIC
	K15_DynamicLibrary* gameLibrary;
	K15_Mutex* gameLibrarySynchronizer;
#endif //K15_LOAD_GAME_LIB_DYNAMIC
};
/*********************************************************************************/

uint8 K15_GameThreadMain(void* p_Parameter);
uint8 K15_LoadGameLibrary(const char* p_LibraryPath);

#endif //_K15_Runtime_Game_Init_h_