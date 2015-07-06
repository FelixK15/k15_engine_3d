#include <K15_Prerequisites.h>
#include <K15_GameInit.h>

#include <K15_OSContext.h>
#include <K15_Logging.h>
#include <K15_SystemEvents.h>

#include <K15_RenderCommands.h>
#include <K15_AsyncOperation.h>
#include <K15_ResourceContext.h>
#include <K15_Mesh.h>
#include <K15_GUIContext.h>
#include <K15_MemoryBuffer.h>

#include <K15_RenderCameraDesc.h>

#include <K15_RenderContext.h>

#include <K15_Matrix4.h>

#define K15_GAME_MEMORY_USAGE size_megabyte(20)

struct K15_Sample1GameContext
{
	K15_RenderCommandBuffer* gameRenderBuffer;
	K15_ResourceContext* resourceContext;
	K15_GUIContext* guiContext;

	K15_Mesh* robbie;

	K15_RenderCameraHandle camera;
};

intern uint8 g_Initialized = K15_FALSE;

/*********************************************************************************/
intern inline void K15_InternalSetGameContext(K15_GameContext* p_GameContext)
{
#ifdef K15_LOAD_GAME_LIB_DYNAMIC
	//Set global context (OSLayer & Logging)
	uint32 numLogContexts = p_GameContext->logContextCount;

	K15_SetLogContexts(p_GameContext->logContexts, numLogContexts);
	K15_InternalSetOSLayerContext(*p_GameContext->osContext);
#endif //K15_LOAD_GAME_LIB_DYNAMIC

	//Get memory from the engine (that is the memory we asked for in the K15_InitGame)
	K15_MemoryBuffer* memory = p_GameContext->gameMemory;

	//How much memory should be used for resource caching?
	uint32 resourceBufferSize = size_megabyte(5) + size_kilobyte(20); //20 kilobyte for bookkeeping

	//Let the game context for this project be the first thing we allocate, so we *know* that the game context is always first in memory.
	K15_Sample1GameContext* sample1GameContext = (K15_Sample1GameContext*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_Sample1GameContext));

	//Create the memory buffer for the resource context (this buffer will be used for resource caching).
	K15_MemoryBuffer* resourceMemoryBuffer = (K15_MemoryBuffer*)K15_GetMemoryFromMemoryBuffer(memory, sizeof(K15_MemoryBuffer));
	K15_MemoryBuffer* guiMemoryBuffer = (K15_MemoryBuffer*)K15_GetMemoryFromMemoryBuffer(memory, K15_GUI_CONTEXT_MEMORY_SIZE);

	K15_InitializeMemoryBufferWithCustomAllocator(resourceMemoryBuffer, K15_CreateMemoryBufferAllocator(memory), resourceBufferSize, 0);
	K15_InitializeMemoryBufferWithCustomAllocator(guiMemoryBuffer, K15_CreateMemoryBufferAllocator(memory), resourceBufferSize, 0);

	//Create the resource context pointing to the 'data' directory of the working directory.
	K15_ResourceContext* resourceContext = K15_CreateResourceContextWithCustomAllocator("data/", K15_CreateMemoryBufferAllocator(resourceMemoryBuffer), 0);

	//set the resource context so we can use it later.
	sample1GameContext->resourceContext = resourceContext;

	//create render command buffer for the game
	K15_RenderCommandBuffer* renderBuffer = K15_CreateRenderCommandBuffer(p_GameContext->renderContext);

	//set render command buffer
	sample1GameContext->gameRenderBuffer = renderBuffer;

	K15_RenderCameraDesc cameraDesc = {};

	cameraDesc.fov = K15_HALF_PI;
	cameraDesc.nearPlane = 1.0f;
	cameraDesc.farPlane = 1000.f;
	cameraDesc.type = K15_RENDER_CAMERA_TYPE_PERSPECTIVE_PROJECTION;
	cameraDesc.flags = K15_RENDER_CAMERA_FLAG_DRAW_TO_RENDER_TARGET | K15_RENDER_CAMERA_FLAG_ACTIVATE_CAMERA;
	cameraDesc.position = K15_CreateVector(0.0f, 0.0f, 0.f);

	K15_RenderCommandCreateCamera(renderBuffer, &sample1GameContext->camera, &cameraDesc);
	K15_RenderCommandBindCamera(renderBuffer, &sample1GameContext->camera);

	K15_AsyncContext* asyncContext = p_GameContext->asyncContext;

	sample1GameContext->guiContext = K15_CreateGUIContextWithCustomAllocator(K15_CreateMemoryBufferAllocator(guiMemoryBuffer), resourceContext, p_GameContext->renderContext);

	//K15_DispatchRenderCommandBuffer(sample1GameContext->guiContext->guiRenderCommandBuffer);

	//Load mesh (creates render commands internally)
	//K15_Mesh* mesh = K15_LoadMesh(resourceContext, renderBuffer, "meshes/cube.k15mesh");
	//K15_Mesh* mesh = K15_LoadMesh(resourceContext, renderBuffer, "meshes/robbie/Robbie_the_Rabbit_rigged.k15mesh");

	//sample1GameContext->robbie = mesh;
}
/*********************************************************************************/



/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_InitGame(K15_InitGameInputData* p_InputData, K15_InitGameOutputData* p_OutputData)
{
	//This function will get called *once* on startup

	//Tell the engine how much memory we need.
	p_OutputData->requestedMemorySize = K15_GAME_MEMORY_USAGE;
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_TickGame(K15_GameContext* p_GameContext)
{
	static bool forward = true;
	static float z = -5.0f;
	//Global 'initialized' variable is used to check if we need to set
	//global contexts like OSContext or Logging. If the game library
	//gets dynamically reloaded, the global contexts get reset.
	if (g_Initialized == K15_FALSE)
	{
		//Set game context and distribute memory from the engine
		K15_InternalSetGameContext(p_GameContext);
		g_Initialized = K15_TRUE;
	}
	K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->gameMemory->buffer;
	K15_RenderCommandBuffer* gameRenderCommandBuffer = gameContext->gameRenderBuffer;

	K15_DispatchRenderCommandBuffer(gameContext->guiContext->guiRenderCommandBuffer);
	K15_DispatchRenderCommandBuffer(gameRenderCommandBuffer);


// 	if (p_GameContext->frameCounter >= 10)
// 	{
// 		//Game context is an internal struct. We know that the head of the game memory, which the engines
// 		//provides is reserved for the struct.
// 		K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->gameMemory->buffer;
// 
// 		K15_Mesh* robbieMesh = gameContext->robbie;
// 
// 		K15_Matrix4 worldMatrix = K15_GetIdentityMatrix4();
// 
// 		worldMatrix._1_4 = 0.f;
// 		worldMatrix._2_4 = 0.f;
// 		worldMatrix._3_4 = z;
// 
// 		if(forward)
// 		{
// 			z -= 0.5f;
// 
// 			if (z <= -8.f)
// 			{
// 				forward = false;
// 			}
// 		}
// 		else
// 		{
// 			z += 0.5f;
// 
// 			if (z >= -4.f)
// 			{
// 				forward = true;
// 			}
// 		}
// 
// 		K15_RenderCommandDrawMesh(gameRenderCommandBuffer, &robbieMesh->renderMeshHandle, &worldMatrix);
// 
// 	}
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_OnInputEvent(K15_GameContext* p_GameContext, K15_SystemEvent* p_SystemEvent)
{
	if (!g_Initialized)
	{
		return;
	}

	if (p_SystemEvent->event == K15_MOUSE_MOVED)
	{
		uint32 posX = p_SystemEvent->params.position.x;
		uint32 posY = p_SystemEvent->params.position.y;

		K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->gameMemory->buffer;
		K15_SetGUIContextMousePosition(gameContext->guiContext, posX, posY);
	}
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_OnSystemEvent(K15_GameContext* p_GameContext, K15_SystemEvent* p_SystemEvent)
{
	if (!g_Initialized)
	{
		return;
	}

}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_OnWindowEvent(K15_GameContext* p_GameContext, K15_SystemEvent* p_SystemEvent)
{
	if (!g_Initialized)
	{
		return;
	}

	if (p_SystemEvent->event == K15_WINDOW_RESIZED)
	{
		uint32 width = p_SystemEvent->params.size.width;
		uint32 height = p_SystemEvent->params.size.height;

		K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->gameMemory->buffer;
		K15_SetGUIContextWindowSize(gameContext->guiContext, width, height);
	}
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_QuitGame(K15_GameContext* p_GameContext)
{
	//This function will get called *once* on shutdown
}
/*********************************************************************************/