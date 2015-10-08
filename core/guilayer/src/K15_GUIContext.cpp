#include "K15_GUIContext.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_OSContext.h"
#include "K15_Window.h"

#include "K15_MaterialFormat.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"

#include "K15_RenderTextureDesc.h"
#include "K15_RenderCommands.h"
#include "K15_RenderContext.h"
#include "K15_ResourceContext.h"
#include "K15_Logging.h"

#include "K15_Math.h"


/*********************************************************************************/
intern inline K15_GUIButton* K15_InternalCreateGUIButton(K15_GUIContext* p_GUIContext)
{
	
}
/*********************************************************************************/



/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	return K15_CreateGUIContextWithCustomAllocator(K15_CreateDefaultMemoryAllocator(), p_ResourceContext, p_RenderCommandQueue);
}
/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_GUIContext* guiContext = (K15_GUIContext*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, sizeof(K15_GUIContext));

	uint32 windowHeight = 0;
	uint32 windowWidth = 0;

	if (osContext->window.window)
	{
		windowHeight = osContext->window.window->height;
		windowWidth = osContext->window.window->width;
	}

	guiContext->memoryAllocator = p_MemoryAllocator;
	guiContext->guiMemory = (byte*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, K15_GUI_CONTEXT_MEMORY_SIZE);
	guiContext->guiMemoryMaxSize = K15_GUI_CONTEXT_MEMORY_SIZE;
	guiContext->guiMemoryCurrentSize = 0;
	guiContext->windowHeight = windowHeight;
	guiContext->windowWidth = windowWidth;
	guiContext->mousePosX = 0.f;
	guiContext->mousePosY = 0.f;

	uint32 flags = 0;

	guiContext->guiRenderTexture = K15_LoadResource(p_ResourceContext, K15_TEXTURE_RESOURCE_IDENTIFIER, "gui_template.k15texture", flags);;
	guiContext->guiRenderFont = K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "gui_font.k15font", flags);;
	guiContext->guiRenderMaterial = K15_LoadResource(p_ResourceContext, K15_MATERIAL_RESOURCE_IDENTIFIER, "gui_material.k15material", flags);;

	return guiContext;
}
/*********************************************************************************/
void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight)
{
	assert(p_GUIContext);

	uint32 oldWindowWidth = p_GUIContext->windowWidth;
	uint32 oldWindowHeight = p_GUIContext->windowHeight;

	if (oldWindowHeight != 0 &&
		oldWindowWidth != 0)
	{
		uint32 mousePosAbsoluteX = oldWindowWidth * p_GUIContext->mousePosX;
		uint32 mousePosAbsoluteY = oldWindowHeight * p_GUIContext->mousePosY;

		p_GUIContext->mousePosX = K15_ClampReal(mousePosAbsoluteX / p_WindowHeight, 1.0f, 0.0f);
		p_GUIContext->mousePosY = K15_ClampReal(mousePosAbsoluteY / p_WindowHeight, 1.0f, 0.0f);
	}

	p_GUIContext->windowHeight = p_WindowHeight;
	p_GUIContext->windowWidth = p_WindowWidth;
}
/*********************************************************************************/
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY)
{
	assert(p_GUIContext);

	real32 windowWidth = (real32)p_GUIContext->windowWidth;
	real32 windowHeight = (real32)p_GUIContext->windowHeight;

	real32 x = (real32)p_MouseX;
	real32 y = (real32)p_MouseY;

	p_GUIContext->mousePosX = K15_ClampReal(x / windowWidth, 1.0f, 0.0f);
	p_GUIContext->mousePosY = K15_ClampReal(y / windowHeight, 1.0f, 0.0f);
}
/*********************************************************************************/
bool8 K15_Button(K15_GUIContext* p_GUIContext, float p_PositionX, float p_PositionY, const char* p_Caption)
{
	assert(p_GUIContext);

	uint32 offset = p_GUIContext->guiMemoryCurrentSize;
	uint32 newOffset = offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIButton);

	K15_ASSERT_TEXT(newOffset >= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

	p_GUIContext->guiMemoryCurrentSize = newOffset;

	K15_GUIElementHeader* buttonHeader = (K15_GUIElementHeader*)p_GUIContext->guiMemory + offset;
	K15_GUIButton* button = (K15_GUIButton*)p_GUIContext->guiMemory + offset + sizeof(K15_GUIElementHeader);

	buttonHeader->type = K15_GUI_TYPE_BUTTON;

	button->state = K15_GUI_BUTTON_STATE_NORMAL;

	return K15_FALSE;
}
/*********************************************************************************/