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

	K15_ResourceHandle styleFontResource = K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "fonts/gui_font.ttf", 0); 

	defaultStyle.windowTitleHeight = 20;

	defaultStyle.controlUpperBackgroundColor = 0x303030;
	defaultStyle.controlLowerBackgroundColor = 0x505050;
	defaultStyle.controlUpperBorderColor = 0x606060;
	defaultStyle.controlLowerBorderColor = 0x101010;
	defaultStyle.hoveredControlUpperBackgroundColor = 0x476e72;
	defaultStyle.hoveredControlLowerBackgroundColor = 0x488085; 
	defaultStyle.interactedControlLowerBackgroundColor = 0x476e72;
	defaultStyle.interactedControlUpperBackgroundColor = 0x3a5357;
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

		currentMemoryOffset += guiElementHeader->offset;
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

		currentMemoryOffset += guiElementHeader->offset;

		if (guiElementHeader->offset == 0)
		{
			break;
		}
	}

	return foundDuplicate;
}
#endif //K15_GUI_CONTEXT_CHECK_FOR_DUPLICATE_IDENTIFIERS
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
	guiContext->leftMouseDown = K15_FALSE;
	guiContext->rightMouseDown = K15_FALSE;
	guiContext->memoryLock = K15_CreateSemaphoreWithInitialValue(1);

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
	uint32* p_WindowWidth, uint32* p_WindowHeight, const char* p_Identifier)
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

	if (elementLastFrame)
	{
		K15_GUIWindow* windowLastFrame = (K15_GUIWindow*)(elementLastFrame + 1);
		*p_WindowHeight = windowLastFrame->windowPixelHeight;
		*p_WindowWidth = windowLastFrame->windowPixelWidth;
		*p_LeftPixelPos = elementLastFrame->posPixelX;
		*p_TopPixelPos = elementLastFrame->posPixelY;

		lastWindowState = windowLastFrame->state;
	}

	K15_GUIContextStyle* style = &p_GUIContext->style;
	K15_RenderFontDesc* guiFont = style->styleFont;
	uint32 windowTitleHeight = style->windowTitleHeight;

	float windowTextPixelWidth = 0.f;
	K15_GetTextSizeInPixels(guiFont, &windowTextPixelWidth, 0, p_Caption, titleLength);

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];
	K15_GUIElementHeader* windowElementHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUIWindow* window = (K15_GUIWindow*)
		(guiContextFrontBuffer + offset + sizeof(K15_GUIElementHeader));

	*p_WindowWidth = K15_MAX(*p_WindowWidth, windowTextPixelWidth);

	//check mouse drag
	uint32 mousePixelPosX = p_GUIContext->mousePosPixelX;
	uint32 mousePixelPosY = p_GUIContext->mousePosPixelY;

	bool8 mouseInside = K15_Collision2DBoxPoint(*p_LeftPixelPos, *p_TopPixelPos,
		*p_LeftPixelPos + *p_WindowWidth, *p_TopPixelPos + windowTitleHeight,
		mousePixelPosX, mousePixelPosY);

	bool8 leftMouseDown = p_GUIContext->leftMouseDown;

	if (leftMouseDown &&
		mouseInside &&
		lastWindowState != K15_GUI_WINDOW_STATE_DRAGGED)
	{
		currentWindowState = K15_GUI_WINDOW_STATE_DRAGGED;
		window->dragPixelOffsetX = mousePixelPosX - *p_LeftPixelPos;
		window->dragPixelOffsetY = mousePixelPosY - *p_TopPixelPos;
	}
	else if (leftMouseDown &&
		lastWindowState == K15_GUI_WINDOW_STATE_DRAGGED)
	{
		*p_LeftPixelPos = p_GUIContext->mousePosPixelX - window->dragPixelOffsetX;
		*p_TopPixelPos = p_GUIContext->mousePosPixelY - window->dragPixelOffsetY;
	}
	else if (!leftMouseDown &&
		lastWindowState == K15_GUI_WINDOW_STATE_DRAGGED)
	{
		currentWindowState = K15_GUI_WINDOW_STATE_NORMAL;
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

	if (currentWindowState == K15_GUI_WINDOW_STATE_NORMAL)
	{
		K15_ASSERT_TEXT(!p_GUIContext->currentWindow,
			"There's already a window being currently created. "
			"Finish the window creation with K15_EndWindow() first, "
			"before starting a new window with K15_BeginWindow()");

		p_GUIContext->currentWindow = window;
	}

	//copy text to gui memory buffer
	memcpy(guiContextFrontBuffer + textOffset, p_Caption, titleLength);

	return currentWindowState == K15_GUI_WINDOW_STATE_NORMAL;
}
/*********************************************************************************/
void K15_EndWindow(K15_GUIContext* p_GUIContext)
{
	K15_ASSERT(p_GUIContext);

	K15_ASSERT_TEXT(p_GUIContext->currentWindow,
		"There's currently no window being created. "
		"Call K15_BeginWindow() to create a new window.");

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
	K15_GUIElementHeader* buttonHeaderLastFrame = K15_InternalGetGUIElementLastFrame(p_GUIContext, guiElementIdentifierHash);

	if (buttonHeaderLastFrame)
	{
		K15_GUIButton* buttonLastFrame = (K15_GUIButton*)(++buttonHeaderLastFrame);
		pressedLastFrame = (buttonLastFrame->state == K15_GUI_BUTTON_STATE_PRESSED);
	}

	byte* guiContextFrontBuffer = p_GUIContext->guiMemory[K15_GUI_MEMORY_FRONT_BUFFER];

	K15_GUIElementHeader* buttonHeader = (K15_GUIElementHeader*)(guiContextFrontBuffer + offset);
	K15_GUIButton* button = (K15_GUIButton*)(guiContextFrontBuffer+ offset + sizeof(K15_GUIElementHeader));

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textWidth, &textHeight, p_Caption, captionLength);

	button->state = K15_GUI_BUTTON_STATE_NORMAL;
	button->textLength = captionLength;
	button->textOffsetInBytes = textOffsetInBytes;

	buttonHeader->type = K15_GUI_TYPE_BUTTON;
	buttonHeader->posPixelX = 0;
	buttonHeader->posPixelY = 400;
	buttonHeader->pixelWidth = textWidth;
	buttonHeader->pixelHeight = textHeight;
	buttonHeader->offset = newOffset;
	buttonHeader->identifierHash = K15_GenerateStringHash(p_Identifier);

	bool8 mouseInside = K15_Collision2DBoxPoint(buttonHeader->posPixelX, buttonHeader->posPixelY, 
		buttonHeader->posPixelX + buttonHeader->pixelWidth + 20,
		buttonHeader->posPixelY + buttonHeader->pixelHeight + 10,
		p_GUIContext->mousePosPixelX, p_GUIContext->mousePosPixelY);

	if (p_GUIContext->leftMouseDown && 
		mouseInside)
	{
		button->state = K15_GUI_BUTTON_STATE_PRESSED;
	}
	else if (mouseInside)
	{
		button->state = K15_GUI_BUTTON_STATE_HOVERED;
	}

	if (pressedLastFrame)
	{
		pressed = (!p_GUIContext->leftMouseDown && mouseInside);
		
		if (p_GUIContext->leftMouseDown)
		{
			button->state = mouseInside ? K15_GUI_BUTTON_STATE_PRESSED : K15_GUI_BUTTON_STATE_NORMAL;
		}
	}

	//copy text to gui memory buffer
	memcpy(guiContextFrontBuffer + textOffsetInBytes, p_Caption, captionLength);

	return pressed;
}
/*********************************************************************************/
