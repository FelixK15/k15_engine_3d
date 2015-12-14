#include "K15_GUIContext.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_OSContext.h"
#include "K15_Window.h"

#include "K15_MaterialFormat.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"

#include "K15_RenderFontDesc.h"
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

	K15_ResourceHandle styleTextureResource = K15_LoadResource(p_ResourceContext, K15_TEXTURE_RESOURCE_IDENTIFIER, "textures/gui_template.png", 0); 
	K15_ResourceHandle styleFontResource = K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "fonts/gui_font.ttf", 0); 

	defaultStyle.styleTexture = K15_GetResourceRenderHandle(p_ResourceContext, styleTextureResource);
	defaultStyle.styleFont = K15_GetResourceFontDesc(p_ResourceContext, styleFontResource);

	//button style
	{
		//normal
		/*********************************************************************************/
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].posPixelX = 0;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].posPixelY = 0;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].pixelWidth = 29;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].pixelHeight = 20;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginLeft = 3;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginTop = 3;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginBottom = 3;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginRight = 3;
		//pressed
		/*********************************************************************************/
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].posPixelX = 0;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].posPixelY = 21;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].pixelWidth = 29;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].pixelHeight = 20;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginLeft = 3;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginTop = 3;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginBottom = 3;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginRight = 3;
		//hovered
		/*********************************************************************************/
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].posPixelX = 0;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].posPixelY = 42;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].pixelWidth = 29;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].pixelHeight = 20;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginLeft = 4;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginTop = 4;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginBottom = 5;
		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginRight = 5;
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

	K15_ResourceHandle guiMaterial = K15_LoadResource(p_ResourceContext, K15_MATERIAL_RESOURCE_IDENTIFIER, "materials/gui_material", flags); 

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

	uint32 captionLength = (uint32)strlen(p_Caption);
	uint32 offset = p_GUIContext->guiMemoryCurrentSize;
	uint32 memoryCaptionPos =  offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIButton);
	uint32 newOffset = memoryCaptionPos + captionLength;

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

	p_GUIContext->guiMemoryCurrentSize = newOffset;

	K15_GUIElementHeader* buttonHeader = (K15_GUIElementHeader*)(p_GUIContext->guiMemory + offset);
	K15_GUIButton* button = (K15_GUIButton*)(p_GUIContext->guiMemory + offset + sizeof(K15_GUIElementHeader));

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, p_Caption, captionLength);

	button->state = K15_GUI_BUTTON_STATE_NORMAL;
	button->textLength = captionLength;
	button->text = (const char*)(p_GUIContext->guiMemory + memoryCaptionPos);

	buttonHeader->type = K15_GUI_TYPE_BUTTON;
	buttonHeader->posPixelX = 0;
	buttonHeader->posPixelY = 400;
	buttonHeader->pixelWidth = textWidth;
	buttonHeader->pixelHeight = textHeight;
	buttonHeader->offset = newOffset;

	bool8 mouseInside = K15_Collision2DBoxPoint(buttonHeader->posPixelX, buttonHeader->posPixelY, 
		buttonHeader->posPixelX + buttonHeader->pixelWidth,
		buttonHeader->posPixelY + buttonHeader->pixelHeight,
		p_GUIContext->mousePosPixelX, p_GUIContext->mousePosPixelY);

	if (mouseInside)
	{
		button->state = K15_GUI_BUTTON_STATE_HOVERED;
	}

	//copy text incl 0 terminator
	memcpy(p_GUIContext->guiMemory + memoryCaptionPos, p_Caption, captionLength);

	return K15_FALSE;
}
/*********************************************************************************/