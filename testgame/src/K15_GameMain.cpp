#include <K15_Prerequisites.h>
#include <K15_GameInit.h>

#include <K15_OSContext.h>
#include <K15_Logging.h>

#include <K15_ResourceContext.h>
#include <K15_MeshFormat.h>
#include <K15_MemoryBuffer.h>

#define K15_GAME_MEMORY_USAGE size_megabyte(20)

struct K15_Sample1GameContext
{
	K15_ResourceContext* resourceContext;
	K15_Resource* robbie;
};

intern uint8 g_Initialized = K15_FALSE;

/*********************************************************************************/
intern inline void K15_InternalSetGameContext(K15_GameContext* p_GameContext)
{
	uint32 numLogContexts = p_GameContext->logContextCount;

	K15_SetLogContexts(p_GameContext->logContexts, numLogContexts);
	K15_InternalSetOSLayerContext(*p_GameContext->osContext);

	K15_MemoryBuffer* memory = p_GameContext->gameMemory;

	uint32 resourceBufferSize = size_megabyte(5);

	K15_Sample1GameContext* sample1GameContext = (K15_Sample1GameContext*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_Sample1GameContext));
	K15_ResourceContext* resourceContext = (K15_ResourceContext*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_ResourceContext));
	sample1GameContext->resourceContext = resourceContext;

	K15_MemoryBuffer* resourceMemoryBuffer = (K15_MemoryBuffer*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_MemoryBuffer));

	K15_InitializeDefaultPreallocatedMemoryBuffer(resourceMemoryBuffer, K15_GetMemoryFromMemoryBuffer(memory, resourceBufferSize), resourceBufferSize, 0);

	K15_InitializeResourceContext(resourceContext, resourceMemoryBuffer, "data/", 0);

	K15_AsyncOperation* asyncRobbie = K15_AsyncLoadResource(sample1GameContext->resourceContext, "meshes/robbie/Robbie_the_Rabbit_rigged.k15mesh", 0);
	
	sample1GameContext->robbie = K15_LoadResource(sample1GameContext->resourceContext, "meshes/robbie/Robbie_the_Rabbit_rigged.k15mesh", 0);
}
/*********************************************************************************/



/***********************************************uint**********************************/
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

	K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->gameMemory;
	K15_ResourceContext* resourceContext = gameContext->resourceContext;
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_QuitGame()
{

}
/*********************************************************************************/