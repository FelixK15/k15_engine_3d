#include <K15_Prerequisites.h>
#include <K15_GameInit.h>

#include <K15_OSContext.h>
#include <K15_Logging.h>
#include <K15_SystemEvents.h>

#include <K15_AsyncOperation.h>
#include <K15_ResourceContext.h>
#include <K15_MeshFormat.h>
#include <K15_MemoryBuffer.h>

#include <K15_RenderContext.h>

#define K15_GAME_MEMORY_USAGE size_megabyte(20)

struct K15_Sample1GameContext
{
	K15_RenderCommandBuffer* gameRenderBuffer;
	K15_ResourceContext* resourceContext;
	K15_Resource* robbie;
};

intern uint8 g_Initialized = K15_FALSE;

/*********************************************************************************/
intern inline void K15_InternalSetGameContext(K15_GameContext* p_GameContext)
{
	//Set global context (OSLayer & Logging)
	uint32 numLogContexts = p_GameContext->logContextCount;

	K15_SetLogContexts(p_GameContext->logContexts, numLogContexts);
	K15_InternalSetOSLayerContext(*p_GameContext->osContext);


	//Get memory from the engine (that is the memory we asked for in the K15_InitGame)
	K15_MemoryBuffer* memory = p_GameContext->gameMemory;

	//How much memory should be used for resource caching?
	uint32 resourceBufferSize = size_megabyte(5) + size_kilobyte(20); //20 kilobyte for bookkeeping

	//Let the game context for this project be the first thing we allocate, so we *know* that the game context is always first in memory.
	K15_Sample1GameContext* sample1GameContext = (K15_Sample1GameContext*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_Sample1GameContext));

	//Create the memory buffer for the resource context (this buffer will be used for resource caching).
	K15_MemoryBuffer* resourceMemoryBuffer = (K15_MemoryBuffer*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_MemoryBuffer));
	K15_InitializeMemoryBufferWithCustomAllocator(resourceMemoryBuffer, K15_CreateMemoryBufferAllocator(memory), resourceBufferSize, 0);

	//Create the resource context pointing to the 'data' directory of the working directory.
	K15_ResourceContext* resourceContext = K15_CreateResourceContextWithCustomAllocator("data/", K15_CreateMemoryBufferAllocator(resourceMemoryBuffer), 0);

	//set the resource context so we can use it later.
	sample1GameContext->resourceContext = resourceContext;

	//create render command buffer for the game
	K15_RenderCommandBuffer* renderBuffer = K15_CreateRenderCommandBuffer(p_GameContext->renderContext);

	//set render command buffer
	sample1GameContext->gameRenderBuffer = renderBuffer;

	K15_AsyncContext* asyncContext = p_GameContext->asyncContext;

	K15_MeshResource* robbieMesh = K15_LoadMeshResource(resourceContext, "meshes/robbie/Robbie_the_Rabbit_rigged.k15mesh", 0);

	K15_Resource** resourcePtr = &sample1GameContext->robbie;
	K15_AsyncLoadResource(asyncContext, resourcePtr, resourceContext, "meshes/robbie/Robbie_the_Rabbit_rigged.k15mesh", 0, K15_ASYNC_OPERATION_FIRE_AND_FORGET_FLAG);
}
/*********************************************************************************/



/***********************************************uint**********************************/
K15_EXPORT_SYMBOL void K15_InitGame(K15_InitGameInputData* p_InputData, K15_InitGameOutputData* p_OutputData)
{
	//This function will get called *once* on startup

	//Tell the engine how much memory we need.
	p_OutputData->requestedMemorySize = K15_GAME_MEMORY_USAGE;
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_TickGame(K15_GameContext* p_GameContext)
{
	//Global 'initialized' variable is used to check if we need to set
	//global contexts like OSContext or Logging. If the game library
	//gets dynamically reloaded, the global contexts get reset.
	if (g_Initialized == K15_FALSE)
	{
		//Set game context and distribute memory from the engine
		K15_InternalSetGameContext(p_GameContext);
		g_Initialized = K15_TRUE;
	}

	//Game context is an internal struct. We know that the head of the game memory, which the engines
	//provides is reserved for the struct.
	K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->gameMemory;
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_QuitGame()
{
	//This function will get called *once* on shutdown
}
/*********************************************************************************/