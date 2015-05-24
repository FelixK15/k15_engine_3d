#include <K15_Prerequisites.h>
#include <K15_GameInit.h>

#include <K15_Logging.h>

#include <cstdio>

#define K15_GAME_MEMORY_USAGE size_megabyte(20)

/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_InitGame(K15_InitGameInputData* p_InputData, K15_InitGameOutputData* p_OutputData)
{
	p_OutputData->requestedMemorySize = K15_GAME_MEMORY_USAGE;
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_TickGame(K15_GameContext* p_GameContext)
{

}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_QuitGame()
{

}
/*********************************************************************************/