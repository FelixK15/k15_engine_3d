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
K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext)
{
	return K15_CreateGUIContextWithCustomAllocator(K15_CreateDefaultMemoryAllocator(), p_ResourceContext, p_RenderContext);
}
/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext)
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
	guiContext->guiRenderCommandBuffer = K15_CreateRenderCommandQueue(p_RenderContext);
	guiContext->guiMemoryCurrentSize = 0;
	guiContext->guiTemplateTexture = K15_INVALID_GPU_RESOURCE_HANDLE;
	guiContext->windowHeight = windowHeight;
	guiContext->windowWidth = windowWidth;
	guiContext->mousePosX = 0.f;
	guiContext->mousePosY = 0.f;

	K15_Resource* guiTemplateTexture = K15_LoadResource(p_ResourceContext, "gui_template.k15texture", 0);
	K15_Resource* guiFont = K15_LoadResource(p_ResourceContext, "gui_font.k15font", 0);
	K15_Resource* guiMaterial = K15_LoadResource(p_ResourceContext, "gui_material.k15material", 0);

	if (!guiTemplateTexture)
	{
		K15_LOG_WARNING_MESSAGE("Could not load gui_template texture.");
	}

	K15_FontFormat fontFormat = {};
	K15_TextureFormat guiTextureFormat = {};
	K15_MaterialFormat guiMaterialFormat = {};

	if (guiFont
		&& K15_LoadFontFormatFromMemory(&fontFormat, guiFont->data, guiFont->dataSizeInBytes) == K15_SUCCESS)
	{

	}

	if (guiMaterial
		&& K15_LoadMaterialFormatFromMemory(&guiMaterialFormat, guiMaterial->data, guiMaterial->dataSizeInBytes) == K15_SUCCESS)
	{
		K15_RenderMaterialDesc guiMaterialDesc = {};
		
	}

	if (guiTemplateTexture &&
		K15_LoadTextureFormatFromMemory(&guiTextureFormat, guiTemplateTexture->data, guiTemplateTexture->dataSizeInBytes) == K15_SUCCESS)
	{
		K15_RenderCommandCreateTextureFromTextureFormat(guiContext->guiRenderCommandBuffer, &guiContext->guiTemplateTexture, &guiTextureFormat);
		
	}

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

	uint32 windowWidth = p_GUIContext->windowWidth;
	uint32 windowHeight = p_GUIContext->windowHeight;

	p_GUIContext->mousePosX = K15_ClampReal(p_MouseX / windowWidth, 1.0f, 0.0f);
	p_GUIContext->mousePosY = K15_ClampReal(p_MouseY / windowHeight, 1.0f, 0.0f);
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