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

	defaultStyle.windowTitlePixelHeight = 20;
	defaultStyle.sliderPixelHeight = 18;
	defaultStyle.sliderPixelWidth = 10;
	defaultStyle.sliderLinePixelWidth = 150;
	defaultStyle.windowContentPixelPadding = 5;
	defaultStyle.buttonContentPixelPadding = 3;

	defaultStyle.controlUpperBackgroundColor = 0x303030;
	defaultStyle.controlLowerBackgroundColor = 0x505050;
	defaultStyle.controlUpperBorderColor = 0x606060;
	defaultStyle.controlLowerBorderColor = 0x101010;
	defaultStyle.hoveredControlUpperBackgroundColor = 0x476e72;
	defaultStyle.hoveredControlLowerBackgroundColor = 0x3a5357;
	defaultStyle.interactedControlLowerBackgroundColor = 0x476e72;
	defaultStyle.interactedControlUpperBackgroundColor = 0x488085;
	defaultStyle.textColor = 0xFFFFFF;
	defaultStyle.windowTitleTextColor = 0xEEEEEE;
	defaultStyle.windowTitleBarLowerColor = 0x4580FF;
	defaultStyle.windowTitleBarUpperColor = 0x4544FF;
	defaultStyle.windowUpperBackgroundColor = 0x101010;
	defaultStyle.windowLowerBackgroundColor = 0x808080;
	defaultStyle.windowBorderUpperColor = 0x050545;
	defaultStyle.windowBorderLowerColor = 0x000000;
	defaultStyle.styleFont = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);

	return defaultStyle;
}
/*********************************************************************************/
struct K15_InternalGetGUIElementLastFrameInfo
{
	K15_GUIElementHeader* element;
	K15_GUIElementHeader* elementLastFrame;
};
/*********************************************************************************/
intern void K15_InternalGetGUIElementLastFrameHelper(K15_GUIContext* p_GUIContext,
	K15_GUIElementHeader* p_GUIElementHeader, void* p_UserData)
{
	K15_InternalGetGUIElementLastFrameInfo* info = 
		(K15_InternalGetGUIElementLastFrameInfo*)p_UserData;;

	if (info->elementLastFrame)
	{
		return;
	}

	uint32 elementIdentifierHash = p_GUIElementHeader->identifierHash;

	if (elementIdentifierHash == info->element->identifierHash)
	{
		info->elementLastFrame = p_GUIElementHeader;
	}
}
/*********************************************************************************/
intern K15_GUIElementHeader* K15_InternalGetGUIElementLastFrame(K15_GUIContext* p_GUIContext, 
	K15_GUIElementHeader* p_GUIElementHeader)
{
	K15_InternalGetGUIElementLastFrameInfo info = {};
	info.element = p_GUIElementHeader;

	K15_IterateGUIElementsHelper(p_GUIContext, K15_InternalGetGUIElementLastFrameHelper, 
		(void*)&info, K15_GUI_MEMORY_BACK_BUFFER);

	return info.elementLastFrame;
}
/*********************************************************************************/
#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
/*********************************************************************************/
struct K15_InternalGUIDuplicateIdentifierInfo
{
	uint32 identifierHash;
	bool8 foundDuplicate;
};
/*********************************************************************************/
intern void K15_InternalCheckForDuplicateIdentifiers(K15_GUIContext* p_GUIContext, 
	K15_GUIElementHeader* p_GUIElementHeader, void* p_IdentifierInfo)
{
	K15_InternalGUIDuplicateIdentifierInfo* duplicateIdentifierInfo =
		(K15_InternalGUIDuplicateIdentifierInfo*)p_IdentifierInfo;

	if (duplicateIdentifierInfo->foundDuplicate)
	{
		return;
	}

	uint32 elementIdentifier = duplicateIdentifierInfo->identifierHash;
	bool8 foundDuplicate = p_GUIElementHeader->identifierHash == elementIdentifier;

	duplicateIdentifierInfo->foundDuplicate = foundDuplicate;
}
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
/*********************************************************************************/
intern K15_GUILayout* K15_InternalPushLayoutCategory(K15_GUIContext* p_GUIContext, K15_GUILayoutType p_LayoutType,
	uint32 p_ElementPixelPadding, int32 p_LeftPixelPos, int32 p_TopPixelPos, uint32 p_PixelWidth, uint32 p_PixelHeight)
{
	K15_ASSERT(p_GUIContext);

	uint32 layoutIndex = p_GUIContext->layoutIndex;

	K15_ASSERT(layoutIndex != K15_GUI_MAX_LAYOUTS);

	K15_GUILayout* layout = &p_GUIContext->layoutStack[layoutIndex];
	layout->type = p_LayoutType;
	layout->pixelPosX = p_LeftPixelPos;
	layout->pixelPosY = p_TopPixelPos;
	layout->pixelWidth = p_PixelWidth;
	layout->pixelHeight = p_PixelHeight;
	layout->pixelOffsetX = 0;
	layout->pixelOffsetY = 0;
	layout->elementPixelPadding = p_ElementPixelPadding;
	layout->elementCount = 0;

	p_GUIContext->layoutIndex = layoutIndex + 1;

	return layout;
}
/*********************************************************************************/
intern K15_GUILayout* K15_InternalGetTopGUILayout(K15_GUIContext* p_GUIContext)
{
	K15_GUILayout* topLayout = 0;

	if (p_GUIContext->layoutIndex > 0)
	{
		uint32 topLayoutIndex = p_GUIContext->layoutIndex - 1;
		topLayout = &p_GUIContext->layoutStack[topLayoutIndex];
	}

	return topLayout;
}
/*********************************************************************************/
intern void K15_InternalGetAlignedGUIDimension(K15_GUIContext* p_GUIContext,
	int32* p_LeftPixelPos, int32* p_TopPixelPos, uint32* p_PixelWidth, uint32* p_PixelHeight)
{
	K15_GUILayout* topLayout = K15_InternalGetTopGUILayout(p_GUIContext);

	if (topLayout)
	{
		K15_GUILayoutType topLayoutType = topLayout->type;

		uint32 pixelOffsetX = topLayout->pixelOffsetX;
		uint32 pixelOffsetY = topLayout->pixelOffsetY;
		uint32 elementPixelPadding = topLayout->elementPixelPadding;

		uint32 leftPixelPos = p_LeftPixelPos ? *p_LeftPixelPos : 0;
		uint32 topPixelPos = p_TopPixelPos ? *p_TopPixelPos : 0;
		uint32 pixelWidth = p_PixelWidth ? *p_PixelWidth : 0;
		uint32 pixelHeight = p_PixelHeight ? *p_PixelHeight : 0;

		switch (topLayoutType)
		{
		case K15_GUI_LAYOUT_VERTICAL:
		{
			leftPixelPos = topLayout->pixelPosX + pixelOffsetX;
			topPixelPos = topLayout->pixelPosY + pixelOffsetY + elementPixelPadding;
			pixelWidth = topLayout->pixelWidth;
			pixelOffsetY += pixelHeight + elementPixelPadding * 2;

			break;
		}

		case K15_GUI_LAYOUT_HORIZONTAL:
		{
			leftPixelPos = topLayout->pixelPosX + pixelOffsetX + elementPixelPadding;
			topPixelPos = topLayout->pixelPosY + pixelOffsetY;
			pixelHeight = topLayout->pixelHeight;
			pixelOffsetX += pixelWidth + elementPixelPadding * 2;

			break;
		}
		}

		topLayout->pixelOffsetX = pixelOffsetX;
		topLayout->pixelOffsetY = pixelOffsetY;

		if (p_LeftPixelPos)
		{
			*p_LeftPixelPos = leftPixelPos;
		}

		if (p_TopPixelPos)
		{
			*p_TopPixelPos = topPixelPos;
		}

		if (p_PixelWidth)
		{
			*p_PixelWidth = pixelWidth;
		}

		if (p_PixelHeight)
		{
			*p_PixelHeight = pixelHeight;
		}
	}
}
/*********************************************************************************/
intern K15_GUIElementHeader* K15_InternalAddGUIElement(K15_GUIContext* p_GUIContext,
	K15_GUIElementType p_GUIElementType, uint32 p_ElementSizeInBytes, 
	const char* p_Identifier, void** p_ElementPointer,
	uint32 p_MinPixelWidth = UINT_MAX, uint32 p_MinPixelHeight = UINT_MAX)
{
	uint32 offset = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 newOffset = offset + sizeof(K15_GUIElementHeader) + p_ElementSizeInBytes;
	uint32 guiElementIdentifierHash = K15_GenerateStringHash(p_Identifier);

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
	K15_InternalGUIDuplicateIdentifierInfo identifierInfo = {};
	identifierInfo.identifierHash = guiElementIdentifierHash;
	K15_IterateGUIElementsHelper(p_GUIContext, K15_InternalCheckForDuplicateIdentifiers, 
		(void*)&identifierInfo, K15_GUI_MEMORY_FRONT_BUFFER);

	K15_ASSERT_TEXT(!identifierInfo.foundDuplicate, 
		"Found duplicate for identifier '%s'. Please use a different identifier.", p_Identifier);
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = newOffset;

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	K15_GUILayout* topLayout = K15_InternalGetTopGUILayout(p_GUIContext);

	if (topLayout)
	{
		++topLayout->elementCount;
	}

	K15_GUIElementHeader* elementHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	*p_ElementPointer = (guiContextFrontBuffer + offset + sizeof(K15_GUIElementHeader));

	if (p_MinPixelWidth != UINT_MAX)
	{
		elementHeader->pixelWidth = p_MinPixelWidth;
	}

	if (p_MinPixelHeight != UINT_MAX)
	{
		elementHeader->pixelHeight = p_MinPixelHeight;
	}

	K15_InternalGetAlignedGUIDimension(p_GUIContext, &elementHeader->posPixelX, &elementHeader->posPixelY,
		&elementHeader->pixelWidth, &elementHeader->pixelHeight);

	bool8 hoveredElement = K15_FALSE;
	bool8 mouseInside = K15_Collision2DBoxPoint(elementHeader->posPixelX, elementHeader->posPixelY,
		elementHeader->posPixelX + elementHeader->pixelWidth,
		elementHeader->posPixelY + elementHeader->pixelHeight,
		p_GUIContext->mousePosPixelX, p_GUIContext->mousePosPixelY);

	if (p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash &&
		!mouseInside)
	{
		p_GUIContext->hoveredElementIdentifier = 0;
	}
	else if (p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash ||
		(p_GUIContext->hoveredElementIdentifier == 0 &&
		mouseInside))
	{
		p_GUIContext->hoveredElementIdentifier = guiElementIdentifierHash;
		hoveredElement = K15_TRUE;
	}

	bool8 activatedElement = K15_FALSE;

	if (p_GUIContext->activeElementIdentifier == guiElementIdentifierHash &&
		!p_GUIContext->leftMouseDown)
	{
		p_GUIContext->activeElementIdentifier = 0;
	}
	else if (p_GUIContext->activeElementIdentifier == guiElementIdentifierHash  ||
		(p_GUIContext->activeElementIdentifier == 0 &&
		p_GUIContext->leftMouseDown &&
		mouseInside))
	{
		p_GUIContext->activeElementIdentifier = guiElementIdentifierHash;
		activatedElement = K15_TRUE;
	}

	elementHeader->hovered = hoveredElement;
	elementHeader->activated = activatedElement;
	elementHeader->offset = offset;
	elementHeader->elementSizeInBytes = p_ElementSizeInBytes + sizeof(K15_GUIElementHeader);
	elementHeader->type = p_GUIElementType;
	elementHeader->identifierHash = guiElementIdentifierHash;

	return elementHeader;
}
/*********************************************************************************/
intern void K15_InternalPushVerticalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding,
	int32 p_LeftPixelPos, int32 p_TopPixelPos, uint32 p_PixelWidth, uint32 p_PixelHeight)
{
	K15_GUILayout* verticalLayout = K15_InternalPushLayoutCategory(p_GUIContext,
		K15_GUI_LAYOUT_VERTICAL, p_ElementPadding,
		p_LeftPixelPos, p_TopPixelPos, p_PixelWidth, p_PixelHeight);
}
/*********************************************************************************/
intern void K15_InternalPushHorizontalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding,
	int32 p_LeftPixelPos, int32 p_TopPixelPos, uint32 p_PixelWidth, uint32 p_PixelHeight)
{
	K15_GUILayout* horizontalLayout = K15_InternalPushLayoutCategory(p_GUIContext,
		K15_GUI_LAYOUT_HORIZONTAL, p_ElementPadding,
		p_LeftPixelPos, p_TopPixelPos, p_PixelWidth, p_PixelHeight);
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
	uint32 guiMemoryBufferSize = actualGUIMemorySize/2; 

	byte* guiMemory = (byte*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, actualGUIMemorySize);
	memset(guiMemory, 0, actualGUIMemorySize);
	
	K15_ASSERT_TEXT(guiMemory, "Could not acquire gui memory (%dkb) from allocator '%s'",
		actualGUIMemorySize / 1024, p_MemoryAllocator.name);

	guiContext->memoryAllocator = p_MemoryAllocator;
	guiContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER] = guiMemory;
	guiContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER] = guiMemory + guiMemoryBufferSize;
	guiContext->guiMemoryMaxSize = guiMemoryBufferSize;
	guiContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = 0;
	guiContext->guiMemoryCurrentSize[K15_GUI_MEMORY_BACK_BUFFER] = 0;
	guiContext->windowHeight = windowHeight;
	guiContext->windowWidth = windowWidth;
	guiContext->mousePosPixelX = 0;
	guiContext->mousePosPixelY = 0;
	guiContext->currentWindow = 0;
	guiContext->layoutIndex = 0;
	guiContext->leftMouseDown = K15_FALSE;
	guiContext->rightMouseDown = K15_FALSE;
	guiContext->memoryLock = K15_CreateSemaphoreWithInitialValue(1);
	guiContext->activeElementIdentifier = 0;
	guiContext->hoveredElementIdentifier = 0;

	//create and assign default style
	guiContext->style = K15_InternalCreateDefaultStyle(p_ResourceContext);

	uint32 flags = 0;

	K15_ResourceHandle guiMaterial = K15_LoadResource(p_ResourceContext, K15_MATERIAL_RESOURCE_IDENTIFIER, "materials/gui_material", flags); 

	guiContext->guiRenderMaterial = K15_GetResourceRenderMaterialDesc(p_ResourceContext, guiMaterial);

	return guiContext;
}
/*********************************************************************************/
void K15_FlipGUIContextMemory(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext);

	//are we allowed to flip the buffers yet?
	K15_WaitSemaphore(p_GUIContext->memoryLock);

	byte* tempBuffer = 0;
	byte** frontBuffer = &p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];
	byte** backBuffer = &p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	tempBuffer = *frontBuffer;
	*frontBuffer = *backBuffer;
	*backBuffer = tempBuffer;

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_BACK_BUFFER] =
		p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];

	uint32 frontBufferMemorySize = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = 0;
}
/*********************************************************************************/
void K15_HandleGUIInputEvent(K15_GUIContext* p_GUIContext, K15_SystemEvent* p_SystemEvent)
{
	if (p_SystemEvent->event == K15_MOUSE_MOVED)
	{
		uint32 posX = p_SystemEvent->params.position.x;
		uint32 posY = p_SystemEvent->params.position.y;

		K15_SetGUIContextMousePosition(p_GUIContext, posX, posY);
	}
	else if (p_SystemEvent->event == K15_MOUSE_BUTTON_PRESSED ||
		p_SystemEvent->event == K15_MOUSE_BUTTON_RELEASED)
	{
		bool8 leftMouse = p_SystemEvent->params.mouseButton == K15_LEFT_MOUSE_BUTTON;
		bool8 rightMouse = p_SystemEvent->params.mouseButton == K15_RIGHT_MOUSE_BUTTON;
		bool8 pressed = p_SystemEvent->event == K15_MOUSE_BUTTON_PRESSED;

		p_GUIContext->leftMouseDown = pressed && leftMouse;
		p_GUIContext->rightMouseDown = pressed && rightMouse;
	}
	else if (p_SystemEvent->event == K15_TEXT_INPUT)
	{
		char character = p_SystemEvent->params.utf8Char;
	}
}
/*********************************************************************************/
void K15_HandleGUIWindowEvent(K15_GUIContext* p_GUIContext, K15_SystemEvent* p_SystemEvent)
{
	if (p_SystemEvent->event == K15_WINDOW_RESIZED)
	{
		uint32 width = p_SystemEvent->params.size.width;
		uint32 height = p_SystemEvent->params.size.height;

		K15_SetGUIContextWindowSize(p_GUIContext, width, height);
	}
}
/*********************************************************************************/
void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight)
{
	K15_ASSERT(p_GUIContext);

	uint32 oldWindowWidth = p_GUIContext->windowWidth;
	uint32 oldWindowHeight = p_GUIContext->windowHeight;

	p_GUIContext->windowHeight = p_WindowHeight;
	p_GUIContext->windowWidth = p_WindowWidth;
}
/*********************************************************************************/
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY)
{
	K15_ASSERT(p_GUIContext);

	p_GUIContext->mousePosPixelX = p_MouseX;
	p_GUIContext->mousePosPixelY = p_MouseY;
}
/*********************************************************************************/
char* K15_ComboBox(K15_GUIContext* p_GUIContext, char** p_Elements, uint32 p_NumElements, const char* p_Identifier)
{
#if 0
	K15_ASSERT(p_GUIContext);

	uint32 lengthAllElements = 0;

	for (uint32 elementIndex = 0;
		elementIndex < p_NumElements;
		++elementIndex)
	{
		lengthAllElements += (uint32)strlen(p_Elements[elementIndex]) + 1; //0 terminator
	}

	uint32 guiElementIdentifierHash = K15_GenerateStringHash(p_Identifier);
	uint32 offset = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 elementOffsetInBytes = offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIComboBox);
	uint32 newOffset = elementOffsetInBytes + lengthAllElements;

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
	K15_ASSERT_TEXT(K15_InternalCheckForDuplicateIdentifiers(p_GUIContext, guiElementIdentifierHash) == K15_FALSE,
		"Found duplicate for identifier '%s'. Please use a different identifier.", p_Identifier);
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = newOffset;

	K15_GUIElementHeader* elementLastFrame = K15_InternalGetGUIElementLastFrame(p_GUIContext, 
		guiElementIdentifierHash);

	uint32 selectedElementIndex = 0;
	bool8 expandedLastFrame = K15_FALSE;

	if (elementLastFrame)
	{
		K15_GUIComboBox* comboBoxLastFrame = (K15_GUIComboBox*)(++elementLastFrame);
		selectedElementIndex = comboBoxLastFrame->selectedIndex;
		expandedLastFrame = comboBoxLastFrame->expanded;
	}

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	K15_GUIElementHeader* comboBoxHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUIComboBox* comboBox = (K15_GUIComboBox*)(guiContextFrontBuffer+ offset + sizeof(K15_GUIElementHeader));
	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	const char* selectedElementString = p_Elements[selectedElementIndex];
	uint32 selectedElementStringLength = (uint32)strlen(selectedElementString);

	float textWidth = 0.f;
	float textHeight = 0.f;

	uint32 expanderPixelWidth = 0;// p_GUIContext->style.guiComboBoxStyle.expanderPixelWidth;
	uint32 expanderPixelHeight = 0;// p_GUIContext->style.guiComboBoxStyle.expanderPixelWidth;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, selectedElementString, selectedElementStringLength);

	comboBoxHeader->identifierHash = guiElementIdentifierHash;
	comboBoxHeader->offset = newOffset;
	comboBoxHeader->type = K15_GUI_TYPE_COMBO_BOX;
	comboBoxHeader->pixelHeight = K15_MAX((uint32)textHeight, expanderPixelHeight);
	comboBoxHeader->pixelWidth = (uint32)textWidth + expanderPixelWidth;
	comboBoxHeader->posPixelX = 10;
	comboBoxHeader->posPixelY = 100;

	comboBox->elementsOffsetInBytes = elementOffsetInBytes;
	comboBox->expanded = expandedLastFrame;
	comboBox->numElements = p_NumElements;
	comboBox->selectedIndex = selectedElementIndex;

	//copy elements incl 0 terminator
	for(uint32 elementIndex = 0;
		elementIndex < p_NumElements;
		++elementIndex)
	{
		uint32 elementLength = (uint32)strlen(p_Elements[elementIndex])+1;
		memcpy(guiContextFrontBuffer + elementOffsetInBytes,
			p_Elements[elementIndex], elementLength);
		elementOffsetInBytes += elementLength;
	}

	return p_Elements[selectedElementIndex];
#endif

	return 0;
}
/*********************************************************************************/
bool8 K15_BeginWindow(K15_GUIContext* p_GUIContext, const char* p_Caption, 
	int32* p_LeftPixelPos, int32* p_TopPixelPos, 
	uint32* p_WindowWidth, uint32* p_WindowHeight, 
	K15_GUILayoutType p_LayoutType, const char* p_Identifier)
{
	K15_ASSERT(p_GUIContext);
	K15_ASSERT(p_Caption);
	K15_ASSERT(p_LeftPixelPos);
	K15_ASSERT(p_TopPixelPos);
	K15_ASSERT(p_WindowWidth);
	K15_ASSERT(p_WindowHeight);
	K15_ASSERT(p_Identifier);

	uint32 titleLength = (uint32)strlen(p_Caption);

	K15_GUIWindow* windowElement = 0;
	K15_GUIElementHeader* windowElementHeader = K15_InternalAddGUIElement(p_GUIContext,
		K15_GUI_TYPE_WINDOW, sizeof(K15_Window) + titleLength, p_Identifier, (void**)&windowElement);

	K15_GUIElementHeader* elementLastFrame = K15_InternalGetGUIElementLastFrame(p_GUIContext,
		windowElementHeader);

	K15_GUIWindowState lastWindowState = K15_GUI_WINDOW_STATE_NORMAL;
	K15_GUIWindowState currentWindowState = K15_GUI_WINDOW_STATE_NORMAL;

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	int32 dragPixelOffsetX = 0;
	int32 dragPixelOffsetY = 0;

	if (elementLastFrame)
	{
		K15_GUIWindow* windowLastFrame = (K15_GUIWindow*)(elementLastFrame + 1);
		*p_WindowHeight = windowLastFrame->windowPixelHeight;
		*p_WindowWidth = windowLastFrame->windowPixelWidth;
		*p_LeftPixelPos = elementLastFrame->posPixelX;
		*p_TopPixelPos = elementLastFrame->posPixelY;

		dragPixelOffsetX = windowLastFrame->dragPixelOffsetX;
		dragPixelOffsetY = windowLastFrame->dragPixelOffsetY;

		lastWindowState = windowLastFrame->state;
		currentWindowState = lastWindowState;
	}

	K15_GUIContextStyle* style = &p_GUIContext->style;
	K15_RenderFontDesc* guiFont = style->styleFont;
	uint32 windowTitleHeight = style->windowTitlePixelHeight;
	uint32 textOffsetInBytes = windowElementHeader->offset + sizeof(K15_GUIWindow) + sizeof(K15_GUIElementHeader);

	float windowTextPixelWidth = 0.f;
	K15_GetTextSizeInPixels(guiFont, &windowTextPixelWidth, 0, p_Caption, titleLength);

	*p_WindowWidth = K15_MAX(*p_WindowWidth, windowTextPixelWidth);

	//check mouse drag
	uint32 mousePixelPosX = p_GUIContext->mousePosPixelX;
	uint32 mousePixelPosY = p_GUIContext->mousePosPixelY;

	bool8 leftMouseDown = p_GUIContext->leftMouseDown;

	if (windowElementHeader->activated &&
		lastWindowState != K15_GUI_WINDOW_STATE_DRAGGED)
	{
		currentWindowState = K15_GUI_WINDOW_STATE_DRAGGED;
		dragPixelOffsetX = mousePixelPosX - *p_LeftPixelPos;
		dragPixelOffsetY = mousePixelPosY - *p_TopPixelPos;
	}
	else if (leftMouseDown &&
		lastWindowState == K15_GUI_WINDOW_STATE_DRAGGED)
	{
		*p_LeftPixelPos = p_GUIContext->mousePosPixelX - dragPixelOffsetX;
		*p_TopPixelPos = p_GUIContext->mousePosPixelY - dragPixelOffsetY;
	}
	else if (!leftMouseDown &&
		lastWindowState == K15_GUI_WINDOW_STATE_DRAGGED)
	{
		p_GUIContext->activeElementIdentifier = 0;
		currentWindowState = K15_GUI_WINDOW_STATE_NORMAL;
		dragPixelOffsetX = 0;
		dragPixelOffsetY = 0;
	}

	windowElementHeader->pixelWidth = *p_WindowWidth;
	windowElementHeader->pixelHeight = windowTitleHeight;
	windowElementHeader->posPixelX = *p_LeftPixelPos;
	windowElementHeader->posPixelY = *p_TopPixelPos;

	windowElement->state = currentWindowState;
	windowElement->titleLength = titleLength;
	windowElement->titleOffsetInBytes = textOffsetInBytes;
	windowElement->windowPixelHeight = *p_WindowHeight;
	windowElement->windowPixelWidth = *p_WindowWidth;
	windowElement->dragPixelOffsetX = dragPixelOffsetX;
	windowElement->dragPixelOffsetY = dragPixelOffsetY;

	bool8 windowActive = currentWindowState != K15_GUI_WINDOW_STATE_HIDDEN;

	if (windowActive)
	{
		K15_ASSERT_TEXT(!p_GUIContext->currentWindow,
			"There's already a window being currently created. "
			"Finish the window creation with K15_EndWindow() first, "
			"before starting a new window with K15_BeginWindow()");

		uint32 pixelPadding = p_GUIContext->style.windowContentPixelPadding;

		p_GUIContext->currentWindow = windowElement;

		K15_InternalPushVerticalLayout(p_GUIContext, 2, *p_LeftPixelPos + pixelPadding, 
			*p_TopPixelPos + windowTitleHeight + pixelPadding,
			*p_WindowWidth - pixelPadding * 2,
			*p_WindowHeight - pixelPadding * 3);
	}

	//copy text to gui memory buffer
	memcpy(guiContextFrontBuffer + textOffsetInBytes, p_Caption, titleLength);

	return windowActive;
}
/*********************************************************************************/
void K15_EndWindow(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext);

	K15_ASSERT_TEXT(p_GUIContext->currentWindow,
		"There's currently no window being created. "
		"Call K15_BeginWindow() to create a new window.");

	K15_PopLayout(p_GUIContext);

	p_GUIContext->currentWindow = 0;
}

/*********************************************************************************/
bool8 K15_Button(K15_GUIContext* p_GUIContext, const char* p_Caption, const char* p_Identifier)
{
	K15_ASSERT(p_GUIContext);

	uint32 captionLength = (uint32)strlen(p_Caption);
	uint32 elementSizeInBytes = sizeof(K15_GUIButton) + captionLength;
	K15_GUIButton* button = 0;

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, p_Caption, captionLength);

	uint32 minElementPixelWidth = textWidth + p_GUIContext->style.buttonContentPixelPadding * 2;
	uint32 minElementPixelHeigt = textHeight + p_GUIContext->style.buttonContentPixelPadding * 2;

	K15_GUIElementHeader* buttonElementHeader = K15_InternalAddGUIElement(p_GUIContext, 
		K15_GUI_TYPE_BUTTON, elementSizeInBytes, p_Identifier, (void**)&button, 
		minElementPixelWidth, minElementPixelHeigt);

	uint32 textOffsetInBytes =  buttonElementHeader->offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIButton);
	K15_GUIElementHeader* buttonHeaderLastFrame = K15_InternalGetGUIElementLastFrame(p_GUIContext, 
		buttonElementHeader);

	bool8 pressedLastFrame = K15_FALSE;
	bool8 hoveredLastFrame = K15_FALSE;
	bool8 pressed = K15_FALSE;

	if (buttonHeaderLastFrame)
	{
		pressedLastFrame = buttonHeaderLastFrame->activated;
		hoveredLastFrame = buttonHeaderLastFrame->hovered;
	}

	button->state = hoveredLastFrame ? K15_GUI_BUTTON_STATE_HOVERED : K15_GUI_BUTTON_STATE_NORMAL;
	button->textLength = captionLength;
	button->textOffsetInBytes = textOffsetInBytes;
	button->textPixelWidth = textWidth;
	button->textPixelHeight = textHeight;

	if (buttonElementHeader->hovered)
	{
		button->state = K15_GUI_BUTTON_STATE_HOVERED;
	}
	else if (!buttonElementHeader->hovered &&
		hoveredLastFrame)
	{
		button->state = K15_GUI_BUTTON_STATE_NORMAL;
	}

	if (buttonElementHeader->activated)
	{
		button->state = K15_GUI_BUTTON_STATE_PRESSED;
	}

	if (pressedLastFrame)
	{
		pressed = (buttonElementHeader->activated);
		
		if (p_GUIContext->leftMouseDown)
		{
			button->state = buttonElementHeader->hovered ? 
				K15_GUI_BUTTON_STATE_PRESSED : K15_GUI_BUTTON_STATE_NORMAL;
		}
	}

	if (pressedLastFrame && !pressed)
	{
		button->state = buttonElementHeader->hovered ? 
			K15_GUI_BUTTON_STATE_HOVERED : K15_GUI_BUTTON_STATE_NORMAL;
	}

	//copy text to gui memory buffer
	memcpy(p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER] + textOffsetInBytes,
		p_Caption, captionLength);

	return pressed;
}
/*********************************************************************************/
void K15_Label(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier)
{
	K15_ASSERT(p_GUIContext);
	K15_ASSERT(p_LabelText);
	K15_ASSERT(p_Identifier);

	uint32 textLength = (uint32)strlen(p_LabelText);
	uint32 elementSizeInBytes = sizeof(K15_GUILabel) + textLength;

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;
	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, p_LabelText, textLength);

	K15_GUILabel* label = 0;
	K15_GUIElementHeader* labelHeader = K15_InternalAddGUIElement(p_GUIContext, K15_GUI_TYPE_LABEL, elementSizeInBytes,
		p_Identifier, (void**)&label, textWidth, textHeight);

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	uint32 textOffsetInBytes = labelHeader->offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUILabel);

	label->textLength = textLength;
	label->textOffsetInBytes = textOffsetInBytes;
	label->textPixelHeight = textHeight;
	label->textPixelWidth = textWidth;

	//copy text to gui memory buffer
	memcpy(guiContextFrontBuffer + textOffsetInBytes, p_LabelText, textLength);
}
/*********************************************************************************/
float K15_FloatSlider(K15_GUIContext* p_GUIContext, float p_Value, float p_MinValue, float p_MaxValue, 
	const char* p_Identifier)
{
	K15_ASSERT(p_GUIContext);
	
	float minValue = p_MinValue;
	float maxValue = p_MaxValue;

	minValue = K15_MIN(p_MinValue, p_MaxValue);
	maxValue = K15_MAX(p_MinValue, p_MaxValue);

	float currentValue = K15_CLAMP(p_Value, maxValue, minValue);

	K15_GUIContextStyle* style = &p_GUIContext->style;

	uint32 elementSizeInBytes = sizeof(K15_GUIFloatSlider);
	K15_GUIFloatSlider* slider = 0;
	K15_GUIElementHeader* sliderHeader = K15_InternalAddGUIElement(p_GUIContext, 
		K15_GUI_TYPE_FLOAT_SLIDER, elementSizeInBytes, p_Identifier, (void**)&slider,
		style->sliderLinePixelWidth, style->sliderPixelHeight);

	K15_GUIElementHeader* sliderLastFrameHeader = K15_InternalGetGUIElementLastFrame(p_GUIContext, sliderHeader);

	float sliderRange = (maxValue - minValue);
	float valuePercent = (currentValue - minValue) / sliderRange;

	uint32 sliderLinePixelWidth = style->sliderLinePixelWidth;
	uint32 sliderPixelWidth = style->sliderPixelWidth;
	uint32 sliderPixelHeight = style->sliderPixelHeight;
	uint32 sliderPixelOffsetX = (float)sliderHeader->pixelWidth * valuePercent;
	uint32 sliderPixelOffsetY = sliderHeader->pixelHeight / 2;
	
	uint32 sliderPixelLeftPos = sliderHeader->posPixelX +
		((float)sliderHeader->pixelWidth * valuePercent) - sliderPixelWidth / 2;
	uint32 sliderPixelRightPos = sliderPixelLeftPos + sliderPixelWidth;
	uint32 sliderPixelTopPos = sliderHeader->posPixelY + sliderPixelOffsetY - sliderPixelHeight / 2;
	uint32 sliderPixelBottomPos = sliderPixelTopPos + sliderPixelHeight;

	if (sliderLastFrameHeader &&
		!sliderLastFrameHeader->activated &&
		sliderHeader->activated)
	{
		slider->mousePosPixelX = p_GUIContext->mousePosPixelX;
	}

	if (sliderHeader->activated)
	{
		uint32 offsetX = 0;
		if (slider->mousePosPixelX >= sliderHeader->posPixelX)
		{
			offsetX = (slider->mousePosPixelX - sliderHeader->posPixelX);
		}

		slider->mousePosPixelX = p_GUIContext->mousePosPixelX;

		offsetX = K15_CLAMP(offsetX, sliderHeader->pixelWidth, 0);

		valuePercent = (float)offsetX / (float)sliderHeader->pixelWidth;
		currentValue = K15_Lerp(minValue, maxValue, valuePercent);
	}

	slider->maxValue = maxValue;
	slider->minValue = minValue;
	slider->value = currentValue;
	slider->sliderPixelPosLeft = sliderPixelLeftPos;
	slider->sliderPixelPosRight = sliderPixelRightPos;
	slider->sliderPixelPosTop = sliderPixelTopPos;
	slider->sliderPixelPosBottom = sliderPixelBottomPos;

	return currentValue;
}
/*********************************************************************************/
void K15_PushVerticalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding, uint32 p_PixelWidth)
{
	int32 leftPixelPos = 0;
	int32 topPixelPos = 0;
	uint32 pixelWidth = p_PixelWidth;
	uint32 pixelHeight = 0;

	K15_InternalGetAlignedGUIDimension(p_GUIContext, &leftPixelPos, &topPixelPos, 
		&pixelWidth, &pixelHeight);

	K15_InternalPushVerticalLayout(p_GUIContext, p_ElementPadding,
		leftPixelPos, topPixelPos, pixelWidth, pixelHeight);
}
/*********************************************************************************/
void K15_PushHorizontalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding, uint32 p_PixelHeight)
{
	int32 leftPixelPos = 0;
	int32 topPixelPos = 0;
	uint32 pixelWidth = 0;
	uint32 pixelHeight = p_PixelHeight;

	K15_InternalGetAlignedGUIDimension(p_GUIContext, &leftPixelPos, &topPixelPos, &pixelWidth, &pixelHeight);

	K15_InternalPushHorizontalLayout(p_GUIContext, p_ElementPadding,
		leftPixelPos, topPixelPos, pixelWidth, pixelHeight);
}
/*********************************************************************************/
void K15_PopLayout(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext);
	uint32 layoutIndex = p_GUIContext->layoutIndex;

	K15_ASSERT(layoutIndex != 0);

	p_GUIContext->layoutIndex = layoutIndex - 1;
}
/*********************************************************************************/
void K15_IterateGUIElementsHelper(K15_GUIContext* p_GUIContext, K15_GUIIterator p_GUIIterator, void* p_UserData,
	uint32 p_BufferIndex)
{
	uint32 currentGUIMemoryOffset = 0;
	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize[p_BufferIndex];
	byte* guiMemory = p_GUIContext->guiMemory[p_BufferIndex];

	while (currentGUIMemoryOffset < guiMemorySize)
	{
		K15_GUIElementHeader* guiElement = (K15_GUIElementHeader*)(guiMemory + currentGUIMemoryOffset);
		p_GUIIterator(p_GUIContext, guiElement, p_UserData);

		currentGUIMemoryOffset = guiElement->offset + guiElement->elementSizeInBytes;
	}
}
/*********************************************************************************/