#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

#include "K15_GUILayerPrerequisites.h"
#include "K15_CustomMemoryAllocator.h"

#include <K15_RenderMaterialDesc.h>

enum K15_GUIElementType
{
	K15_GUI_TYPE_INVALID = 0,
	K15_GUI_TYPE_BUTTON
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
	const char* text;
	uint32 textLength;
};

struct K15_GUIElementHeader
{
	K15_GUIElementType type;

	uint32 posPixelX;
	uint32 posPixelY;
	uint32 pixelWidth;
	uint32 pixelHeight;

	uint32 offset;
};

//TODO define K15_GUIContextStyle

struct K15_GUIContextStyle
{
	K15_RenderResourceHandle* styleTexture;
	K15_RenderFontDesc* styleFont;

	struct 
	{
		struct 
		{
			uint32 marginLeft;
			uint32 marginRight;
			uint32 marginTop;
			uint32 marginBottom;
			uint32 posPixelX;
			uint32 posPixelY;
			uint32 pixelWidth;
			uint32 pixelHeight;
		} stateStyle[K15_GUI_BUTTON_STATE_COUNT];

	} guiButtonStyle;
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

	byte* guiMemory;

	uint32 categoryIndex;

	uint32 guiMemoryMaxSize;
	uint32 guiMemoryCurrentSize;

	uint32 mousePosPixelX;
	uint32 mousePosPixelY;

	uint32 virtualResolutionWidth;
	uint32 virtualResolutionHeight;

	uint32 windowWidth;
	uint32 windowHeight;
};

K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);

void K15_ResetGUIContextMemory(K15_GUIContext* p_GUIContext);

void K15_SetGUIContextVirtualResolution(K15_GUIContext* p_GUIContext, uint32 p_VirtualWidth, uint32 p_VirtualHeight);

void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight);
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY);

bool8 K15_Button(K15_GUIContext* p_GUIContext, const char* p_Caption);

#endif //_K15_GUILayer_Context_h_