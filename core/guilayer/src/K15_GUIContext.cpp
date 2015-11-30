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
intern inline K15_GUIContextStyle K15_InternalCreateDefaultStyle(K15_ResourceContext* p_ResourceContext)
{
	K15_GUIContextStyle defaultStyle = {};

	K15_ResourceHandle styleTextureResource = K15_LoadResource(p_ResourceContext, K15_TEXTURE_RESOURCE_IDENTIFIER, "gui_template.k15texture", 0); 
	K15_ResourceHandle styleFontResource = K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "gui_font.k15font", 0); 

	defaultStyle.styleTexture = K15_GetResourceRenderHandle(p_ResourceContext, styleTextureResource);
	defaultStyle.styleFont = K15_GetResourceFontDesc(p_ResourceContext, styleFontResource);

	//button style
	{
		defaultStyle.guiButtonStyle.posPixelX = 0;
		defaultStyle.guiButtonStyle.posPixelY = 0;
		defaultStyle.guiButtonStyle.pixelWidth = 31;
		defaultStyle.guiButtonStyle.pixelHeight = 23;
		defaultStyle.guiButtonStyle.marginLeft = 4;
		defaultStyle.guiButtonStyle.marginTop = 4;
		defaultStyle.guiButtonStyle.marginBottom = 4;
		defaultStyle.guiButtonStyle.marginRight = 4;
	}
	
	return defaultStyle;
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

	uint32 bookKeeping = size_kilobyte(1);

	if (osContext->window.window)
	{
		windowHeight = osContext->window.window->height;
		windowWidth = osContext->window.window->width;
	}

	uint32 actualGUIMemorySize = K15_GUI_CONTEXT_MEMORY_SIZE - bookKeeping;

	byte* guiMemory = (byte*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, actualGUIMemorySize);

	K15_ASSERT_TEXT(guiMemory, "Could not acquire gui memory (%dkb) from allocator '%s'",
		actualGUIMemorySize / 1024, p_MemoryAllocator.name);

	guiContext->memoryAllocator = p_MemoryAllocator;
	guiContext->guiMemory = guiMemory;
	guiContext->guiMemoryMaxSize = actualGUIMemorySize ;
	guiContext->guiMemoryCurrentSize = 0;
	guiContext->windowHeight = windowHeight;
	guiContext->windowWidth = windowWidth;
	guiContext->mousePosPixelX = 0;
	guiContext->mousePosPixelY = 0;
	guiContext->virtualResolutionHeight = 0;
	guiContext->virtualResolutionWidth = 0;
	
	//create and assign default style
	guiContext->style = K15_InternalCreateDefaultStyle(p_ResourceContext);

	uint32 flags = 0;

	K15_ResourceHandle guiMaterial = K15_LoadResource(p_ResourceContext, K15_MATERIAL_RESOURCE_IDENTIFIER, "gui_material.k15material", flags); 

	guiContext->guiRenderMaterial = K15_GetResourceRenderMaterialDesc(p_ResourceContext, guiMaterial);

	return guiContext;
}
/*********************************************************************************/
void K15_ResetGUIContextMemory(K15_GUIContext* p_GUIContext)
{
	assert(p_GUIContext);

	p_GUIContext->guiMemoryCurrentSize = 0;
}
/*********************************************************************************/
void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight)
{
	assert(p_GUIContext);

	uint32 oldWindowWidth = p_GUIContext->windowWidth;
	uint32 oldWindowHeight = p_GUIContext->windowHeight;

	p_GUIContext->windowHeight = p_WindowHeight;
	p_GUIContext->windowWidth = p_WindowWidth;
}
/*********************************************************************************/
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY)
{
	assert(p_GUIContext);

	p_GUIContext->mousePosPixelX = p_MouseX;
	p_GUIContext->mousePosPixelY = p_MouseY;
}
/*********************************************************************************/
bool8 K15_Button(K15_GUIContext* p_GUIContext, const char* p_Caption)
{
	assert(p_GUIContext);

	uint32 offset = p_GUIContext->guiMemoryCurrentSize;
	uint32 newOffset = offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIButton);

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

	p_GUIContext->guiMemoryCurrentSize = newOffset;

	K15_GUIElementHeader* buttonHeader = (K15_GUIElementHeader*)(p_GUIContext->guiMemory + offset);
	K15_GUIButton* button = (K15_GUIButton*)(p_GUIContext->guiMemory + offset + sizeof(K15_GUIElementHeader));

	button->state = K15_GUI_BUTTON_STATE_NORMAL;
	buttonHeader->type = K15_GUI_TYPE_BUTTON;
	buttonHeader->posPixelX = 0;
	buttonHeader->posPixelY = 400;
	buttonHeader->pixelWidth = 40;
	buttonHeader->pixelHeight = 20;
	buttonHeader->offset = newOffset;

	//buttonHeader->width
	return K15_FALSE;
}
/*********************************************************************************/