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
#include "K15_String.h"
#include "K15_Thread.h"

/*********************************************************************************/
intern inline K15_GUIContextStyle K15_InternalCreateDefaultStyle(K15_ResourceContext* p_ResourceContext)
{
	K15_GUIContextStyle defaultStyle = {};

	K15_ResourceHandle styleFontResource = K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "fonts/gui_font.ttf", 0); 

	defaultStyle.guiButtonStyle.edgePixelThickness = 1;
	defaultStyle.guiButtonStyle.roundEdges = K15_FALSE;

// 	defaultStyle.styleTexture = K15_GetResourceRenderHandle(p_ResourceContext, styleTextureResource);
// 	defaultStyle.styleFont = K15_GetResourceFontDesc(p_ResourceContext, styleFontResource);
// 
// 	//button style
// 	{
// 		//normal
// 		/*********************************************************************************/
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].posPixelX = 0;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].posPixelY = 0;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].pixelWidth = 29;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].pixelHeight = 20;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginLeft = 3;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginTop = 3;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginBottom = 3;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_NORMAL].marginRight = 3;
// 		//pressed
// 		/*********************************************************************************/
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].posPixelX = 0;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].posPixelY = 21;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].pixelWidth = 29;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].pixelHeight = 20;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginLeft = 3;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginTop = 3;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginBottom = 3;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_HOVERED].marginRight = 3;
// 		//hovered
// 		/*********************************************************************************/
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].posPixelX = 0;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].posPixelY = 42;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].pixelWidth = 29;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].pixelHeight = 20;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginLeft = 4;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginTop = 4;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginBottom = 5;
// 		defaultStyle.guiButtonStyle.stateStyle[K15_GUI_BUTTON_STATE_PRESSED].marginRight = 5;
// 		/*********************************************************************************/
// 		defaultStyle.guiButtonStyle.padding = 5;
// 	}
// 
// 	//combo box style
// 	{
// 		defaultStyle.guiComboBoxStyle.marginBottom = 3;
// 		defaultStyle.guiComboBoxStyle.marginTop = 3;
// 		defaultStyle.guiComboBoxStyle.marginLeft = 3;
// 		defaultStyle.guiComboBoxStyle.marginRight = 3;
// 		defaultStyle.guiComboBoxStyle.posPixelX = 30;
// 		defaultStyle.guiComboBoxStyle.posPixelY = 0;
// 		defaultStyle.guiComboBoxStyle.pixelWidth = 29;
// 		defaultStyle.guiComboBoxStyle.pixelHeight = 20;
// 		defaultStyle.guiComboBoxStyle.expanderPosPixelX = 60;
// 		defaultStyle.guiComboBoxStyle.expanderPosPixelY = 0;
// 		defaultStyle.guiComboBoxStyle.expanderPixelHeight = 14;
// 		defaultStyle.guiComboBoxStyle.expanderPixelWidth = 7;
// 	}
	
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
	guiContext->virtualResolutionHeight = 0;
	guiContext->virtualResolutionWidth = 0;
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
		buttonHeader->posPixelX + buttonHeader->pixelWidth,
		buttonHeader->posPixelY + buttonHeader->pixelHeight,
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

	//copy text incl 0 terminator
	memcpy(guiContextFrontBuffer + textOffsetInBytes, p_Caption, captionLength);

	return pressed;
}
/*********************************************************************************/
