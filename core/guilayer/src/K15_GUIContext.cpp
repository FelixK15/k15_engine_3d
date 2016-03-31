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

	//Button Style
	defaultStyle.buttonStyle.borderLowerColor = 0xFF101010;
	defaultStyle.buttonStyle.borderUpperColor = 0xFF606060;
	defaultStyle.buttonStyle.lowerBackgroundColor = 0xFF505050;
	defaultStyle.buttonStyle.upperBackgroundColor = 0xFF303030;
	defaultStyle.buttonStyle.textColor = 0xFF000000;
	defaultStyle.buttonStyle.borderPixelThickness = 2;
	defaultStyle.buttonStyle.textPixelPadding = 2;
	defaultStyle.buttonStyle.font = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);
	
	//Window Style
	defaultStyle.windowStyle.borderLowerColor = 0xFF000000;
	defaultStyle.windowStyle.borderUpperColor = 0xFF050545;
	defaultStyle.windowStyle.lowerBackgroundColor = 0xFF808080;
	defaultStyle.windowStyle.upperBackgroundColor = 0xFF101010;
	defaultStyle.windowStyle.upperTitleBackgroundColor = 0xFF000000;
	defaultStyle.windowStyle.lowerTitleBackgroundColor = 0xFF000000;
	defaultStyle.windowStyle.titleTextColor = 0xFFEEEEEE;
	defaultStyle.windowStyle.borderPixelThickness = 2;
	defaultStyle.windowStyle.titlePixelPadding = 2;
	defaultStyle.windowStyle.font = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);
	
	//Label Style
	defaultStyle.labelStyle.textColor = 0xFF000000;
	defaultStyle.labelStyle.font = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);

	return defaultStyle;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGUISearchElementByIdentifier(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash,
	uint32 p_BufferOffsetInBytes, uint32 p_BufferSizeInBytes)
{
	K15_GUIElement* element = 0;
	byte* dataBuffer = p_GUIContext->memoryBuffer + p_BufferOffsetInBytes;
	uint32 offsetInBytes = 0;

	while (offsetInBytes < p_BufferSizeInBytes)
	{
		K15_GUIElement* currentElement = (K15_GUIElement*)(dataBuffer + offsetInBytes);

		if (currentElement->identifierHash == p_IdentifierHash)
		{
			element = currentElement;
			break;
		}
		offsetInBytes += currentElement->sizeInBytes;
	}

	return element;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGUIGetRetainedElement(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash)
{
	uint32 retainedDataBufferOffset = p_GUIContext->retainedDataOffsetInBytes;
	uint32 retainedDataBufferSize = p_GUIContext->retainedDataSizeInBytes;
	return K15_InternalGUISearchElementByIdentifier(p_GUIContext, p_IdentifierHash,
		retainedDataBufferOffset, retainedDataBufferSize);
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGUIGetElement(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash)
{
	uint32 dataSizeInBytes = p_GUIContext->memoryCurrentSizeInBytes;
	return K15_InternalGUISearchElementByIdentifier(p_GUIContext, p_IdentifierHash,
		0, dataSizeInBytes);
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
		p_GUIContext->retainedDataOffsetInBytes;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalGUIGetGUIElementFromRetainedDataBuffer(K15_GUIContext* p_GUIContext,
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

		offset += currentElement->sizeInBytes;
	}

	return retainedElement;
}
/*********************************************************************************/
// struct K15_GUIGetRetainedElementInfo
// {
// 	uint32 identifierHash;
// 	K15_GUIElement* retainedElement;
// };
/*********************************************************************************/
struct K15_GUIMousePickQuery
{
	uint16 mousePosX;
	uint16 mousePosY;
	K15_GUIElement* pickedElement;
};
/*********************************************************************************/
intern void K15_InternalGUIPickGUIElementWithMouse(K15_GUIContext* p_GUIContext, K15_GUIElement* p_GUIElement,
	void* p_UserData)
{
	K15_GUIMousePickQuery* mousePickQuery = (K15_GUIMousePickQuery*)p_UserData;

	bool8 pickedElement = K15_Collision2DBoxPoint(p_GUIElement->rect.pixelPosLeft, p_GUIElement->rect.pixelPosTop,
		p_GUIElement->rect.pixelPosRight, p_GUIElement->rect.pixelPosBottom,
		mousePickQuery->mousePosX, mousePickQuery->mousePosY);

	if (pickedElement)
	{
		mousePickQuery->pickedElement = p_GUIElement;
	}
}
/*********************************************************************************/
intern void K15_InternalGUISetHoveredElement(K15_GUIContext* p_GUIContext, K15_GUIElement* p_Element)
{
	uint32 prevHoveredElementIdHash = p_GUIContext->hoveredElementIdHash;

	K15_GUIElement* prevHoveredElement =
		K15_InternalGUIGetElement(p_GUIContext, prevHoveredElementIdHash);

	if (prevHoveredElement)
	{
		prevHoveredElement->flagMask &= ~K15_GUI_ELEMENT_HOVERED;
	}

	if (p_Element)
	{
		p_GUIContext->hoveredElementIdHash = p_Element->identifierHash;
		p_Element->flagMask |= K15_GUI_ELEMENT_HOVERED;
	}
}
/*********************************************************************************/
intern void K15_InternalGUISetMouseDownElement(K15_GUIContext* p_GUIContext, K15_GUIElement* p_Element)
{
	uint32 prevMouseDownElementIdHash = p_GUIContext->mouseDownElementIdHash;

	K15_GUIElement* prevMouseDownElement =
		K15_InternalGUIGetElement(p_GUIContext, prevMouseDownElementIdHash);

	if (prevMouseDownElement)
	{
		prevMouseDownElement->flagMask &= ~K15_GUI_ELEMENT_MOUSE_DOWN;
	}

	if (p_Element)
	{
		p_GUIContext->mouseDownElementIdHash = p_Element->identifierHash;
		p_Element->flagMask |= K15_GUI_ELEMENT_MOUSE_DOWN;
	}
}
/*********************************************************************************/
intern void K15_InternalSetFocusedElement(K15_GUIContext* p_GUIContext, K15_GUIElement* p_Element)
{
	uint32 prevFocusedElementIdHash = p_GUIContext->focusedElementIdHash;
	p_GUIContext->focusedElementIdHash = 0;

	K15_GUIElement* prevFocusedDownElement =
		K15_InternalGUIGetElement(p_GUIContext, prevFocusedElementIdHash);

	if (prevFocusedDownElement)
	{
		prevFocusedDownElement->flagMask &= ~K15_GUI_ELEMENT_FOCUSED;
	}

	if (p_Element)
	{
		p_GUIContext->focusedElementIdHash = p_Element->identifierHash;
		p_Element->flagMask |= K15_GUI_ELEMENT_FOCUSED;

		//set element to clicked when it was also the element that has the last mouse down state
		if (p_Element->flagMask & K15_GUI_ELEMENT_MOUSE_DOWN)
		{
			p_GUIContext->clickedElementIdHash = p_Element->identifierHash;
			p_Element->flagMask |= K15_GUI_ELEMENT_CLICKED;
			p_Element->flagMask &= ~K15_GUI_ELEMENT_MOUSE_DOWN;
		}
	}
}
/*********************************************************************************/
intern void K15_InternalGUIHandleMouseMoved(K15_GUIContext* p_GUIContext, uint16 p_MousePosX, uint16 p_MousePosY)
{
	K15_GUIMousePickQuery mousePickQuery = {};
	mousePickQuery.mousePosX = p_MousePosX;
	mousePickQuery.mousePosY = p_MousePosY;

	K15_GUIIterateElements(p_GUIContext, K15_InternalGUIPickGUIElementWithMouse, &mousePickQuery);
	K15_InternalGUISetHoveredElement(p_GUIContext, mousePickQuery.pickedElement);
}
/*********************************************************************************/
intern void K15_InternalGUIHandleMousePressed(K15_GUIContext* p_GUIContext, K15_GUIMouseInput* p_MouseInput,
	uint16 p_MousePosX, uint16 p_MousePosY)
{
	K15_GUIMousePickQuery mousePickQuery = {};
	mousePickQuery.mousePosX = p_MousePosX;
	mousePickQuery.mousePosY = p_MousePosY;

	K15_GUIIterateElements(p_GUIContext, K15_InternalGUIPickGUIElementWithMouse, &mousePickQuery);
	K15_InternalGUISetMouseDownElement(p_GUIContext, mousePickQuery.pickedElement);
}
/*********************************************************************************/
intern void K15_InternalGUIHandleMouseReleased(K15_GUIContext* p_GUIContext, K15_GUIMouseInput* p_MouseInput,
	uint16 p_MousePosX, uint16 p_MousePosY)
{
	K15_GUIMousePickQuery mousePickQuery = {};
	mousePickQuery.mousePosX = p_MousePosX;
	mousePickQuery.mousePosY = p_MousePosY;

	K15_GUIIterateElements(p_GUIContext, K15_InternalGUIPickGUIElementWithMouse, &mousePickQuery);
	K15_InternalSetFocusedElement(p_GUIContext, mousePickQuery.pickedElement);
}
/*********************************************************************************/
intern void K15_InternalGUIHandleMouseWheel(K15_GUIContext* p_GUIContext, K15_GUIMouseInput* p_MouseInput,
	uint16 p_MousePosX, uint16 p_MousePosY)
{

}
/*********************************************************************************/
intern void K15_InternalGUIHandleMouseInput(K15_GUIContext* p_GUIContext, K15_GUIContextInput* p_Input)
{
	uint16 oldMousePosX = p_Input->mousePosX;
	uint16 oldMousePosY = p_Input->mousePosY;
	uint16 newMousePosX = p_Input->mousePosX;
	uint16 newMousePosY = p_Input->mousePosY;

	for (uint32 mouseInputIndex = 0;
		mouseInputIndex < p_Input->numBufferedMouseInputs;
		++mouseInputIndex)
	{
		K15_GUIMouseInput* mouseInput = (p_Input->bufferedMouseInput + mouseInputIndex);

		if (mouseInput->type == K15_GUI_MOUSE_MOVED)
		{
			newMousePosX = mouseInput->mousePosX;
			newMousePosY = mouseInput->mousePosY;
			K15_InternalGUIHandleMouseMoved(p_GUIContext, newMousePosX, newMousePosY);
		}
		else if (mouseInput->type == K15_GUI_MOUSE_BUTTON_PRESSED)
		{
			K15_InternalGUIHandleMousePressed(p_GUIContext, mouseInput, newMousePosX, newMousePosY);
		}
		else if (mouseInput->type == K15_GUI_MOUSE_BUTTON_RELEASED)
		{
			K15_InternalGUIHandleMouseReleased(p_GUIContext, mouseInput, newMousePosX, newMousePosY);
		}
		else if (mouseInput->type == K15_GUI_MOUSE_WHEEL_DOWN ||
			mouseInput->type == K15_GUI_MOUSE_WHEEL_UP)
		{
			K15_InternalGUIHandleMouseWheel(p_GUIContext, mouseInput, newMousePosX, newMousePosY);
		}
	}

	p_Input->mouseDeltaX = newMousePosX - oldMousePosX;
	p_Input->mouseDeltaY = newMousePosY - oldMousePosY;
	p_Input->mousePosX = newMousePosX;
	p_Input->mousePosY = newMousePosY;

	p_Input->numBufferedMouseInputs = 0;
}
/*********************************************************************************/
intern void K15_InternalGUIHandleKeyboardInput(K15_GUIContext* p_GUIContext, K15_GUIContextInput* p_Input)
{
	for (uint32 inputIndex = 0;
		inputIndex < p_Input->numBufferedKeyboardInputs;
		++inputIndex)
	{
		K15_GUIKeyboardInput* keyboardInput = p_Input->bufferedKeyboardInput + inputIndex;

		if (keyboardInput->keyType == K15_GUI_KEY_F1 &&
			keyboardInput->type == K15_GUI_KEY_PRESSED)
		{
			//Toggle debug mode
			p_GUIContext->debugModeActive = !p_GUIContext->debugModeActive;
		}
	}

	p_Input->numBufferedKeyboardInputs = 0;
}
/*********************************************************************************/
intern void K15_InternalGUIHandleInput(K15_GUIContext* p_GUIContext)
{
	K15_GUIContextInput* input = &p_GUIContext->input;

	//reset clicked element
	K15_GUIElement* clickedElement = K15_InternalGUIGetElement(p_GUIContext, p_GUIContext->clickedElementIdHash);

	if (clickedElement)
	{
		clickedElement->flagMask &= ~K15_GUI_ELEMENT_CLICKED;
	}

	p_GUIContext->clickedElementIdHash = 0;

	K15_InternalGUIHandleMouseInput(p_GUIContext, input);
	K15_InternalGUIHandleKeyboardInput(p_GUIContext, input);
}
/*********************************************************************************/
intern void K15_InternalSetGUIRectCoordinates(K15_GUIRectangle* p_GUIRectangle,
	int32* p_PosLeft, int32* p_PosTop, uint32* p_Width, uint32* p_Height)
{
	if (p_PosLeft)
	{
		p_GUIRectangle->pixelPosLeft = *p_PosLeft;
	}

	if (p_PosTop)
	{
		p_GUIRectangle->pixelPosTop = *p_PosTop;
	}

	if (p_Width)
	{
		p_GUIRectangle->pixelPosRight = p_GUIRectangle->pixelPosLeft + *p_Width;
	}

	if (p_Height)
	{
		p_GUIRectangle->pixelPosBottom = p_GUIRectangle->pixelPosTop + *p_Height;
	}
}
/*********************************************************************************/
intern void K15_InternalRetrieveGUIRectCoordinates(K15_GUIRectangle* p_GUIRectangle,
	int32* p_PosLeft, int32* p_PosTop, uint32* p_Width, uint32* p_Height)
{
	if (p_PosLeft)
	{
		*p_PosLeft = p_GUIRectangle->pixelPosLeft;
	}

	if (p_PosTop)
	{
		*p_PosTop = p_GUIRectangle->pixelPosTop;
	}

	if (p_Width)
	{
		*p_Width = (p_GUIRectangle->pixelPosRight - p_GUIRectangle->pixelPosLeft);
	}

	if (p_Height)
	{
		*p_Height = (p_GUIRectangle->pixelPosBottom - p_GUIRectangle->pixelPosTop);
	}
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalAddUnalignedGUIElement(K15_GUIContext* p_GUIContext, K15_GUIElementType p_GUIElementType,
	int32* p_PosLeft, int32* p_PosTop, uint32* p_Width, uint32* p_Height, const char* p_Identifier)
{
	K15_ASSERT(p_GUIContext->memoryCurrentSizeInBytes <= p_GUIContext->memoryMaxSizeInBytes);
	//so what do we need to do here?
	// - Add GUI Element to GUI Context (so we can iterate over it)
	// - Retrieve 'old' GUI Element (so we can retrieve the information (clicked or not etc.)
	// - Add GUI Element Rect to layout
	uint32 identifierHash = K15_GenerateStringHash(p_Identifier);

	//Add element to gui memory buffer
	K15_GUIElement* element = (K15_GUIElement*)(p_GUIContext->memoryBuffer + p_GUIContext->memoryCurrentSizeInBytes);
	element->identifierHash = identifierHash;
	element->type = p_GUIElementType;
	element->offsetInBytes = p_GUIContext->memoryCurrentSizeInBytes;
	element->sizeInBytes = sizeof(K15_GUIElement);
	
	if (p_GUIContext->layoutIndex > 0)
	{
		element->flagMask |= K15_GUI_ELEMENT_LAYOUTED;
	}

	p_GUIContext->memoryCurrentSizeInBytes += sizeof(K15_GUIElement);

	//try to retrieve the retained informations from the gui element
	K15_GUIElement* retainedElement = (K15_GUIElement*)K15_InternalGUIGetRetainedElement(p_GUIContext, identifierHash);

	if (retainedElement)
	{
		if (retainedElement->flagMask & K15_GUI_ELEMENT_LAYOUTED)
		{
			element->rect = retainedElement->rect;
			K15_InternalRetrieveGUIRectCoordinates(&element->rect, p_PosLeft, p_PosTop, p_Width, p_Height);
		}
		else
		{
			K15_InternalSetGUIRectCoordinates(&element->rect, p_PosLeft, p_PosTop, p_Width, p_Height);
		}
		element->flagMask = retainedElement->flagMask;
	}
	else
	{
		element->flagMask = 0;
		K15_InternalSetGUIRectCoordinates(&element->rect, p_PosLeft, p_PosTop, p_Width, p_Height);
	}
	K15_InternalGUIAddGUIRectToTopLayout(p_GUIContext, &element->rect);

	return element;
}
/*********************************************************************************/
intern K15_GUIElement* K15_InternalAddAlignedGUIElement(K15_GUIContext* p_GUIContext, K15_GUIElementType p_GUIElementType,
	const char* p_Identifier)
{
	return K15_InternalAddUnalignedGUIElement(p_GUIContext, p_GUIElementType, 0, 0, 0, 0, p_Identifier);
}
/*********************************************************************************/
intern byte* K15_InternalAddGUIElementMemory(K15_GUIContext* p_GUIContext, K15_GUIElement* p_GUIElement,
	uint32 p_SizeMemoryInBytes)
{
	//byte* retainedBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
	uint32 offsetInBytes = p_GUIElement->offsetInBytes + p_GUIElement->sizeInBytes;

	K15_ASSERT(offsetInBytes + p_SizeMemoryInBytes <= p_GUIContext->memoryMaxSizeInBytes);

	byte* elementMemory = p_GUIContext->memoryBuffer + offsetInBytes;

	p_GUIElement->sizeInBytes += p_SizeMemoryInBytes;
	p_GUIContext->memoryCurrentSizeInBytes += p_SizeMemoryInBytes;

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
	layout->type = p_LayoutType;
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
	K15_GUIContext* guiContext = (K15_GUIContext*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, 
		sizeof(K15_GUIContext));

	uint32 windowHeight = 0;
	uint32 windowWidth = 0;

	uint32 bookKeeping = sizeof(K15_GUIContext) + 512;

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

	guiContext->memoryLock = K15_CreateSemaphoreWithInitialValue(1);
	guiContext->memoryAllocator = p_MemoryAllocator;
	guiContext->retainedDataSizeInBytes = 0;
	guiContext->memoryCurrentSizeInBytes = 0;
	guiContext->focusedElementIdHash = 0;
	guiContext->hoveredElementIdHash = 0;
	guiContext->clickedElementIdHash = 0;
	guiContext->mouseDownElementIdHash = 0;
	guiContext->layoutIndex = 0;
	guiContext->numLayouts = 0;
	guiContext->memoryBuffer = guiMemory;
	guiContext->retainedDataOffsetInBytes = guiMemoryBufferSize;
	guiContext->memoryMaxSizeInBytes = guiMemoryBufferSize;
	guiContext->style = K15_InternalCreateDefaultStyle(p_ResourceContext);
	guiContext->input.numBufferedKeyboardInputs = 0;
	guiContext->input.numBufferedMouseInputs = 0;
	guiContext->debugModeActive = K15_FALSE;
	return guiContext;
}
/*********************************************************************************/
byte* K15_GUIGetGUIElementMemory(K15_GUIElement* p_GUIElement)
{
	return (byte*)(p_GUIElement + 1);
}
/*********************************************************************************/
void K15_GUIBeginDockingArea(K15_GUIContext* p_GUIContext, int32 p_PosX, int32 p_PosY,
	uint32 p_Width, uint32 p_Height, uint32 p_AllowedDockingAreasMask)
{
	
}
/*********************************************************************************/
bool8 K15_GUIBeginWindow(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Title, const char* p_Identifier)
{
	K15_GUIContextStyle* style = &p_GUIContext->style;
	return K15_GUIBeginWindowEX(p_GUIContext, p_PosX, p_PosY, p_Width, p_Height, p_Title, 
		p_Identifier, &style->windowStyle);
}
/*********************************************************************************/
bool8 K15_GUIBeginWindowEX(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier, const char* p_Title,
	K15_GUIWindowStyle* p_GUIWindowStyle)
{
	K15_GUIElement* windowElement = K15_InternalAddUnalignedGUIElement(p_GUIContext, K15_GUI_WINDOW,
		p_PosX, p_PosY, p_Width, p_Height, p_Identifier);

	if (windowElement->flagMask & K15_GUI_ELEMENT_MOUSE_DOWN)
	{
		*p_PosX += p_GUIContext->input.mouseDeltaX;
		*p_PosY += p_GUIContext->input.mouseDeltaY;
	}

	uint32 titleLength = (uint32)strlen(p_Title);
	uint32 sizeElementMemoryInBytes = sizeof(K15_GUIWindowData);

	byte* windowElementMemory = K15_InternalAddGUIElementMemory(p_GUIContext, windowElement, sizeElementMemoryInBytes);
	char* titleMemory = (char*)K15_InternalAddGUIElementMemory(p_GUIContext, windowElement, titleLength);

	K15_GUIWindowData windowData = {};
	windowData.style = p_GUIWindowStyle;
	windowData.textLength = titleLength;
	windowData.title = titleMemory;
	windowData.contentRect = windowElement->rect;
	windowData.contentRect.pixelPosTop += 20;

	memcpy(titleMemory, p_Title, titleLength);
	memcpy(windowElementMemory, &windowData, sizeof(K15_GUIWindowData));

	//Add layout for this window
	K15_InternalGUIPushLayout(p_GUIContext, &windowData.contentRect, K15_GUI_HORIZONTAL_LAYOUT);

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
	K15_GUIElement* buttonElement = K15_InternalAddAlignedGUIElement(p_GUIContext, K15_GUI_BUTTON, p_Identifier);

	bool8 active = buttonElement->flagMask & K15_GUI_ELEMENT_CLICKED;

	uint32 textLength = (uint32)strlen(p_ButtonText);
	uint32 sizeButtonDataInBytes = sizeof(K15_GUIButtonData);

	byte* buttonElementMemory = (byte*)K15_InternalAddGUIElementMemory(p_GUIContext, buttonElement, sizeButtonDataInBytes);
	char* textMemory = (char*)K15_InternalAddGUIElementMemory(p_GUIContext, buttonElement, textLength);

	K15_GUIButtonData buttonData = {};
	buttonData.style = p_GUIButtonStyle;
	buttonData.textLength = textLength;
	buttonData.text = textMemory;

	memcpy(buttonElementMemory, &buttonData, sizeof(K15_GUIButtonData));
	memcpy(textMemory, p_ButtonText, textLength);

	return active;
}
/*********************************************************************************/
void K15_GUILabel(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier)
{
	K15_GUILabelEX(p_GUIContext, p_LabelText, p_Identifier, &p_GUIContext->style.labelStyle);
}
/*********************************************************************************/
void K15_GUILabelEX(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier, 
	K15_GUILabelStyle* p_GUILabelStyle)
{
	K15_GUIElement* labelElement = K15_InternalAddAlignedGUIElement(p_GUIContext, K15_GUI_LABEL, p_Identifier);

	uint32 textLength = (uint32)strlen(p_LabelText);
	
	byte* labelElementMemory = (byte*)K15_InternalAddGUIElementMemory(p_GUIContext, labelElement, sizeof(K15_GUILabelData));
	char* textMemory = (char*)K15_InternalAddGUIElementMemory(p_GUIContext, labelElement, textLength);

	K15_GUILabelData labelData = {};
	labelData.style = p_GUILabelStyle;
	labelData.textLength = textLength;
	labelData.text = textMemory;

	memcpy(labelElementMemory, &labelData, sizeof(K15_GUILabelData));
	memcpy(textMemory, p_LabelText, textLength);
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
intern void K15_InternalGUIAlignElements(K15_GUILayout* p_Layouts, uint16 p_NumLayouts)
{
	for (uint16 layoutIndex = 0;
		layoutIndex < p_NumLayouts;
		++layoutIndex)
	{
		K15_GUILayout* layout = p_Layouts + layoutIndex;
		K15_GUILayoutType type = layout->type;
		uint16 numElements = layout->numElements;
		uint16 layoutWidth = layout->layoutRectangle.pixelPosRight - layout->layoutRectangle.pixelPosLeft;
		uint16 layoutHeight = layout->layoutRectangle.pixelPosBottom - layout->layoutRectangle.pixelPosTop;
		uint16 layoutPosX = layout->layoutRectangle.pixelPosLeft;
		uint16 layoutPosY = layout->layoutRectangle.pixelPosTop;
		uint16 offsetX = 0;
		uint16 offsetY = 0;

		if (numElements == 0)
		{
			continue;
		}

		uint32 elementWidth = (layoutWidth / numElements);

		for (uint16 elementIndex = 0;
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
				offsetY += 20;
				break;

			case K15_GUI_HORIZONTAL_LAYOUT:
				rect->pixelPosRight = rect->pixelPosLeft + elementWidth;
				rect->pixelPosBottom = rect->pixelPosTop + 20;
				offsetX += elementWidth;
 
				break;
			}
		}
	}
}
/*********************************************************************************/
void K15_GUIFinishGUIFrame(K15_GUIContext* p_GUIContext)
{
	K15_InternalGUIAlignElements(p_GUIContext->layoutStack, p_GUIContext->numLayouts);
	K15_InternalGUIHandleInput(p_GUIContext);

	byte* retainedBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);

	//wait until we can safely access the retained memory
	K15_WaitSemaphore(p_GUIContext->memoryLock);

	//load of current frame gui data to retained memory segment
	memcpy(retainedBuffer, p_GUIContext->memoryBuffer,
		p_GUIContext->memoryCurrentSizeInBytes);

	p_GUIContext->layoutIndex = 0;
	p_GUIContext->numLayouts = 0;
	p_GUIContext->retainedDataSizeInBytes = p_GUIContext->memoryCurrentSizeInBytes;
	p_GUIContext->memoryCurrentSizeInBytes = 0;
}
/*********************************************************************************/
intern void K15_InternalIterateGUIElements(K15_GUIContext* p_GUIContext, 
	byte* p_Buffer, uint32 p_BufferSizeInBytes, K15_GUIIteratorFnc p_Function, void* p_UserData)
{
	uint32 offsetInBytes = 0;

	while (offsetInBytes < p_BufferSizeInBytes)
	{
		K15_GUIElement* element = (K15_GUIElement*)(p_Buffer + offsetInBytes);
		p_Function(p_GUIContext, element, p_UserData);

		offsetInBytes += element->sizeInBytes;
	}
}
/*********************************************************************************/
void K15_GUIIterateElements(K15_GUIContext* p_GUIContext, K15_GUIIteratorFnc p_Function, void* p_UserData)
{
	K15_InternalIterateGUIElements(p_GUIContext, p_GUIContext->memoryBuffer, p_GUIContext->memoryCurrentSizeInBytes,
		p_Function, p_UserData);
}
/*********************************************************************************/
void K15_GUIIterateRetainedElements(K15_GUIContext* p_GUIContext, K15_GUIIteratorFnc p_Function, void* p_UserData)
{
	byte* retainedMemoryBuffer = K15_InternalGUIGetRetainedDataBuffer(p_GUIContext);
	K15_InternalIterateGUIElements(p_GUIContext, retainedMemoryBuffer, p_GUIContext->retainedDataSizeInBytes,
		p_Function, p_UserData);
}
/*********************************************************************************/
void K15_GUIAddMouseInput(K15_GUIContextInput* p_GUIContextInput, K15_GUIMouseInput* p_MouseInput)
{
	K15_ASSERT(p_GUIContextInput);
	K15_ASSERT(p_MouseInput);

	uint32 mouseInputIndex = p_GUIContextInput->numBufferedMouseInputs++;
	K15_ASSERT(mouseInputIndex != K15_GUI_MAX_BUFFERED_MOUSE_INPUTS);

	p_GUIContextInput->bufferedMouseInput[mouseInputIndex] = *p_MouseInput;
}
/*********************************************************************************/
void K15_GUIAddKeyboardInput(K15_GUIContextInput* p_GUIContextInput, K15_GUIKeyboardInput* p_KeyboardInput)
{
	K15_ASSERT(p_GUIContextInput);
	K15_ASSERT(p_KeyboardInput);

	uint32 keyInputIndex = p_GUIContextInput->numBufferedKeyboardInputs++;
	K15_ASSERT(keyInputIndex != K15_GUI_MAX_BUFFERED_KEYBOARD_INPUTS);

	p_GUIContextInput->bufferedKeyboardInput[keyInputIndex] = *p_KeyboardInput;
}
/*********************************************************************************/