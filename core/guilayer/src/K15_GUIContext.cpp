#include "K15_GUIContext.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_OSContext.h"
#include "K15_Window.h"
#include "K15_SystemEvents.h"

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
#include "K15_String.h"
#include "K15_Thread.h"

/*********************************************************************************/
enum K15_GUIElementType
{
	K15_GUI_BUTTON = 0,
	K15_GUI_SLIDER,
	K15_GUI_WINDOW
};
/*********************************************************************************/
struct K15_GUIRectangle
{
	int32 pixelPosLeft;
	int32 pixelPosTop;
	int32 pixelPosRight;
	int32 pixelPosBottom;
};
/*********************************************************************************/
struct K15_GUILayout
{
	K15_GUIRectangle* elementRectangles[K15_GUI_MAX_ELEMENTS_PER_LAYOUT];
	K15_GUIRectangle layoutRectangle;
	uint32 numElements;
};
/*********************************************************************************/
struct K15_GUIElement
{
	K15_GUIRectangle rect;
	uint32 identifierHash;
};
/*********************************************************************************/
intern inline K15_GUIContextStyle K15_InternalCreateDefaultStyle(K15_ResourceContext* p_ResourceContext)
{
	K15_GUIContextStyle defaultStyle = {};

	K15_ResourceHandle styleFontResource = 
		K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "fonts/gui_font.ttf", 0); 

	defaultStyle.buttonStyle.borderLowerColor = 0x101010;
	defaultStyle.buttonStyle.borderUpperColor = 0x606060;
	defaultStyle.buttonStyle.lowerBackgroundColor = 0x505050;
	defaultStyle.buttonStyle.upperBackgroundColor = 0x303030;
	defaultStyle.buttonStyle.textColor = 0x000000;
	defaultStyle.buttonStyle.borderPixelThickness = 2;
	defaultStyle.buttonStyle.font = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);
	
	defaultStyle.windowStyle.borderLowerColor = 0x000000;
	defaultStyle.windowStyle.borderUpperColor = 0x050545;
	defaultStyle.windowStyle.lowerBackgroundColor = 0x808080;
	defaultStyle.windowStyle.upperBackgroundColor = 0x101010;
	defaultStyle.windowStyle.titleTextColor = 0xEEEEEE;
	defaultStyle.windowStyle.borderPixelThickness = 2;
	defaultStyle.windowStyle.font = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);
	
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

	uint32 bookKeeping = size_kilobyte(5);

	if (osContext->window.window)
	{
		windowHeight = osContext->window.window->height;
		windowWidth = osContext->window.window->width;
	}

	uint32 actualGUIMemorySize = K15_GUI_CONTEXT_MEMORY_SIZE - bookKeeping;
	uint32 guiMemoryBufferSize = actualGUIMemorySize / 2;

	byte* guiMemory = (byte*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, actualGUIMemorySize);
	memset(guiMemory, 0, actualGUIMemorySize);

	K15_ASSERT_TEXT(guiMemory, "Could not acquire gui memory (%dkb) from allocator '%s'",
		actualGUIMemorySize / 1024, p_MemoryAllocator.name);

	guiContext->memoryAllocator = p_MemoryAllocator;

	return guiContext;
}
/*********************************************************************************/
intern K15_GUILayout* K15_InternalGUIGetTopLayout(K15_GUIContext* p_GUIContext)
{
	K15_GUILayout* topLayout = 0;
	uint32 layoutIndex = p_GUIContext->numLayouts;

	if (layoutIndex > 0)
	{
		layoutIndex -= 1;
		topLayout = &p_GUIContext->layoutStack[layoutIndex];
	}
	
	return topLayout;
}
/*********************************************************************************/
intern void K15_InternalGUIAddGUIRectToTopLayout(K15_GUIContext* p_GUIContext, K15_GUIRectangle* p_GUIRect)
{
	K15_GUILayout* topLayout = K15_InternalGUIGetTopLayout(p_GUIContext);

	if (topLayout)
	{
		K15_ASSERT(topLayout->numElements != K15_GUI_MAX_ELEMENTS_PER_LAYOUT);

		uint32 elementIndex = topLayout->numElements++;
		topLayout->elementRectangles[elementIndex] = p_GUIRect;
	}
}
/*********************************************************************************/
intern byte* K15_InternalGUIGetRetainedDataBuffer(K15_GUIContext* p_GUIContext)
{
	return p_GUIContext->memoryBuffer + 
		p_GUIContext->retainedDataOffsetInBytes + 
		p_GUIContext->retainedDataSizeInBytes;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGUIGetGUIElementFromRetainedDataBufer(K15_GUIContext* p_GUIContext, 
	uint32 p_IdentifierHash)
{
	byte* retainendDataBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
	K15_GUIElement* retainedElement = 0;

	uint32 offset = 0;

	while (offset < p_GUIContext->retainedDataSizeInBytes)
	{
		K15_GUIElement* currentElement = (K15_GUIElement*)(retainendDataBuffer + offset);

		if (currentElement->identifierHash == p_IdentifierHash)
		{
			retainedElement = currentElement;
			break;
		}

		offset += sizeof(K15_GUIElement);
	}

	return retainedElement;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGetRetainedGUIElement(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash)
{
	
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalAddGUIElement(K15_GUIContext* p_GUIContext, K15_GUIElementType p_GUIElementType,
	const char* p_Identifier)
{
	//so what do we need to do here?
	// - Add GUI Element to GUI Context (so we can iterate over it)
	// - Retrieve 'old' GUI Element (so we can retrieve the information (clicked or not etc.)
	// - Add GUI Element Rect to layout
	uint32 identifierHash = K15_GenerateStringHash(p_Identifier);

	K15_GUIElement* element = (K15_GUIElement*)K15_InternalGetRetainedGUIElement(p_GUIContext, identifierHash);

	if (!element)
	{
		byte* retainedDataBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
		K15_ASSERT(K15_PTR_DIFF(p_GUIContext->memoryBuffer, retainedDataBuffer) <= K15_GUI_CONTEXT_MEMORY_SIZE);
		element = (K15_GUIElement*)(retainedDataBuffer + p_GUIContext->retainedDataSizeInBytes);
		p_GUIContext->retainedDataSizeInBytes += sizeof(K15_GUIElement);
	}

	K15_InternalGUIAddGUIRectToTopLayout(p_GUIContext, &element->rect);
}
/*********************************************************************************/
void K15_GUIBeginDockingArea(K15_GUIContext* p_GUIContext, int32 p_PosX, int32 p_PosY,
	uint32 p_Width, uint32 p_Height, uint32 p_AllowedDockingAreasMask)
{
//	K15_InternalGUIPushArea(p_GUIContext, K15_GUI_DOCKING_AREA, p_PosX, p_PosY, p_Width, p_Height);
}
/*********************************************************************************/
bool8 K15_GUIBeginWindowEX(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier,
	K15_GUIWindowStyle* p_GUIWindowStyle)
{
	K15_GUIElement* windowElement =	K15_InternalAddGUIElement(p_GUIContext, K15_GUI_WINDOW, 
		*p_PosX, *p_PosY, *p_Width, *p_Height, p_Identifier);

	K15_GUIRectangle* windowElementRect = &windowElement->rect;
	K15_InternalGUIPushLayout(p_GUIContext, windowElementRect);
}
/*********************************************************************************/
bool8 K15_GUIBeginWindow(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier)
{
	K15_GUIContextStyle* style = &p_GUIContext->style;
	return K15_GUIBeginWindowEX(p_GUIContext, p_PosX, p_PosY, p_Width, p_Height, p_Identifier, &style->windowStyle);
}
/*********************************************************************************/
bool8 K15_GUIButton(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier)
{

}
/*********************************************************************************/
void K15_GUIPushVerticalLayout(K15_GUIContext* p_GUIContext)
{

}
/*********************************************************************************/
void K15_GUIPushHorizontalLayout(K15_GUIContext* p_GUIContext)
{

}
/*********************************************************************************/