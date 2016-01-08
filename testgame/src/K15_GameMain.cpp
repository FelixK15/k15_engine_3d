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
#include <K15_BlockAllocator.h>
#include <K15_StackAllocator.h>

#include <K15_RenderCameraDesc.h>
#include <K15_RenderFontDesc.h>

#include <K15_RenderContext.h>

#include <K15_Rectangle.h>
#include <K15_Matrix4.h>

#include <K15_System.h>
#include <K15_String.h>

#define K15_GAME_MEMORY_USAGE size_megabyte(20)

struct K15_Sample1GameContext
{
	K15_RenderCommandQueue* gameRenderQueue;
	K15_ResourceContext* resourceContext;
	K15_GUIContext* guiContext;
	K15_RenderFontDesc* gameFont;
	K15_RenderCameraDesc camera;
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

	//How much memory should be used for resource caching?
	const uint32 resourceBufferSize = size_megabyte(5) + size_kilobyte(20); //20 kilobyte for bookkeeping

	//Get memory from the engine (that is the memory we asked for in the K15_InitGame)
	K15_MemoryBuffer gameMemoryBuffer = p_GameContext->gameMemory;

	//allocator for game data
	K15_CustomMemoryAllocator gameMemoryAllocator = K15_CreateStackAllocator(gameMemoryBuffer, "Game Memory Allocator");

	//Let the game context for this project be the first thing we allocate, so we *know* that the game context is always first in memory.
	K15_Sample1GameContext* sample1GameContext = (K15_Sample1GameContext*)K15_AllocateFromMemoryAllocator(&gameMemoryAllocator, sizeof(K15_Sample1GameContext));
	
	//Create the memory buffer for the resource context (this buffer will be used for resource caching).
	K15_MemoryBuffer resourceMemoryBuffer = K15_CreateMemoryBufferFromCustomAllocator(&gameMemoryAllocator, resourceBufferSize);

	//Create the memory buffer for the gui context
	K15_MemoryBuffer guiMemoryBuffer = K15_CreateMemoryBufferFromCustomAllocator(&gameMemoryAllocator, K15_GUI_CONTEXT_MEMORY_SIZE);

	//create render command buffer for the game
	K15_RenderCommandQueue* mainRenderQueue = K15_CreateRenderCommandQueue(p_GameContext->renderContext, "MainRenderQueue");

	//Create the resource context pointing to the 'data' directory of the working directory.
	K15_ResourceContext* resourceContext = K15_CreateResourceContextWithCustomAllocator(p_GameContext->asyncContext, p_GameContext->renderContext, "data/", K15_CreateDefaultMemoryAllocator());//,K15_CreateBlockAllocator(resourceMemoryBuffer, "Resource Allocator"));

	K15_GUIContext* guiContext = K15_CreateGUIContextWithCustomAllocator(K15_CreateStackAllocator(guiMemoryBuffer, "GUI Stack Allocator"), resourceContext, mainRenderQueue);

	K15_ResourceHandle gameFont = K15_LoadResource(resourceContext, K15_FONT_RESOURCE_IDENTIFIER, "gameFont.ttf", 0);

	//set the resource context so we can use it later.
	sample1GameContext->resourceContext = resourceContext;
	sample1GameContext->guiContext = guiContext;
	sample1GameContext->gameRenderQueue = mainRenderQueue;
	sample1GameContext->gameFont = K15_GetResourceRenderFontDesc(resourceContext, gameFont);

	K15_RenderCameraDesc cameraDesc = {};

	cameraDesc.fov = K15_HALF_PI;
	cameraDesc.nearPlane = 1.0f;
	cameraDesc.farPlane = 1000.f;
	cameraDesc.type = K15_RENDER_CAMERA_TYPE_PERSPECTIVE_PROJECTION;
	cameraDesc.flags = K15_RENDER_CAMERA_FLAG_DRAW_TO_RENDER_TARGET | K15_RENDER_CAMERA_FLAG_ACTIVATE_CAMERA;
	cameraDesc.position = K15_CreateVector(0.0f, 0.0f, 0.f);

	p_GameContext->userData = sample1GameContext;

	K15_LoadStockShader(p_GameContext->renderContext, resourceContext);

	//TODO
	//K15_RenderCommandCreateCamera(renderBuffer, &sample1GameContext->camera, &cameraDesc);
	//K15_RenderCommandBindCamera(renderBuffer, &sample1GameContext->camera);

	//K15_AsyncContext* asyncContext = p_GameContext->asyncContext;

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
	//Global 'initialized' variable is used to check if we need to set
	//global contexts like OSContext or Logging. If the game library
	//gets dynamically reloaded, the global contexts get reset.
	if (g_Initialized == K15_FALSE)
	{
		//Set game context and distribute memory from the engine
		K15_InternalSetGameContext(p_GameContext);
		g_Initialized = K15_TRUE;
	}

	K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->userData;
	K15_RenderCommandQueue* gameRenderCommandQueue = gameContext->gameRenderQueue;
	K15_GUIContext* guiContext = gameContext->guiContext;
	K15_RenderFontDesc* gameFont = gameContext->gameFont;

 	char* text = (char*)alloca(128);
 	sprintf(text, "FPS: %.1f\nms: %.3f", p_GameContext->frameCounter.FPS, p_GameContext->frameCounter.avgDeltaTime);
 	K15_RenderCommandDraw2DText(gameRenderCommandQueue, gameFont, text, 0.0f, 0.0f);

	K15_Button(guiContext, "bla", "test_button");
//	K15_ComboBox(guiContext, K15_CreateStringArray(3, "bla1", "bla2", "bla3"), 3, "text_combobox");

	K15_RenderCommandDraw2DGUI(gameRenderCommandQueue, guiContext);
	K15_FlipGUIContextMemory(guiContext);

	K15_DispatchRenderCommandQueue(p_GameContext->renderContext, gameContext->resourceContext->commandQueue);
	K15_DispatchRenderCommandQueue(p_GameContext->renderContext, gameRenderCommandQueue);
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

		K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->userData;
		K15_SetGUIContextMousePosition(gameContext->guiContext, posX, posY);
	}
	else if (p_SystemEvent->event == K15_MOUSE_BUTTON_PRESSED ||
			p_SystemEvent->event == K15_MOUSE_BUTTON_RELEASED)
	{
		bool8 leftMouse = p_SystemEvent->params.mouseButton == K15_LEFT_MOUSE_BUTTON;
		bool8 rightMouse = p_SystemEvent->params.mouseButton == K15_RIGHT_MOUSE_BUTTON;
		bool8 pressed = p_SystemEvent->event == K15_MOUSE_BUTTON_PRESSED;

		K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->userData;
		gameContext->guiContext->leftMouseDown = pressed && leftMouse;
		gameContext->guiContext->rightMouseDown = pressed && rightMouse;
	}
	else if (p_SystemEvent->event == K15_TEXT_INPUT)
	{
		char character = p_SystemEvent->params.utf8Char;
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

		K15_Sample1GameContext* gameContext = (K15_Sample1GameContext*)p_GameContext->userData;
		K15_SetGUIContextWindowSize(gameContext->guiContext, width, height);
		K15_RenderCommandWindowResized(gameContext->gameRenderQueue, width, height);
	}
}
/*********************************************************************************/
K15_EXPORT_SYMBOL void K15_QuitGame(K15_GameContext* p_GameContext)
{
	//This function will get called *once* on shutdown
}
/*********************************************************************************/