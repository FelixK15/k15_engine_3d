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
intern K15_GUILayout* K15_InternalGUIGetTopLayout(K15_GUIContext* p_GUIContext)
{
	K15_GUILayout* topLayout = 0;
	uint32 layoutIndex = p_GUIContext->layoutIndex;

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
struct K15_GUIGetRetainedElementInfo
{
	uint32 identifierHash;
	K15_GUIElement* retainedElement;
};
/*********************************************************************************/
intern void K15_InternalGUIGetRetainedElementIterator(K15_GUIContext* p_GUIContext, K15_GUIElement* p_GUIElement,
	void *p_UserData)
{
	K15_GUIGetRetainedElementInfo* info = (K15_GUIGetRetainedElementInfo*)p_UserData;

	if (p_GUIElement->identifierHash == info->identifierHash)
	{
		info->retainedElement = p_GUIElement;
	}
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGUIGetRetainedElement(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash)
{
	K15_GUIGetRetainedElementInfo retainedElementInfo = {};
	retainedElementInfo.identifierHash = p_IdentifierHash;
	K15_GUIIterateElements(p_GUIContext, K15_InternalGUIGetRetainedElementIterator, &retainedElementInfo);

	return retainedElementInfo.retainedElement;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalAddGUIElement(K15_GUIContext* p_GUIContext, K15_GUIElementType p_GUIElementType,
	int32* p_PosLeft, int32* p_PosTop, uint32* p_Width, uint32* p_Height, const char* p_Identifier)
{
	//so what do we need to do here?
	// - Add GUI Element to GUI Context (so we can iterate over it)
	// - Retrieve 'old' GUI Element (so we can retrieve the information (clicked or not etc.)
	// - Add GUI Element Rect to layout
	uint32 identifierHash = K15_GenerateStringHash(p_Identifier);

	K15_GUIElement* element = (K15_GUIElement*)K15_InternalGUIGetRetainedElement(p_GUIContext, identifierHash);

	if (!element)
	{
		byte* retainedDataBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
		K15_ASSERT(K15_PTR_DIFF(p_GUIContext->memoryBuffer, retainedDataBuffer) <= K15_GUI_CONTEXT_MEMORY_SIZE);
		element = (K15_GUIElement*)(retainedDataBuffer + p_GUIContext->retainedDataSizeInBytes);
		element->offsetInBytes = p_GUIContext->retainedDataSizeInBytes;
		element->sizeInBytes = sizeof(K15_GUIElement);

		p_GUIContext->retainedDataSizeInBytes += sizeof(K15_GUIElement);
	}

	K15_InternalGUIAddGUIRectToTopLayout(p_GUIContext, &element->rect);

	return element;
}
/*********************************************************************************/
intern byte* K15_InternalAddGUIElementMemory(K15_GUIContext* p_GUIContext, K15_GUIElement* p_GUIElement,
	uint32 p_SizeMemoryInBytes)
{
	byte* retainedBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
	uint32 offsetInBytes = p_GUIElement->offsetInBytes + p_GUIElement->sizeInBytes;

	K15_ASSERT(offsetInBytes + p_SizeMemoryInBytes <= p_GUIContext->memoryMaxSizeInBytes);

	byte* elementMemory = retainedBuffer + offsetInBytes;

	p_GUIContext->retainedDataSizeInBytes += p_SizeMemoryInBytes;

	return elementMemory;
}
/*********************************************************************************/
intern void K15_InternalGUIPushLayout(K15_GUIContext* p_GUIContext, K15_GUIRectangle* p_LayoutArea, 
	K15_GUILayoutType p_LayoutType)
{
	++p_GUIContext->numLayouts;
	uint32 layoutIndex = p_GUIContext->layoutIndex++;
	K15_ASSERT(layoutIndex != K15_GUI_MAX_LAYOUTS);

	K15_GUILayout* layout = p_GUIContext->layoutStack + layoutIndex;
	layout->layoutRectangle = *p_LayoutArea;
	layout->numElements = 0;
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
void K15_GUIBeginDockingArea(K15_GUIContext* p_GUIContext, int32 p_PosX, int32 p_PosY,
	uint32 p_Width, uint32 p_Height, uint32 p_AllowedDockingAreasMask)
{

}
/*********************************************************************************/
bool8 K15_GUIBeginWindow(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier)
{
	K15_GUIContextStyle* style = &p_GUIContext->style;
	return K15_GUIBeginWindowEX(p_GUIContext, p_PosX, p_PosY, p_Width, p_Height, p_Identifier, &style->windowStyle);
}
/*********************************************************************************/
bool8 K15_GUIBeginWindowEX(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier,
	K15_GUIWindowStyle* p_GUIWindowStyle)
{
	K15_GUIElement* windowElement = K15_InternalAddGUIElement(p_GUIContext, K15_GUI_WINDOW,
		p_PosX, p_PosY, p_Width, p_Height, p_Identifier);

	uint32 sizeElementMemoryInBytes = sizeof(K15_GUIWindowStyle);
	byte* windowElementMemory = K15_InternalAddGUIElementMemory(p_GUIContext, windowElement, sizeElementMemoryInBytes);
	memcpy(windowElementMemory, p_GUIWindowStyle, sizeof(K15_GUIWindowStyle));

	//Don't add the window to a layout (except docking layouts...But that will have to wait)
	K15_GUIRectangle* windowElementRect = &windowElement->rect;
	windowElementRect->pixelPosTop += p_GUIWindowStyle->titlePixelPadding;
	windowElementRect->pixelPosBottom += p_GUIWindowStyle->titlePixelPadding;

	K15_InternalGUIPushLayout(p_GUIContext, windowElementRect, K15_GUI_VERTICAL_LAYOUT);

	return K15_TRUE;
}
/*********************************************************************************/
bool8 K15_GUIButton(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier)
{
	K15_GUIButtonStyle* defaultButtonStyle = &p_GUIContext->style.buttonStyle;
	return K15_GUIButtonEX(p_GUIContext, p_ButtonText, p_Identifier, defaultButtonStyle);
}
/*********************************************************************************/
bool8 K15_GUIButtonEX(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier,
	K15_GUIButtonStyle* p_GUIButtonStyle)
{
	return K15_FALSE;
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
void K15_GUIPopLayout(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext->layoutIndex);
	p_GUIContext->layoutIndex--;
}
/*********************************************************************************/
void K15_GUIEndWindow(K15_GUIContext* p_GUIContext)
{
	K15_GUIPopLayout(p_GUIContext);
}
/*********************************************************************************/
void K15_GUIEndDockingArea(K15_GUIContext* p_GUIContext)
{

}
/*********************************************************************************/
intern void K15_InternalGUIAlignElements(K15_GUILayout* p_Layouts, uint32 p_NumLayouts)
{
	for (uint32 layoutIndex = 0;
	layoutIndex < p_NumLayouts;
		++layoutIndex)
	{
		K15_GUILayout* layout = p_Layouts + layoutIndex;
		K15_GUILayoutType type = layout->type;
		uint32 numElements = layout->numElements;
		uint32 layoutWidth = layout->layoutRectangle.pixelPosRight - layout->layoutRectangle.pixelPosLeft;
		uint32 layoutHeight = layout->layoutRectangle.pixelPosBottom - layout->layoutRectangle.pixelPosTop;
		uint32 layoutPosX = layout->layoutRectangle.pixelPosLeft;
		uint32 layoutPosY = layout->layoutRectangle.pixelPosTop;
		uint32 offsetX = 0;
		uint32 offsetY = 0;

		for (uint32 elementIndex = 0;
		elementIndex < numElements;
			++elementIndex)
		{
			K15_GUIRectangle* rect = layout->elementRectangles[elementIndex];
			rect->pixelPosLeft = layoutPosX + offsetX;
			rect->pixelPosTop = layoutPosY + offsetY;

			switch (type)
			{
			case K15_GUI_VERTICAL_LAYOUT:
				rect->pixelPosRight = rect->pixelPosLeft + layoutWidth;
				rect->pixelPosBottom = rect->pixelPosTop + 20;
				break;

			case K15_GUI_HORIZONTAL_LAYOUT:
				rect->pixelPosRight = rect->pixelPosLeft + (layoutWidth / numElements);
				rect->pixelPosBottom = rect->pixelPosTop + 20;
				break;
			}
		}
	}
}
/*********************************************************************************/
intern void K15_InternalGUIHandleInput(K15_GUIContext* p_GUIContext)
{
	K15_GUIIterateElements(p_GUIContext, )
}
/*********************************************************************************/
void K15_GUIFinishGUIFrame(K15_GUIContext* p_GUIContext)
{
	K15_InternalGUIAlignElements(p_GUIContext->layoutStack, p_GUIContext->numLayouts);
	K15_InternalGUIHandleInput()
	
}
/*********************************************************************************/
void K15_GUIIterateElements(K15_GUIContext* p_GUIContext, K15_GUIIteratorFnc p_Function, void* p_UserData)
{
	byte* retainedMemory = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
	uint32 offsetInBytes = 0;

	while (offsetInBytes < p_GUIContext->retainedDataSizeInBytes)
	{
		K15_GUIElement* element = (K15_GUIElement*)(retainedMemory + offsetInBytes);
		p_Function(p_GUIContext, element, p_UserData);

		offsetInBytes = element->offsetInBytes;
	}
}
/*********************************************************************************/