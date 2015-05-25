#include <K15_Prerequisites.h>
#include <K15_GameInit.h>

#include <K15_OSContext.h>
#include <K15_Logging.h>

#define K15_GAME_MEMORY_USAGE size_megabyte(20)

intern uint8 g_Initialized = K15_FALSE;

/*********************************************************************************/
intern inline void K15_InternalSetGameContext(K15_GameContext* p_GameContext)
{
	uint32 numLogContexts = p_GameContext->logContextCount;

	K15_SetLogContexts(p_GameContext->logContexts, numLogContexts);
	//K15_InternalSetOSLayerContext(*p_GameContext->osContext);
}
/*********************************************************************************/



/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_InitGame(K15_InitGameInputData* p_InputData, K15_InitGameOutputData* p_OutputData)
{
	p_OutputData->requestedMemorySize = K15_GAME_MEMORY_USAGE;
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_TickGame(K15_GameContext* p_GameContext)
{
	if (g_Initialized == K15_FALSE)
	{
		K15_InternalSetGameContext(p_GameContext);
		g_Initialized = K15_TRUE;
	}

// 	K15_ResourceContext* resourceContext;
// 
// 	K15_FutureResource* resourceFuture = K15_LoadResourceAsync(resourceContext, "data/robbie/Robbie_the_rabbit_rigged.k15mesh");
// 
// 	if (resourceFuture)
// 	{
// 
// 	}
// 	K15_Resource* robbieMesh = K15_LoadResource(resourceContext, "data/robbie/Robbie_the_rabbit_rigged.k15mesh");

	K15_LOG_WARNING_MESSAGE("test!");
	K15_LOG_WARNING_MESSAGE("test2!");
	K15_LOG_WARNING_MESSAGE("test3!");
	K15_LOG_WARNING_MESSAGE("test4!");
	K15_LOG_WARNING_MESSAGE("test5!");
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_QuitGame()
{

}
/*********************************************************************************/