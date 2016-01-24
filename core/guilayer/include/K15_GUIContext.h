#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

struct K15_SystemEvent;
struct K15_GUIElementHeader;
struct K15_GUIContext;

#include "K15_GUIContextPrerequisites.h"
#include "K15_CustomMemoryAllocator.h"

#include <K15_RenderMaterialDesc.h>

typedef void(*K15_GUIIterator)(K15_GUIContext*, K15_GUIElementHeader*, void*);

enum K15_GUIElementType
{
	K15_GUI_TYPE_INVALID = 0,
	K15_GUI_TYPE_BUTTON,
	K15_GUI_TYPE_COMBO_BOX,
	K15_GUI_TYPE_LABEL,
	K15_GUI_TYPE_FLOAT_SLIDER,
	K15_GUI_TYPE_WINDOW
};

enum K15_GUIButtonState
{
	K15_GUI_BUTTON_STATE_NORMAL = 0,
	K15_GUI_BUTTON_STATE_PRESSED,
	K15_GUI_BUTTON_STATE_HOVERED,
	K15_GUI_BUTTON_STATE_DISABLED,

	K15_GUI_BUTTON_STATE_COUNT
};

enum K15_GUIWindowState
{
	K15_GUI_WINDOW_STATE_NORMAL = 0,
	K15_GUI_WINDOW_STATE_HIDDEN,
	K15_GUI_WINDOW_STATE_DRAGGED
};

enum K15_GUILayout
{
	K15_GUI_LAYOUT_HORIZONTAL = 0,
	K15_GUI_LAYOUT_VERTICAL
};

struct K15_GUIButton
{
	K15_GUIButtonState state;
	uint32 textOffsetInBytes;
	uint32 textLength;
	uint32 textPixelWidth;
	uint32 textPixelHeight;
};

struct K15_GUILabel
{
	uint32 textLength;
	uint32 textOffsetInBytes;
	uint32 textPixelWidth;
	uint32 textPixelHeight;
};

struct K15_GUIComboBox
{
	bool8 expanded;
	uint32 elementsOffsetInBytes;
	uint32 numElements;
	uint32 selectedIndex;
};

struct K15_GUIFloatSlider
{
	float value;
	float minValue;
	float maxValue;

	int32 mousePosPixelX;
	uint32 sliderPixelPosLeft;
	uint32 sliderPixelPosRight;
	uint32 sliderPixelPosTop;
	uint32 sliderPixelPosBottom;
};

struct K15_GUIWindow
{
	K15_GUIWindowState state;
	uint32 titleOffsetInBytes;
	uint32 titleLength;
	uint32 windowPixelWidth;
	uint32 windowPixelHeight;
	int32 dragPixelOffsetX;
	int32 dragPixelOffsetY;
};

struct K15_GUIElementHeader
{
	K15_GUIElementType type;

	int32 posPixelX;
	int32 posPixelY;
	uint32 pixelWidth;
	uint32 pixelHeight;
	uint32 identifierHash;
	uint32 offset;
	uint32 elementSizeInBytes;
	bool8 hovered;
	bool8 activated;
};

struct K15_GUIContextStyle
{
	K15_RenderFontDesc* styleFont;

	uint32 textColor;
	uint32 disabledTextColor;
	uint32 textBackgroundColor;
	uint32 disabledTextBackgroundColor;

	//sizing
	uint32 windowTitlePixelHeight;
	uint32 sliderPixelHeight;
	uint32 sliderPixelWidth;
	uint32 sliderLinePixelWidth;

	//padding
	uint32 windowContentPixelPadding;
	uint32 buttonContentPixelPadding;

	//controls
	uint32 controlUpperBackgroundColor;
	uint32 controlLowerBackgroundColor;
	uint32 disabledControlUpperBackgroundColor;
	uint32 disabledControlLowerBackgroundColor;
	uint32 hoveredControlLowerBackgroundColor;
	uint32 hoveredControlUpperBackgroundColor;
	uint32 interactedControlLowerBackgroundColor;
	uint32 interactedControlUpperBackgroundColor;
	uint32 controlLowerBorderColor;
	uint32 controlUpperBorderColor;
	
	uint32 textLabelColor;

	uint32 windowUpperBackgroundColor;
	uint32 windowLowerBackgroundColor;
	uint32 windowTitleBarUpperColor;
	uint32 windowTitleBarLowerColor;
	uint32 windowBorderUpperColor;
	uint32 windowBorderLowerColor;
	uint32 windowTitleTextColor;
};

struct K15_GUIVerticalLayoutParameter
{
	uint32 minElementHeight;
};

struct K15_GUIHorizontalLayoutParameter
{
	uint32 minElementWidth;
};

struct K15_GUILayoutCategory
{
	byte param[128];
	uint32 sizeParamInBytes;
	uint32 pixelWidth;
	uint32 pixelHeight;
	uint32 pixelOffsetX;
	uint32 pixelOffsetY;
	uint32 elementPixelPadding;
	uint32 elementCount;
	uint32 guiBufferOffsetInBytes;
	int32 posPixelX;
	int32 posPixelY;
	K15_GUILayout layout;
};

struct K15_GUIContext
{
	K15_CustomMemoryAllocator memoryAllocator;

	K15_RenderMaterialDesc* guiRenderMaterial;
	K15_GUIWindow* currentWindow;

	K15_GUILayoutCategory layoutCategoryStack[K15_GUI_MAX_CATEGORIES];
	K15_GUIContextStyle style;

	K15_Semaphore* memoryLock;

	byte* guiMemory[K15_GUI_MEMORY_BUFFER_COUNT];
	
	bool8 leftMouseDown;
	bool8 rightMouseDown;

	uint32 layoutCategoryIndex;
	uint32 maxLayoutCategoryIndex; //reset per draw
	uint32 activeElementIdentifier;
	uint32 hoveredElementIdentifier;
	uint32 guiMemoryMaxSize;
	uint32 guiMemoryCurrentSize[K15_GUI_MEMORY_BUFFER_COUNT];

	uint32 mousePosPixelX;
	uint32 mousePosPixelY;

	uint32 windowWidth;
	uint32 windowHeight;
};

K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue);

void K15_FlipGUIContextMemory(K15_GUIContext* p_GUIContext);
void K15_PrepareGUIContextForRendering(K15_GUIContext* p_GUIContext);

void K15_HandleGUIInputEvent(K15_GUIContext* p_GUIContext, K15_SystemEvent* p_SystemEvent);
void K15_HandleGUIWindowEvent(K15_GUIContext* p_GUIContext, K15_SystemEvent* p_SystemEvent);

void K15_SetGUIContextWindowSize(K15_GUIContext* p_GUIContext, uint32 p_WindowWidth, uint32 p_WindowHeight);
void K15_SetGUIContextMousePosition(K15_GUIContext* p_GUIContext, uint32 p_MouseX, uint32 p_MouseY);

char* K15_ComboBox(K15_GUIContext* p_GUIContext, char** p_Elements, uint32 p_NumElements, const char* p_Identifier);
bool8 K15_BeginWindow(K15_GUIContext* p_GUIContext, const char* p_Caption, int32* p_LeftPixelPos, int32* p_TopPixelPos, 
	uint32* p_WindowWidth, uint32* p_WindowHeight, K15_GUILayout p_Layout, const char* p_Identifier);
void K15_EndWindow(K15_GUIContext* p_GUIContext);
bool8 K15_Button(K15_GUIContext* p_GUIContext, const char* p_Caption, const char* p_Identifier);
void K15_Label(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier);
float K15_FloatSlider(K15_GUIContext* p_GUIContext, float p_Value, float p_MinValue, float p_MaxValue, const char* p_Identifier);
void K15_PushVerticalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding, uint32 p_MinElementPixelHeight);
void K15_PushHorizontalLayout(K15_GUIContext* p_GUIContext, uint32 p_ElementPadding, uint32 p_MinElementPixelWidth);

void K15_PopLayoutCategory(K15_GUIContext* p_GUIContext);

void K15_IterateGUIElementsHelper(K15_GUIContext* p_GUIContext, K15_GUIIterator p_GUIIterator, 
	void* p_UserData, uint32 p_BufferIndex);

#endif //_K15_GUILayer_Context_h_