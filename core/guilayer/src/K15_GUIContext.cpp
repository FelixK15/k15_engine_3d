#include "K15_GUIContext.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_OSContext.h"
#include "K15_Window.h"

#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"

#include "K15_RenderTextureDesc.h"
#include "K15_RenderCommands.h"
#include "K15_RenderContext.h"
#include "K15_ResourceContext.h"
#include "K15_Logging.h"

#include "K15_Math.h"

/*********************************************************************************/
intern inline K15_GUIElementHeader* K15_InternalCreateGUIElementHeader(K15_GUIContext* p_GUIContext)
{
	uint32 offset = p_GUIContext->guiMemoryCurrentSize;
	uint32 newOffset = offset + sizeof(K15_GUIElementHeader);

	K15_ASSERT_TEXT(newOffset >= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

	p_GUIContext->guiMemoryCurrentSize = newOffset;

	return (K15_GUIElementHeader*)(p_GUIContext->guiMemory + newOffset);
}
/*********************************************************************************/



/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext)
{
	return K15_CreateGUIContextWithCustomAllocator(K15_CreateDefaultMemoryAllocator(), p_ResourceContext, p_RenderContext);
}
/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext)
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_GUIContext* guiContext = (K15_GUIContext*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_GUIContext));

	uint32 windowHeight = 0;
	uint32 windowWidth = 0;

	if (osContext->window.window)
	{
		windowHeight = osContext->window.window->height;
		windowWidth = osContext->window.window->width;
	}

	guiContext->memoryAllocator = p_MemoryAllocator;
	guiContext->guiMemory = (byte*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, K15_GUI_CONTEXT_MEMORY_SIZE);
	guiContext->guiMemoryMaxSize = K15_GUI_CONTEXT_MEMORY_SIZE;
	guiContext->guiRenderCommandBuffer = K15_CreateRenderCommandBuffer(p_RenderContext);
	guiContext->guiMemoryCurrentSize = 0;
	guiContext->guiTemplateTexture = K15_INVALID_GPU_RESOURCE_HANDLE;
	guiContext->windowHeight = windowHeight;
	guiContext->windowWidth = windowWidth;
	guiContext->mousePosX = 0.f;
	guiContext->mousePosY = 0.f;

	K15_Resource* guiTemplateTexture = K15_LoadResource(p_ResourceContext, "gui_template.k15texture", 0);
	K15_Resource* guiFont = K15_LoadResource(p_ResourceContext, "gui_font.k15font", 0);

	if (!guiTemplateTexture)
	{
		K15_LOG_WARNING_MESSAGE("Could not load gui_template texture.");
	}

	K15_FontFormat fontFormat = {};
	
	if (K15_LoadFontFormatFromMemory(&fontFormat, guiFont->data, guiFont->dataSizeInBytes) == K15_SUCCESS)
	{

	}

	K15_TextureFormat guiTextureFormat = {};

	if (K15_LoadTextureFormatFromMemory(&guiTextureFormat, guiTemplateTexture->data, guiTemplateTexture->dataSizeInBytes) == K15_SUCCESS)
	{
		K15_RenderTextureDesc guiTextureDesc = {};
		guiTextureDesc.createMipChain = K15_FALSE;
		guiTextureDesc.dimension.depth = 0;
		//guiTextureDesc.dimension.height = guiTextureFormat.height;
		//guiTextureDesc.dimension.width = guiTextureFormat.width;
		guiTextureDesc.dimension.height = fontFormat.texture.height;
		guiTextureDesc.dimension.width = fontFormat.texture.width;
		//guiTextureDesc.mipmaps.count = guiTextureFormat.mipMapCount;
		guiTextureDesc.mipmaps.count = 1;
		guiTextureDesc.mipmaps.data = (byte**)malloc(K15_PTR_SIZE * guiTextureFormat.mipMapCount);
		guiTextureDesc.mipmaps.dataSize = (uint32*)malloc(sizeof(uint32) * guiTextureFormat.mipMapCount);

		for (uint32 imageIndex = 0;
			imageIndex < guiTextureFormat.mipMapCount;
			++imageIndex)
		{
			//guiTextureDesc.mipmaps.dataSize[imageIndex] = K15_GetTextureMipMapSize(&guiTextureFormat, imageIndex);
			guiTextureDesc.mipmaps.dataSize[imageIndex] = K15_GetFontTextureSize(&fontFormat);
			guiTextureDesc.mipmaps.data[imageIndex] = K15_GetFontTexture(&fontFormat);
			//guiTextureDesc.mipmaps.data[imageIndex] = K15_GetTextureMipMap(&guiTextureFormat, imageIndex);
		}

		guiTextureDesc.format = K15_RENDER_FORMAT_R8_UINT;
		//guiTextureDesc.format = K15_RENDER_FORMAT_R8G8B8A8_UINT;
		guiTextureDesc.type = K15_RENDER_TEXTURE_TYPE_2D;
		guiTextureDesc.flags = K15_RENDER_DESC_AUTO_CLEANUP_FLAG;


		K15_RenderCommandCreateTexture(guiContext->guiRenderCommandBuffer, &guiTextureDesc, &guiContext->guiTemplateTexture);
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

	K15_GUIElementHeader* guiButtonHeader = K15_InternalCreateGUIElementHeader(p_GUIContext);

	guiButtonHeader->type = K15_GUI_TYPE_BUTTON;
	guiButtonHeader->positionX = p_PositionX;
	guiButtonHeader->positionY = p_PositionY;

	return K15_FALSE;
}
/*********************************************************************************/