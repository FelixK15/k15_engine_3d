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

	defaultStyle.windowContentPixelPadding = 5;

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
	defaultStyle.windowUpperBackgroundColor = 0x303030;
	defaultStyle.windowLowerBackgroundColor = 0x505050;
	defaultStyle.windowBorderUpperColor = 0x050545;
	defaultStyle.windowBorderLowerColor = 0x000000;
	defaultStyle.styleFont = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);

	return defaultStyle;
}
/*********************************************************************************/
intern K15_GUIElementHeader* K15_InternalGetGUIElementLastFrame(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash)
{
	K15_GUIElementHeader* elementLastFrame = 0;

	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];
	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_BACK_BUFFER];
	uint32 currentMemoryOffset = 0;

	while (currentMemoryOffset < guiMemorySize)
	{
		K15_GUIElementHeader* guiElementHeader = (K15_GUIElementHeader*)(guiMemory + currentMemoryOffset);

		if (p_IdentifierHash == guiElementHeader->identifierHash)
		{
			elementLastFrame = guiElementHeader;
			break;
		}

		currentMemoryOffset = guiElementHeader->offset;
	}

	return elementLastFrame;
}
/*********************************************************************************/
#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
intern bool8 K15_InternalCheckForDuplicateIdentifiers(K15_GUIContext* p_GUIContext, uint32 p_IdentifierHash)
{
	bool8 foundDuplicate = K15_FALSE;

	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 currentMemoryOffset = 0;

	while (currentMemoryOffset < guiMemorySize)
	{
		K15_GUIElementHeader* guiElementHeader = (K15_GUIElementHeader*)(guiMemory + currentMemoryOffset);

		if (p_IdentifierHash == guiElementHeader->identifierHash)
		{
			foundDuplicate = K15_TRUE;
			break;
		}

		currentMemoryOffset = guiElementHeader->offset;
	}

	return foundDuplicate;
}
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
/*********************************************************************************/
intern void K15_InternalGetAlignedGUIDimension_R(K15_GUIContext* p_GUIContext, uint32 p_StackIndex,
	int32* p_LeftPixelPos, int32* p_TopPixelPos, uint32* p_PixelWidth, uint32* p_PixelHeight)
{
	K15_GUILayoutCategory* category = &p_GUIContext->layoutCategoryStack[p_StackIndex];
	K15_GUILayout layout = category->layout;

	uint32 pixelOffsetX = category->pixelOffsetX;
	uint32 pixelOffsetY = category->pixelOffsetY;
	uint32 elementPixelPadding = category->elementPixelPadding;

	uint32 leftPixelPos = p_LeftPixelPos ? *p_LeftPixelPos : 0;
	uint32 topPixelPos = p_TopPixelPos ? *p_TopPixelPos : 0;
	uint32 pixelWidth = p_PixelWidth ? *p_PixelWidth : 0;
	uint32 pixelHeight = p_PixelHeight ? *p_PixelHeight : 0;

	switch (layout)
	{
		case K15_GUI_LAYOUT_VERTICAL:
		{
			K15_GUIVerticalLayoutParameter* param = (K15_GUIVerticalLayoutParameter*)category->param;
			leftPixelPos = category->posPixelX + pixelOffsetX;
			topPixelPos = category->posPixelY + pixelOffsetY + elementPixelPadding;
			pixelWidth = K15_CLAMP(category->pixelWidth, UINT_MAX, param->minElementHeight);
			pixelOffsetY += pixelHeight + elementPixelPadding*2;

			break;
		}

		case K15_GUI_LAYOUT_HORIZONTAL:
		{
			K15_GUIHorizontalLayoutParameter* param = (K15_GUIHorizontalLayoutParameter*)category->param;
			leftPixelPos = category->posPixelX + pixelOffsetX + elementPixelPadding;
			topPixelPos = category->posPixelY + pixelOffsetY;
			pixelHeight = K15_CLAMP(category->pixelHeight, UINT_MAX, param->minElementWidth);
			pixelOffsetX += pixelWidth + elementPixelPadding*2;

			break;
		}
	}

	category->pixelOffsetX = pixelOffsetX;
	category->pixelOffsetY = pixelOffsetY;

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

	if (p_StackIndex != 0)
	{
		K15_InternalGetAlignedGUIDimension_R(p_GUIContext, p_StackIndex - 1, 
			p_LeftPixelPos, p_TopPixelPos, p_PixelWidth, p_PixelHeight);
	}
}
/*********************************************************************************/
intern void K15_InternalGetAlignedGUIDimension(K15_GUIContext* p_GUIContext,
	int32* p_LeftPixelPos, int32* p_TopPixelPos, uint32* p_PixelWidth, uint32* p_PixelHeight)
{
	uint32 categoryIndex = p_GUIContext->layoutCategoryIndex;

	if (categoryIndex != 0)
	{
		K15_InternalGetAlignedGUIDimension_R(p_GUIContext, categoryIndex - 1,
			p_LeftPixelPos, p_TopPixelPos, p_PixelWidth, p_PixelHeight);
	}
}
/*********************************************************************************/
intern K15_GUILayoutCategory* K15_InternalPushLayoutCategory(K15_GUIContext* p_GUIContext, K15_GUILayout p_Layout,
	uint32 p_ElementPixelPadding, int32 p_LeftPixelPos, int32 p_TopPixelPos, uint32 p_PixelWidth, uint32 p_PixelHeight)
{
	K15_ASSERT(p_GUIContext);

	uint32 categoryIndex = p_GUIContext->layoutCategoryIndex;

	K15_ASSERT(categoryIndex != K15_GUI_MAX_CATEGORIES);

	K15_GUILayoutCategory* category = &p_GUIContext->layoutCategoryStack[categoryIndex];
	category->layout = p_Layout;
	category->posPixelX = p_LeftPixelPos;
	category->posPixelY = p_TopPixelPos;
	category->pixelWidth = p_PixelWidth;
	category->pixelHeight = p_PixelHeight;
	category->pixelOffsetX = p_ElementPixelPadding;
	category->pixelOffsetY = p_ElementPixelPadding;
	category->elementPixelPadding = p_ElementPixelPadding;

	p_GUIContext->layoutCategoryIndex = categoryIndex + 1;

	return category;
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
	guiContext->layoutCategoryIndex = 0;
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
	assert(p_GUIContext);

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

//	memset(p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER], 0, frontBufferMemorySize);
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
char* K15_ComboBox(K15_GUIContext* p_GUIContext, char** p_Elements, uint32 p_NumElements, const char* p_Identifier)
{
	assert(p_GUIContext);

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
}
/*********************************************************************************/
bool8 K15_BeginWindow(K15_GUIContext* p_GUIContext, const char* p_Caption, 
	int32* p_LeftPixelPos, int32* p_TopPixelPos, 
	uint32* p_WindowWidth, uint32* p_WindowHeight, 
	K15_GUILayout p_Layout, const char* p_Identifier)
{
	K15_ASSERT(p_GUIContext);
	K15_ASSERT(p_Caption);
	K15_ASSERT(p_LeftPixelPos);
	K15_ASSERT(p_TopPixelPos);
	K15_ASSERT(p_WindowWidth);
	K15_ASSERT(p_WindowHeight);
	K15_ASSERT(p_Identifier);

	uint32 guiElementIdentifierHash = K15_GenerateStringHash(p_Identifier);
	uint32 offset = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 titleLength = (uint32)strlen(p_Caption);
	uint32 textOffset = offset + sizeof(K15_GUIWindow) + sizeof(K15_GUIElementHeader);
	uint32 newOffset = textOffset + titleLength;

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
	K15_ASSERT_TEXT(K15_InternalCheckForDuplicateIdentifiers(p_GUIContext, guiElementIdentifierHash) == K15_FALSE,
		"Found duplicate for identifier '%s'. Please use a different identifier.", p_Identifier);
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = newOffset;

	K15_GUIElementHeader* elementLastFrame = K15_InternalGetGUIElementLastFrame(p_GUIContext,
		guiElementIdentifierHash);

	K15_GUIWindowState lastWindowState = K15_GUI_WINDOW_STATE_NORMAL;
	K15_GUIWindowState currentWindowState = K15_GUI_WINDOW_STATE_NORMAL;

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];
	K15_GUIElementHeader* windowElementHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUIWindow* window = (K15_GUIWindow*)
		(guiContextFrontBuffer + offset + sizeof(K15_GUIElementHeader));

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

	float windowTextPixelWidth = 0.f;
	K15_GetTextSizeInPixels(guiFont, &windowTextPixelWidth, 0, p_Caption, titleLength);

	*p_WindowWidth = K15_MAX(*p_WindowWidth, windowTextPixelWidth);

	//check mouse drag
	uint32 mousePixelPosX = p_GUIContext->mousePosPixelX;
	uint32 mousePixelPosY = p_GUIContext->mousePosPixelY;

	bool8 mouseInside = K15_Collision2DBoxPoint(*p_LeftPixelPos, *p_TopPixelPos,
		*p_LeftPixelPos + *p_WindowWidth, *p_TopPixelPos + windowTitleHeight,
		mousePixelPosX, mousePixelPosY);

	bool8 leftMouseDown = p_GUIContext->leftMouseDown;

	if (p_GUIContext->activeElementIdentifier == 0)
	{
		if (mouseInside &&
			p_GUIContext->hoveredElementIdentifier == 0)
		{
			p_GUIContext->hoveredElementIdentifier = guiElementIdentifierHash;
		}
		else if (!mouseInside &&
			p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash)
		{
			p_GUIContext->hoveredElementIdentifier = 0;
		}
	}

	if (leftMouseDown &&
		mouseInside &&
		lastWindowState != K15_GUI_WINDOW_STATE_DRAGGED &&
		p_GUIContext->activeElementIdentifier == 0 &&
		p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash)
	{
		currentWindowState = K15_GUI_WINDOW_STATE_DRAGGED;
		dragPixelOffsetX = mousePixelPosX - *p_LeftPixelPos;
		dragPixelOffsetY = mousePixelPosY - *p_TopPixelPos;
		p_GUIContext->activeElementIdentifier = guiElementIdentifierHash;
		p_GUIContext->hoveredElementIdentifier = guiElementIdentifierHash;
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

	windowElementHeader->identifierHash = guiElementIdentifierHash;
	windowElementHeader->offset = newOffset;
	windowElementHeader->pixelWidth = *p_WindowWidth;
	windowElementHeader->pixelHeight = windowTitleHeight;
	windowElementHeader->posPixelX = *p_LeftPixelPos;
	windowElementHeader->posPixelY = *p_TopPixelPos;
	windowElementHeader->type = K15_GUI_TYPE_WINDOW;

	window->state = currentWindowState;
	window->titleLength = titleLength;
	window->titleOffsetInBytes = textOffset;
	window->windowPixelHeight = *p_WindowHeight;
	window->windowPixelWidth = *p_WindowWidth;
	window->dragPixelOffsetX = dragPixelOffsetX;
	window->dragPixelOffsetY = dragPixelOffsetY;

	bool8 windowActive = currentWindowState != K15_GUI_WINDOW_STATE_HIDDEN;

	if (windowActive)
	{
		K15_ASSERT_TEXT(!p_GUIContext->currentWindow,
			"There's already a window being currently created. "
			"Finish the window creation with K15_EndWindow() first, "
			"before starting a new window with K15_BeginWindow()");

		uint32 pixelPadding = p_GUIContext->style.windowContentPixelPadding;

		p_GUIContext->currentWindow = window;

		K15_PushVerticalLayout(p_GUIContext, 5, 10, *p_LeftPixelPos, 
			*p_TopPixelPos + windowTitleHeight + pixelPadding,
			*p_WindowWidth - pixelPadding * 2,
			*p_WindowHeight - pixelPadding * 2);
	}

	//copy text to gui memory buffer
	memcpy(guiContextFrontBuffer + textOffset, p_Caption, titleLength);

	return windowActive;
}
/*********************************************************************************/
void K15_EndWindow(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext);

	K15_ASSERT_TEXT(p_GUIContext->currentWindow,
		"There's currently no window being created. "
		"Call K15_BeginWindow() to create a new window.");

	K15_PopLayoutCategory(p_GUIContext);

	p_GUIContext->currentWindow = 0;
}
/*********************************************************************************/
bool8 K15_Button(K15_GUIContext* p_GUIContext, const char* p_Caption, const char* p_Identifier)
{
	assert(p_GUIContext);

	uint32 captionLength = (uint32)strlen(p_Caption);
	uint32 offset = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 textOffsetInBytes =  offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIButton);
	uint32 newOffset = textOffsetInBytes + captionLength;
	uint32 guiElementIdentifierHash = K15_GenerateStringHash(p_Identifier);

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
 	K15_ASSERT_TEXT(K15_InternalCheckForDuplicateIdentifiers(p_GUIContext, guiElementIdentifierHash) == K15_FALSE,
		"Found duplicate for identifier '%s'. Please use a different identifier.", p_Identifier);
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = newOffset;

	bool8 pressed = K15_FALSE;
	bool8 pressedLastFrame = K15_FALSE;
	bool8 hoveredLastFrame = K15_FALSE;
	K15_GUIElementHeader* buttonHeaderLastFrame = K15_InternalGetGUIElementLastFrame(p_GUIContext, guiElementIdentifierHash);

	if (buttonHeaderLastFrame)
	{
		K15_GUIButton* buttonLastFrame = (K15_GUIButton*)(++buttonHeaderLastFrame);
		pressedLastFrame = (buttonLastFrame->state == K15_GUI_BUTTON_STATE_PRESSED);
		hoveredLastFrame = (buttonLastFrame->state == K15_GUI_BUTTON_STATE_HOVERED);
	}

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	K15_GUIElementHeader* buttonHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUIButton* button = (K15_GUIButton*)(guiContextFrontBuffer+ offset + sizeof(K15_GUIElementHeader));

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, p_Caption, captionLength);

	button->state = hoveredLastFrame ? K15_GUI_BUTTON_STATE_HOVERED : K15_GUI_BUTTON_STATE_NORMAL;
	button->textLength = captionLength;
	button->textOffsetInBytes = textOffsetInBytes;
	button->textPixelWidth = textWidth;
	button->textPixelHeight = textHeight;

	buttonHeader->type = K15_GUI_TYPE_BUTTON;
	buttonHeader->posPixelX = 0;
	buttonHeader->posPixelY = 0;
	buttonHeader->pixelWidth = textWidth;
	buttonHeader->pixelHeight = textHeight + 5;
	buttonHeader->offset = newOffset;
	buttonHeader->identifierHash = K15_GenerateStringHash(p_Identifier);

	K15_InternalGetAlignedGUIDimension(p_GUIContext, &buttonHeader->posPixelX, &buttonHeader->posPixelY,
		&buttonHeader->pixelWidth, &buttonHeader->pixelHeight);

	bool8 mouseInside = K15_Collision2DBoxPoint(buttonHeader->posPixelX, buttonHeader->posPixelY, 
		buttonHeader->posPixelX + buttonHeader->pixelWidth,
		buttonHeader->posPixelY + buttonHeader->pixelHeight,
		p_GUIContext->mousePosPixelX, p_GUIContext->mousePosPixelY);

	if (mouseInside &&
		p_GUIContext->hoveredElementIdentifier == 0)
	{
		p_GUIContext->hoveredElementIdentifier = guiElementIdentifierHash;
		button->state = K15_GUI_BUTTON_STATE_HOVERED;
	}
	else if (!mouseInside &&
		p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash)
	{
		p_GUIContext->hoveredElementIdentifier = 0;
		button->state = K15_GUI_BUTTON_STATE_NORMAL;
	}

	if (p_GUIContext->leftMouseDown && 
		p_GUIContext->activeElementIdentifier == 0 &&
		p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash &&
		mouseInside)
	{
		button->state = K15_GUI_BUTTON_STATE_PRESSED;
	}

	if (pressedLastFrame)
	{
		pressed = (p_GUIContext->leftMouseDown && mouseInside);
		
		if (p_GUIContext->leftMouseDown)
		{
			button->state = mouseInside ? K15_GUI_BUTTON_STATE_PRESSED : K15_GUI_BUTTON_STATE_NORMAL;
		}
	}

	if (pressed && !pressedLastFrame)
	{
		p_GUIContext->activeElementIdentifier = guiElementIdentifierHash;
	}
	else if (pressedLastFrame && !pressed)
	{
		button->state = mouseInside ? K15_GUI_BUTTON_STATE_HOVERED : K15_GUI_BUTTON_STATE_NORMAL;
		p_GUIContext->activeElementIdentifier = 0;
	}

	//copy text to gui memory buffer
	memcpy(guiContextFrontBuffer + textOffsetInBytes, p_Caption, captionLength);

	return pressed;
}
/*********************************************************************************/
void K15_Label(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier)
{
	assert(p_GUIContext);
	assert(p_LabelText);
	assert(p_Identifier);

	uint32 textLength = (uint32)strlen(p_LabelText);
	uint32 offset = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 textOffsetInBytes = offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUILabel);
	uint32 newOffset = textOffsetInBytes + textLength;
	uint32 guiElementIdentifierHash = K15_GenerateStringHash(p_Identifier);

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
	K15_ASSERT_TEXT(K15_InternalCheckForDuplicateIdentifiers(p_GUIContext, guiElementIdentifierHash) == K15_FALSE,
		"Found duplicate for identifier '%s'. Please use a different identifier.", p_Identifier);
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = newOffset;

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	K15_GUIElementHeader* labelHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUILabel* label = (K15_GUILabel*)(guiContextFrontBuffer + offset + sizeof(K15_GUIElementHeader));

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, p_LabelText, textLength);

	labelHeader->identifierHash = guiElementIdentifierHash;
	labelHeader->offset = newOffset;
	labelHeader->type = K15_GUI_TYPE_LABEL;
	labelHeader->pixelHeight = textHeight;
	labelHeader->pixelWidth = textWidth;
	labelHeader->posPixelX = 0;
	labelHeader->posPixelY = 0;

	label->textLength = textLength;
	label->textOffsetInBytes = textOffsetInBytes;
	label->textPixelHeight = textHeight;
	label->textPixelWidth = textWidth;

	K15_InternalGetAlignedGUIDimension(p_GUIContext, &labelHeader->posPixelX, &labelHeader->posPixelY,
		&labelHeader->pixelWidth, &labelHeader->pixelHeight);

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

	uint32 maxFloatTextLength = 20;

	uint32 offset = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER];
	uint32 textOffset = offset + sizeof(K15_GUIElementHeader) + sizeof(K15_GUIFloatSlider);
	uint32 newOffset = textOffset + maxFloatTextLength * 3;
	uint32 guiElementIdentifierHash = K15_GenerateStringHash(p_Identifier);

	K15_ASSERT_TEXT(newOffset <= p_GUIContext->guiMemoryMaxSize, "Out of GUI memory.");

#ifdef K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
	K15_ASSERT_TEXT(K15_InternalCheckForDuplicateIdentifiers(p_GUIContext, guiElementIdentifierHash) == K15_FALSE,
		"Found duplicate for identifier '%s'. Please use a different identifier.", p_Identifier);
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS

	p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_FRONT_BUFFER] = newOffset;

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	K15_GUIElementHeader* sliderLastFrameHeader = K15_InternalGetGUIElementLastFrame(p_GUIContext, guiElementIdentifierHash);

	if (sliderLastFrameHeader)
	{
		K15_GUIFloatSlider* sliderLastFrame = (K15_GUIFloatSlider*)(sliderLastFrameHeader + 1);
		currentValue = sliderLastFrame->value;
	}

	K15_GUIElementHeader* sliderHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUIFloatSlider* slider = (K15_GUIFloatSlider*)(guiContextFrontBuffer + offset + sizeof(K15_GUIElementHeader));

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;
	float textWidth = 0;
	float textHeight = 0;

	float totalTextPixelWidth = 0;
	float minValueTextPixelWidth = 0;
	float maxValueTextPixelWidth = 0;
	float curValueTextPixelWidth = 0;

	char* buffer = (char*)alloca(128);

	uint32 minValueTextLength = sprintf(buffer, "%.2f", p_MinValue);
	K15_GetTextSizeInPixels(guiFont, &minValueTextPixelWidth, &textHeight, buffer, minValueTextLength);
	totalTextPixelWidth += minValueTextPixelWidth;
	memcpy(guiContextFrontBuffer + textOffset, buffer, minValueTextLength);

	uint32 maxValueTextLength = sprintf(buffer, "%.2f", p_MaxValue);
	K15_GetTextSizeInPixels(guiFont, &maxValueTextPixelWidth, &textHeight, buffer, maxValueTextLength);
	totalTextPixelWidth += maxValueTextPixelWidth;
	memcpy(guiContextFrontBuffer + textOffset + minValueTextLength, buffer, maxValueTextLength);

	uint32 curValueTextLength = sprintf(buffer, "%.2f", currentValue);
	K15_GetTextSizeInPixels(guiFont, &curValueTextPixelWidth, &textHeight, buffer, curValueTextLength);
	memcpy(guiContextFrontBuffer + textOffset + minValueTextLength + maxValueTextLength, buffer, curValueTextLength);

	sliderHeader->identifierHash = guiElementIdentifierHash;
	sliderHeader->offset = newOffset;
	sliderHeader->type = K15_GUI_TYPE_FLOAT_SLIDER;
	sliderHeader->pixelHeight = textHeight + style->sliderPixelHeight;
	sliderHeader->pixelWidth = totalTextPixelWidth;

	K15_InternalGetAlignedGUIDimension(p_GUIContext, &sliderHeader->posPixelX, &sliderHeader->posPixelY, 
		&sliderHeader->pixelWidth, &sliderHeader->pixelHeight);

	uint32 sliderLinePixelWidth = style->sliderLinePixelWidth;
	uint32 sliderPixelWidth = style->sliderPixelWidth;
	uint32 sliderPixelHeight = style->sliderPixelHeight;
	uint32 sliderPixelOffsetX = (float)sliderHeader->pixelWidth * currentValue;
	uint32 sliderPixelOffsetY = sliderHeader->pixelHeight / 2 - sliderLinePixelWidth / 2;
	
	uint32 sliderPixelLeftPos = sliderHeader->posPixelX + sliderPixelOffsetX - sliderPixelWidth / 2;
	uint32 sliderPixelRightPos = sliderHeader->posPixelX + sliderPixelOffsetX + sliderPixelWidth / 2;
	uint32 sliderPixelTopPos = sliderHeader->posPixelY + sliderPixelOffsetY - sliderPixelHeight / 2;
	uint32 sliderPixelBottomPos = sliderHeader->posPixelY + sliderPixelOffsetY + sliderPixelHeight / 2;

	bool8 mouseInside = K15_Collision2DBoxPoint(sliderPixelLeftPos, sliderPixelTopPos,
		sliderPixelRightPos, sliderPixelBottomPos,
		p_GUIContext->mousePosPixelX, p_GUIContext->mousePosPixelY);

	if (mouseInside &&
		p_GUIContext->hoveredElementIdentifier == 0)
	{
		p_GUIContext->hoveredElementIdentifier = guiElementIdentifierHash;
	}
	else if (!mouseInside &&
		p_GUIContext->hoveredElementIdentifier == guiElementIdentifierHash)
	{
		p_GUIContext->hoveredElementIdentifier = 0;
	}

	if (p_GUIContext->activeElementIdentifier == 0 &&
		p_GUIContext->leftMouseDown &&
		mouseInside)
	{
		p_GUIContext->activeElementIdentifier = guiElementIdentifierHash;
		slider->mousePosPixelX = p_GUIContext->mousePosPixelX;
	}
	else if (!p_GUIContext->leftMouseDown &&
		p_GUIContext->activeElementIdentifier == guiElementIdentifierHash)
	{
		p_GUIContext->activeElementIdentifier = 0;
	}

	if (p_GUIContext->activeElementIdentifier == guiElementIdentifierHash)
	{
		uint32 offsetX = 0;
		if (slider->mousePosPixelX >= sliderHeader->posPixelX)
		{
			offsetX = (slider->mousePosPixelX - sliderHeader->posPixelX);
		}

		slider->mousePosPixelX = p_GUIContext->mousePosPixelX;

		offsetX = K15_CLAMP(offsetX, sliderHeader->pixelWidth, 0);
		currentValue = (float)offsetX / (float)sliderHeader->pixelWidth;
	}

	slider->maxValue = maxValue;
	slider->minValue = minValue;
	slider->value = currentValue;
	slider->minValueTextOffsetInBytes = textOffset;
	slider->maxValueTextOffsetInBytes = textOffset + minValueTextLength;
	slider->curValueTextOffsetInBytes = textOffset + minValueTextLength + maxValueTextLength;
	slider->minValueTextLength = minValueTextLength;
	slider->maxValueTextLenght = maxValueTextLength;
	slider->curValueTextLength = curValueTextLength;
	slider->sliderPixelPosLeft = sliderPixelLeftPos;
	slider->sliderPixelPosRight = sliderPixelRightPos;
	slider->sliderPixelPosTop = sliderPixelTopPos;
	slider->sliderPixelPosBottom = sliderPixelBottomPos;

	return currentValue;
}
/*********************************************************************************/
void K15_PushVerticalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding, uint32 p_MinElementPixelHeight, 
	int32 p_LeftPixelPos, int32 p_TopPixelPos, uint32 p_PixelWidth, uint32 p_PixelHeight)
{
	K15_InternalGetAlignedGUIDimension(p_GUIContext, &p_LeftPixelPos, &p_TopPixelPos, &p_PixelWidth, &p_PixelHeight);

	K15_GUILayoutCategory* verticalLayout = K15_InternalPushLayoutCategory(p_GUIContext, 
		K15_GUI_LAYOUT_VERTICAL, p_ElementPadding,
		p_LeftPixelPos, p_TopPixelPos, p_PixelWidth, p_PixelHeight);

	byte* paramBuffer = verticalLayout->param;

	K15_GUIVerticalLayoutParameter* verticalLayoutParam = (K15_GUIVerticalLayoutParameter*)paramBuffer;

	verticalLayoutParam->minElementHeight = p_MinElementPixelHeight;
}
/*********************************************************************************/
void K15_PushHorizontalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding, uint32 p_MinElementPixelWidth, 
	int32 p_LeftPixelPos, int32 p_TopPixelPos, uint32 p_PixelWidth, uint32 p_PixelHeight)
{
	K15_InternalGetAlignedGUIDimension(p_GUIContext, &p_LeftPixelPos, &p_TopPixelPos, &p_PixelWidth, &p_PixelHeight);

	K15_GUILayoutCategory* horizontalLayout = K15_InternalPushLayoutCategory(p_GUIContext,
		K15_GUI_LAYOUT_HORIZONTAL, p_ElementPadding,
		p_LeftPixelPos, p_TopPixelPos, p_PixelWidth, p_PixelHeight);

	byte* paramBuffer = horizontalLayout->param;

	K15_GUIHorizontalLayoutParameter* horizontalLayoutParam = (K15_GUIHorizontalLayoutParameter*)paramBuffer;

	horizontalLayoutParam->minElementWidth = p_MinElementPixelWidth;
}
/*********************************************************************************/
void K15_PopLayoutCategory(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext);
	uint32 categoryIndex = p_GUIContext->layoutCategoryIndex;

	K15_ASSERT(categoryIndex != 0);

	p_GUIContext->layoutCategoryIndex = categoryIndex - 1;
}
/*********************************************************************************/