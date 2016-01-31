#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

struct K15_SystemEvent;
struct K15_GUIElementHeader;
struct K15_GUIContext;

#include "K15_GUIContextPrerequisites.h"
#include "K15_CustomMemoryAllocator.h"

#include <K15_RenderMaterialDesc.h>

typedef void(*K15_GUIIterator)(K15_GUIContext*, K15_GUIElementHeader*, void*);

enum K15_GUIDockingAreaFlags
{
	K15_GUI_LEFT_DOCKING_AREA = 0x001,
	K15_GUI_RIGHT_DOCKING_AREA = 0x002,
	K15_GUI_TOP_DOCKING_AREA = 0x004,
	K15_GUI_BOTTOM_DOCKING_AREA = 0x008
};

struct K15_GUIWindowStyle
{
	uint32 borderUpperColor;
	uint32 borderLowerColor;
	uint32 upperBackgroundColor;
	uint32 lowerBackgroundColor;
	uint32 titleTextColor;
	uint32 titlePixelPadding;
	uint32 borderPixelThickness;
	K15_RenderFontDesc* font;
};

struct K15_GUIButtonStyle
{
	uint32 borderUpperColor;
	uint32 borderLowerColor;
	uint32 upperBackgroundColor;
	uint32 lowerBackgroundColor;
	uint32 textColor;
	uint32 borderPixelThickness;
	uint32 textPixelPadding;
	K15_RenderFontDesc* font;
};

struct K15_GUIContextStyle
{
	K15_GUIWindowStyle windowStyle;
	K15_GUIButtonStyle buttonStyle;
};

struct K15_GUIContext
{
	byte* memoryBuffer;
	K15_CustomMemoryAllocator memoryAllocator;
	K15_GUIContextStyle style;
	K15_GUILayout layoutStack[K15_GUI_MAX_LAYOUTS];
	uint32 numLayouts;
	uint32 memoryMaxSizeInBytes;
	uint32 memoryCurrentSizeInBytes;
	uint32 retainedDataOffsetInBytes;
	uint32 retainedDataSizeInBytes;
};

//how this should work:
// - Call gui logic (K15_GUIButton, K15_GUIBeginWindow, K15_GUIPushHorizontalLayout etc.)
// - Store elements internally in the gui context (just rects?) <-- headache
// - iterate over elements (layouting)
// - iterate over elements (input)
// - iterate over elements (rendering)
// - Call gui logic (next frame - retrieve results from last frame. Mainly results of the input)

K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);

void K15_GUIBeginDockingArea(K15_GUIContext* p_GUIContext, int32 p_PosX, int32 p_PosY,
	uint32 p_Width, uint32 p_Height, uint32 p_AllowedDockingAreasMask, const char* p_Identifier);

bool8 K15_GUIBeginWindow(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier);
bool8 K15_GUIBeginWindowEX(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier, K15_GUIWindowStyle* p_GUIWindowStyle);

bool8 K15_GUIButton(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier);
bool8 K15_GUIButtonEX(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier, K15_GUIButtonStyle* p_GUIButtonStyle);

void K15_GUIPushVerticalLayout(K15_GUIContext* p_GUIContext);
void K15_GUIPushHorizontalLayout(K15_GUIContext* p_GUIContext);

void K15_GUIEndWindow(K15_GUIContext* p_GUIContext);

void K15_GUIEndDockingArea(K15_GUIContext* p_GUIContext);



#endif //_K15_GUILayer_Context_h_