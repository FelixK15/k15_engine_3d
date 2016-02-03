#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

struct K15_SystemEvent;
struct K15_GUIContext;
struct K15_GUIElement;

#include "K15_GUIContextPrerequisites.h"
#include "K15_CustomMemoryAllocator.h"

#include <K15_RenderMaterialDesc.h>

typedef void(*K15_GUIIteratorFnc)(K15_GUIContext*, K15_GUIElement*, void*);
/*********************************************************************************/
enum K15_GUIDockingAreaFlags
{
	K15_GUI_LEFT_DOCKING_AREA = 0x001,
	K15_GUI_RIGHT_DOCKING_AREA = 0x002,
	K15_GUI_TOP_DOCKING_AREA = 0x004,
	K15_GUI_BOTTOM_DOCKING_AREA = 0x008
};
/*********************************************************************************/
enum K15_GUILayoutType
{
	K15_GUI_VERTICAL_LAYOUT = 0,
	K15_GUI_HORIZONTAL_LAYOUT
};
/*********************************************************************************/
enum K15_GUIElementType
{
	K15_GUI_BUTTON = 0,
	K15_GUI_SLIDER,
	K15_GUI_WINDOW
};
/*********************************************************************************/
struct K15_GUIRectangle
{
	int32 pixelPosLeft;
	int32 pixelPosTop;
	int32 pixelPosRight;
	int32 pixelPosBottom;
};
/*********************************************************************************/
struct K15_GUIElement
{
	K15_GUIElementType type;
	K15_GUIRectangle rect;
	uint32 identifierHash;
	uint32 offsetInBytes;
	uint32 sizeInBytes;
};
/*********************************************************************************/
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
/*********************************************************************************/
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
/*********************************************************************************/
struct K15_GUIContextStyle
{
	K15_GUIWindowStyle windowStyle;
	K15_GUIButtonStyle buttonStyle;
};
/*********************************************************************************/
struct K15_GUILayout
{
	K15_GUIRectangle* elementRectangles[K15_GUI_MAX_ELEMENTS_PER_LAYOUT];
	K15_GUIRectangle layoutRectangle;
	K15_GUILayoutType type;
	uint32 numElements;
};
/*********************************************************************************/
enum K15_GUIMouseInputType
{
	K15_GUI_LEFT_MOUSE_CLICK = 0,
	K15_GUI_RIGHT_MOUSE_CLICK,
	K15_GUI_MIDDLE_MOUSE_CLICK,

	K15_GUI_LEFT_MOUSE_RELEASE,
	K15_GUI_RIGHT_MOUSE_RELEASE,
	K15_GUI_MIDDLE_MOUSE_RELEASE,

	K15_GUI_MOUSE_WHEEL_UP,
	K15_GUI_MOUSE_WHEEL_DOWN
};
/*********************************************************************************/
enum K15_GUIKeyboardModifierType
{
	K15_GUI_CTRL_MODIFIER = 0,
	K15_GUI_L_ALT_MODIFIER,
	K15_GIU_R_ALT_MODIFIER,
	K15_GUI_SHIFT_MODIFIER
};
/*********************************************************************************/
enum K15_GUIKeyboardKeyType
{
	K15_GUI_KEY_RETURN = 0,
	K15_GUI_KEY_BACK,
	K15_GUI_KEY_TAB,
	K15_GUI_KEY_DEL,
	K15_GUI_KEY_PGDWN,
	K15_GUI_KEY_PGUP,
	K15_GUI_KEY_HOME,
	K15_GUI_KEY_END,
	K15_GUI_KEY_INS,

	//all other keys.
	K15_GUI_KEY_NORMAL
};
/*********************************************************************************/
struct K15_GUIMouseInput
{
	uint32 mousePosX;
	uint32 mousePosY;
	K15_GUIMouseInputType type;
};
/*********************************************************************************/
struct K15_GUIKeyboardInput
{
	uint32 charUTF32;
	K15_GUIKeyboardKeyType keyType;
	K15_GUIKeyboardModifierType modifier;
};
/*********************************************************************************/
struct K15_GUIContextInput
{
	uint32 mousePosX;
	uint32 mousePosY;
	uint32 numBufferedMouseInputs;
	uint32 numBufferedKeyboardInputs;
	K15_GUIMouseInput bufferedMouseInput[K15_GUI_MAX_BUFFERED_MOUSE_INPUTS];
	K15_GUIKeyboardInput bufferedKeyboardInput[K15_GUI_MAX_BUFFERED_KEYBOARD_INPUTS];
};
/*********************************************************************************/
struct K15_GUIContext
{
	byte* memoryBuffer;
	K15_CustomMemoryAllocator memoryAllocator;
	K15_GUIContextStyle style;
	K15_GUILayout layoutStack[K15_GUI_MAX_LAYOUTS];
	uint32 layoutIndex;
	uint32 numLayouts;
	uint32 memoryMaxSizeInBytes;
	uint32 memoryCurrentSizeInBytes;
	uint32 retainedDataOffsetInBytes;
	uint32 retainedDataSizeInBytes;
};
/*********************************************************************************/

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
	uint32 p_Width, uint32 p_Height, uint32 p_AllowedDockingAreasMask);

bool8 K15_GUIBeginWindow(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier);
bool8 K15_GUIBeginWindowEX(K15_GUIContext* p_GUIContext, int32* p_PosX, int32* p_PosY,
	uint32* p_Width, uint32* p_Height, const char* p_Identifier, K15_GUIWindowStyle* p_GUIWindowStyle);

bool8 K15_GUIButton(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier);
bool8 K15_GUIButtonEX(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier, K15_GUIButtonStyle* p_GUIButtonStyle);

void K15_GUIPushVerticalLayout(K15_GUIContext* p_GUIContext);
void K15_GUIPushHorizontalLayout(K15_GUIContext* p_GUIContext);

void K15_GUIPopLayout(K15_GUIContext* p_GUIContext);
void K15_GUIEndWindow(K15_GUIContext* p_GUIContext);

void K15_GUIEndDockingArea(K15_GUIContext* p_GUIContext);

void K15_GUIFinishGUIFrame(K15_GUIContext* p_GUIContext);

void K15_GUIIterateElements(K15_GUIContext* p_GUIContext, K15_GUIIteratorFnc p_Function, void* p_UserData);
void K15_GUIAddMouseInput(K15_GUIContextInput* p_GUIContextInput, K15_GUIMouseInput* p_MouseInput);
void K15_GUIAddKeyboardInput(K15_GUIContextInput* p_GUIContextInput, K15_GUIKeyboardInput* p_KeyboardInput);

#endif //_K15_GUILayer_Context_h_