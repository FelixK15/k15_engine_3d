#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

#include "K15_GUILayerPrerequisites.h"

enum K15_GUIElementType
{
	K15_GUI_TYPE_INVALID = 0,
	K15_GUI_TYPE_BUTTON
};

struct K15_GUIElementHeader
{
	K15_GUIElementType type;

	real32 positionX;
	real32 positionY;
	real32 width;
	real32 height;

	uint32 offset;
};

struct K15_GUIContext
{
	K15_CustomMemoryAllocator* memoryAllocator;
	K15_RenderCommandBuffer* guiRenderCommandBuffer;
	K15_RenderTextureHandle guiTemplateTexture;
	byte* guiMemory;

	uint32 guiMemoryMaxSize;
	uint32 guiMemoryCurrentSize;

	real32 mousePosX;
	real32 mousePosY;

	uint32 windowHeight;
	uint32 windowWidth;
};

K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext);
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext);

void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight);
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY);

bool8 K15_Button(K15_GUIContext* p_GUIContext, float p_PositionX, float p_PositionY, const char* p_Caption);

#endif //_K15_GUILayer_Context_h_