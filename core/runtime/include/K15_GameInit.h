#ifndef _K15_Runtime_Game_Init_h_
#define _K15_Runtime_Game_Init_h_

#include "K15_Prerequisites.h"

struct K15_RenderContext;
struct K15_ConfigFileContext;
struct K15_InitGameInputData;
struct K15_InitGameOutputData;
struct K15_GameContext;

typedef void(*K15_InitGameFnc)(K15_InitGameInputData, K15_InitGameOutputData*);
typedef void(*K15_TickGameFnc)(K15_GameContext*);
typedef void(*K15_QuitGameFnc)(void);

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
	byte* gameMemory;
	K15_TickGameFnc tickFnc;
	K15_OSContext* osContext;
	K15_RenderContext* renderContext;
	K15_ConfigFileContext* configContext;
	K15_Semaphore* gameThreadSynchronizer;
	K15_Semaphore* mainThreadSynchronizer;
};
/*********************************************************************************/

uint8 K15_GameThreadMain(void* p_Parameter);
uint8 K15_LoadGameLibrary(const char* p_LibraryPath);

#endif //_K15_Runtime_Game_Init_h_