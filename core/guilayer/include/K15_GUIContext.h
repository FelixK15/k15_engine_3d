#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

#include "K15_GUIContextPrerequisites.h"
#include "K15_CustomMemoryAllocator.h"

#include <K15_RenderMaterialDesc.h>

enum K15_GUIElementType
{
	K15_GUI_TYPE_INVALID = 0,
	K15_GUI_TYPE_BUTTON,
	K15_GUI_TYPE_COMBO_BOX
};

enum K15_GUIButtonState
{
	K15_GUI_BUTTON_STATE_NORMAL = 0,
	K15_GUI_BUTTON_STATE_PRESSED,
	K15_GUI_BUTTON_STATE_HOVERED,
	K15_GUI_BUTTON_STATE_DISABLED,

	K15_GUI_BUTTON_STATE_COUNT
};

enum K15_GUICategoryLayout
{
	K15_GUI_LAYOUT_HORIZONTAL = 0,
	K15_GUI_LAYOUT_VERTICAL,
	K15_GUI_LAYOUT_GRID
};

struct K15_GUIButton
{
	K15_GUIButtonState state;
	uint32 textOffsetInBytes;
	uint32 textLength;
};

struct K15_GUIComboBox
{
	bool8 expanded;
	uint32 elementsOffsetInBytes;
	uint32 numElements;
	uint32 selectedIndex;
};

struct K15_GUIElementHeader
{
	K15_GUIElementType type;

	uint32 posPixelX;
	uint32 posPixelY;
	uint32 pixelWidth;
	uint32 pixelHeight;
	uint32 identifierHash;
	uint32 offset;
};


struct K15_GUIContextStyle
{
	K15_RenderFontDesc* styleFont;

	uint32 textColor;
	uint32 disabledTextColor;
	uint32 textBackgroundColor;
	uint32 disabledTextBackgroundColor;

	//controls
	uint32 controlUpperBackgroundColor;
	uint32 controlLowerBackgroundColor;
	uint32 disabledControlUpperBackgroundColor;
	uint32 disabledControlLowerBackgroundColor;
	uint32 hoveredControlLowerBackgroundColor;
	uint32 hoveredControlUpperBackgroundColor;
	uint32 controlLowerBorderColor;
	uint32 controlUpperBorderColor;
};

struct K15_GUICategory
{
	uint32 posPixelX;
	uint32 posPixelY;
	K15_GUICategoryLayout layout;
};

struct K15_GUIContext
{
	K15_CustomMemoryAllocator memoryAllocator;

	K15_RenderMaterialDesc* guiRenderMaterial;

	K15_GUICategory categoryStack[K15_GUI_MAX_CATEGORIES];
	K15_GUIContextStyle style;

	K15_Semaphore* memoryLock;

	byte* guiMemory[K15_GUI_MEMORY_BUFFER_COUNT];
	
	bool8 leftMouseDown;
	bool8 rightMouseDown;

	uint32 categoryIndex;

	uint32 guiMemoryMaxSize;
	uint32 guiMemoryCurrentSize[K15_GUI_MEMORY_BUFFER_COUNT];

	uint32 mousePosPixelX;
	uint32 mousePosPixelY;

	uint32 virtualResolutionWidth;
	uint32 virtualResolutionHeight;

	uint32 windowWidth;
	uint32 windowHeight;
};

K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);

void K15_FlipGUIContextMemory(K15_GUIContext* p_GUIContext);

void K15_SetGUIContextVirtualResolution(K15_GUIContext* p_GUIContext, uint32 p_VirtualWidth, uint32 p_VirtualHeight);

void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight);
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY);

bool8 K15_Button(K15_GUIContext* p_GUIContext, const char* p_Caption, const char* p_Identifier);
char* K15_ComboBox(K15_GUIContext* p_GUIContext, char** p_Elements, uint32 p_NumElements, const char* p_Identifier);

#endif //_K15_GUILayer_Context_h_