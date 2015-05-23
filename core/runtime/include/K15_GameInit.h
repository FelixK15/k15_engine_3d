#ifndef _K15_Runtime_Game_Init_h_
#define _K15_Runtime_Game_Init_h_

#include "K15_Prerequisites.h"

struct K15_RenderContext;
struct K15_ConfigFileContext;

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
	K15_OSContext* osContext;
	K15_RenderContext* renderContext;
	K15_ConfigFileContext* configContext;
	K15_Semaphore* gameThreadSynchronizer;
	K15_Semaphore* mainThreadSynchronizer;
};
/*********************************************************************************/

typedef void(*K15_GameInitFnc)(K15_InitGameInputData p_InputData, K15_InitGameOutputData* p_OutputData);

uint8 K15_GameThreadMain(void* p_Parameter);
uint8 K15_LoadGameLibrary(const char* p_LibraryPath);

#endif //_K15_Runtime_Game_Init_h_