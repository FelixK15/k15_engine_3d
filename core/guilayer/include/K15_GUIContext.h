#ifndef _K15_GUILayer_Context_h_
#define _K15_GUILayer_Context_h_

#define K15_GUI_MAX_LAYOUTS 20
#define K15_GUI_MAX_ELEMENT_PTR_PER_LAYOUT 128
#define K15_GUI_MEMORY_BUFFER_COUNT 2
#define K15_GUI_MEMORY_FRONT_BUFFER 0
#define K15_GUI_MEMORY_BACK_BUFFER 1

#define K15_GUI_MAX_BUFFERED_MOUSE_INPUTS 16
#define K15_GUI_MAX_BUFFERED_KEYBOARD_INPUTS 16
#define K15_GUI_MAX_BUFFERED_SYSTEM_EVENTS 16

#define K15_GUI_RESULT_SUCCESS 0
#define K15_GUI_RESULT_OUT_OF_MEMORY 1
#define K15_GUI_RESULT_NOT_ENOUGH_MEMORY 2

#define kg_size_kilo_bytes(n) (n*1024)

#define K15_GUI_MIN_MEMORY_SIZE_IN_BYTES sizeof(K15_GUIContext) + kg_size_kilo_bytes(10)

typedef unsigned int kg_u32;
typedef unsigned short kg_u16;
typedef unsigned char kg_u8;

typedef int kg_s32;
typedef short kg_s16;
typedef char kg_s8;

typedef unsigned char kg_byte;
typedef unsigned char kg_b8;
typedef unsigned int kg_result;

struct K15_GUIContext;
struct K15_GUIElement;

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
enum K15_GUIElementFlags
{
	K15_GUI_ELEMENT_HOVERED = 0x001,
	K15_GUI_ELEMENT_FOCUSED = 0x002,
	K15_GUI_ELEMENT_MOUSE_DOWN = 0x004,
	K15_GUI_ELEMENT_CLICKED = 0x008,
	K15_GUI_ELEMENT_LAYOUTED = 0x010
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
	K15_GUI_WINDOW,
	K15_GUI_LABEL,
	K15_GUI_MENU,
	K15_GUI_SEPARATOR,
	K15_GUI_TOOLBAR,
	K15_GUI_MENU_ITEM,
}; 
/*********************************************************************************/
typedef struct _K15_GUIRectangle
{
	kg_s16 pixelPosLeft;
	kg_s16 pixelPosTop;
	kg_s16 pixelPosRight;
	kg_s16 pixelPosBottom;
} K15_GUIRectangle;
/*********************************************************************************/
typedef struct _K15_GUIDimension
{
	kg_u16 width;
	kg_u16 height;
} K15_GUIDimension;
/*********************************************************************************/
typedef struct _K15_GUISizeHint
{
	K15_GUIRectangle* rect;
	K15_GUIDimension dimHint;
	K15_GUIDimension dimMin;
} K15_GUISizeHint;
/*********************************************************************************/
typedef struct _K15_GUIElement
{
	K15_GUIElementType type;
	K15_GUIRectangle rect;
	K15_GUIDimension minSize;
	K15_GUIDimension sizeHint;
	kg_u32 identifierHash;
	kg_u32 offsetInBytes;
	kg_u32 sizeInBytes;
	kg_u32 flagMask;
} K15_GUIFont;
/*********************************************************************************/
typedef struct _K15_GUIFont
{

} K15_GUIFont;
/*********************************************************************************/
typedef struct _K15_GUIWindowStyle
{
	kg_u32 borderUpperColor;
	kg_u32 borderLowerColor;
	kg_u32 upperBackgroundColor;
	kg_u32 lowerBackgroundColor;
	kg_u32 upperTitleBackgroundColor;
	kg_u32 lowerTitleBackgroundColor;
	kg_u32 titleTextColor;
	kg_u16 titlePixelPadding;
	kg_u16 borderPixelThickness;
	K15_GUIFont* font;
} K15_GUIWindowStyle;
/*********************************************************************************/
typedef struct _K15_GUIButtonStyle
{
	kg_u32 borderUpperColor;
	kg_u32 borderLowerColor;
	kg_u32 upperBackgroundColor;
	kg_u32 lowerBackgroundColor;
	kg_u32 textColor;
	kg_u16 borderPixelThickness;
	kg_u16 verticalPixelPadding;
	kg_u16 horizontalPixelPadding;
	K15_GUIFont* font;
} K15_GUIButtonStyle;
/*********************************************************************************/
typedef struct _K15_GUILabelStyle
{
	kg_u32 textColor;
	K15_GUIFont* font;
} K15_GUILabelStyle;
/*********************************************************************************/
typedef struct _K15_GUIMenuStyle
{
	kg_u32 textColor;
	kg_u32 upperBackgroundColor;
	kg_u32 lowerBackgroundColor;
	kg_u32 upperHoveredBackgroundColor;
	kg_u32 lowerHoveredBackgroundColor;
	kg_u32 upperFocusedBackgroundColor;
	kg_u32 lowerFocusedBackgroundColor;
	kg_u32 verticalPixelPadding;
	kg_u32 horizontalPixelPadding;
	K15_GUIFont* font;
} K15_GUIMenuStyle;
/*********************************************************************************/
typedef struct _K15_GUIToolBarStyle
{
	kg_u32 lowerBackgroundColor;
	kg_u32 upperBackgroundColor;
	kg_u32 pixelHeight;
} K15_GUIToolBarStyle;
/*********************************************************************************/
typedef struct _K15_GUIMenuItemStyle
{
	kg_u32 lowerBackgroundColor;
	kg_u32 upperBackgroundColor;
	kg_u32 lowerHoveredBackgroundColor;
	kg_u32 upperHoveredBackgroundColor;
	kg_u32 lowerMouseDownBackgroundColor;
	kg_u32 upperMouseDownBackgroundColor;
	kg_u32 verticalPixelPadding;
	kg_u32 horizontalPixelPadding;
	kg_u32 textColor;
	K15_GUIFont* font;
} K15_GUIMenuItemStyle;
/*********************************************************************************/
typedef struct _K15_GUIContextStyle
{
	K15_GUIWindowStyle windowStyle;
	K15_GUIButtonStyle buttonStyle;
	K15_GUILabelStyle labelStyle;
	K15_GUIMenuStyle menuStyle;
	K15_GUIToolBarStyle toolBarStyle;
	K15_GUIMenuItemStyle menuItemStyle;
} K15_GUIContextStyle;
/*********************************************************************************/
typedef struct _K15_GUIToolBarData
{
	K15_GUIToolBarStyle* toolBarStyle;
} K15_GUIToolBarData;
/*********************************************************************************/
typedef struct _K15_GUIMenuData
{
	K15_GUIMenuStyle* menuStyle;
	char* title;
	kg_u32 textLength;
	kg_b8 expanded;
	kg_b8 subMenu;
} K15_GUIMenuData;
/*********************************************************************************/
typedef struct _K15_GUIMenuItemData
{
	K15_GUIMenuItemStyle* menuItemStyle;
	char* text;
	kg_u32 textLength;
} K15_GUIMenuItemData;
/*********************************************************************************/
typedef struct _K15_GUIWindowData
{
	K15_GUIWindowStyle* style;
	K15_GUIRectangle contentRect;
	char* title;
	kg_u32 textLength;
	kg_b8 expanded;
} K15_GUIWindowData;
/*********************************************************************************/
typedef struct _K15_GUIButtonData
{
	K15_GUIButtonStyle* style;
	char* text;
	kg_u32 textLength;
} K15_GUIButtonData;
/*********************************************************************************/
typedef struct _K15_GUILabelData
{
	K15_GUILabelStyle* style;
	char* text;
	kg_u32 textLength;
} K15_GUILabelData;
/*********************************************************************************/
typedef struct _K15_GUILayout
{
	K15_GUISizeHint elementSizeHints[K15_GUI_MAX_ELEMENTS_PER_LAYOUT];
	K15_GUIRectangle layoutRectangle;
	K15_GUILayoutType type;
	kg_u32 numElements;

	union 
	{
		kg_u16 fixedWidthPerElement;
		kg_u16 fixedHeightPerElement;
	} params;
} K15_GUILayout;
/*********************************************************************************/
enum K15_GUIContextFlags
{
	K15_GUI_CONTEXT_INSIDE_WINDOW_FLAG		= 0x01,
	K15_GUI_CONTEXT_INSIDE_MENU_FLAG		= 0x02
};
/*********************************************************************************/
enum K15_GUIMouseInputType : kg_u16
{
	K15_GUI_MOUSE_BUTTON_PRESSED = 0,
	K15_GUI_MOUSE_BUTTON_RELEASED,
	K15_GUI_MOUSE_WHEEL_UP,
	K15_GUI_MOUSE_WHEEL_DOWN,
	K15_GUI_MOUSE_MOVED
};
/*********************************************************************************/
enum K15_GUIMouseButtonType : kg_u16
{
	K15_GUI_MOUSE_BUTTON_LEFT = 0,
	K15_GUI_MOUSE_BUTTON_RIGHT,
	K15_GUI_MOUSE_BUTTON_MIDDLE,
	K15_GUI_MOUSE_BUTTON_SPECIAL1,
	K15_GUI_MOUSE_BUTTON_SPECIAL2
};
/*********************************************************************************/
enum K15_GUIKeyboardModifierType
{
	K15_GUI_CTRL_MODIFIER  = 0x01,
	K15_GUI_L_ALT_MODIFIER = 0x02,
	K15_GIU_R_ALT_MODIFIER = 0x04,
	K15_GUI_SHIFT_MODIFIER = 0x08
};
/*********************************************************************************/
enum K15_GUIKeyboardInputType
{
	K15_GUI_KEY_PRESSED = 0,
	K15_GUI_KEY_RELEASED,
	K15_GUI_TEXT_INPUT
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
	K15_GUI_KEY_ESC,

	K15_GUI_KEY_F1,
	K15_GUI_KEY_F2,
	K15_GUI_KEY_F3,
	K15_GUI_KEY_F4,
	K15_GUI_KEY_F5,
	K15_GUI_KEY_F6,
	K15_GUI_KEY_F7,
	K15_GUI_KEY_F8,
	K15_GUI_KEY_F9,
	K15_GUI_KEY_F10,
	K15_GUI_KEY_F11,
	K15_GUI_KEY_F12,

	K15_GUI_KEY_LEFT,
	K15_GUI_KEY_RIGHT,
	K15_GUI_KEY_UP,
	K15_GUI_KEY_DOWN,

	//all other keys.
	K15_GUI_KEY_NORMAL
};
/*********************************************************************************/
enum K15_GUISystemEventType
{
	K15_GUI_WINDOW_RESIZED = 0,
	K15_GUI_SYSTEM_EVENT_COUNT
};
/*********************************************************************************/
typedef struct _K15_GUIMouseInput
{
	union
	{
		struct
		{
			kg_u16 mousePosX;
			kg_u16 mousePosY;
		};
		
		kg_u32 mouseButton;
	};

	K15_GUIMouseInputType type;
} K15_GUIMouseInput;
/*********************************************************************************/
typedef struct _K15_GUIKeyboardInput
{
	struct 
	{
		kg_u16 key;
		kg_u16 modifierMask;
	} keyInput;

	K15_GUIKeyboardInputType type;
	K15_GUIKeyboardKeyType keyType;
	K15_GUIKeyboardModifierType modifier;
} K15_GUIKeyboardInput;
/*********************************************************************************/
typedef struct _K15_GUISystemEvent
{
	K15_GUISystemEventType type;
	
	union
	{
		struct 
		{
			kg_u16 width;
			kg_u16 height;
		} size;
	} params;
} K15_GUISystemEvent;
/*********************************************************************************/
typedef struct _K15_GUIContextEvents
{
	kg_u16 mousePosX;
	kg_u16 mousePosY;
	kg_s16 mouseDeltaX;
	kg_s16 mouseDeltaY;
	kg_u16 numBufferedSystemEvents;
	kg_u16 numBufferedMouseInputs;
	kg_u16 numBufferedKeyboardInputs;
	K15_GUISystemEvent bufferedSystemEvents[K15_GUI_MAX_BUFFERED_SYSTEM_EVENTS];
	K15_GUIMouseInput bufferedMouseInput[K15_GUI_MAX_BUFFERED_MOUSE_INPUTS];
	K15_GUIKeyboardInput bufferedKeyboardInput[K15_GUI_MAX_BUFFERED_KEYBOARD_INPUTS];
} K15_GUIContextEvents;
/*********************************************************************************/
typedef struct _K15_GUIResources
{
		
} K15_GUIResources;
/*********************************************************************************/
typedef struct _K15_GUIContext
{
	kg_byte* memoryBuffer;
	K15_GUIResources* resources;
	K15_GUIContextStyle style;
	K15_GUIContextEvents events;
	K15_GUILayout layoutStack[K15_GUI_MAX_LAYOUTS];
	kg_u8 numMenus;
	kg_u8 numLayouts;
	kg_u8 layoutIndex;
	kg_u16 windowWidth;
	kg_u16 windowHeight;
	kg_u32 focusedElementIdHash;
	kg_u32 clickedElementIdHash;
	kg_u32 activatedElementIdHash;
	kg_u32 hoveredElementIdHash;
	kg_u32 mouseDownElementIdHash;
	kg_u32 memoryBufferCapacityInBytes;
	kg_u32 memoryBufferSizeInBytes;
	kg_u32 flagMask;
} K15_GUIContext;
/*********************************************************************************/

//how this should work:
// - Call gui logic (K15_GUIButton, K15_GUIBeginWindow, K15_GUIPushHorizontalLayout etc.)
// - Store elements internally in the gui context (just rects?) <-- headache
// - iterate over elements (layouting)
// - iterate over elements (input)
// - iterate over elements (rendering)
// - Call gui logic (next frame - retrieve results from last frame. Mainly results of the input)

K15_GUIContext* K15_CreateGUIContext();
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(kg_byte* p_Memory, kg_u32 p_MemorySizeInBytes);

kg_byte K15_GUIGetGUIElementMemory(K15_GUIElement* p_GUIElement);

void K15_GUIBeginDockingArea(K15_GUIContext* p_GUIContext, kg_s16 p_PosX, kg_s16 p_PosY,
	kg_u16 p_Width, kg_u16 p_Height, kg_u32 p_AllowedDockingAreasMask);

void K15_GUIBeginToolBar(K15_GUIContext* p_GUIContext, const char* p_Identifier);
void K15_GUIBeginToolBarEX(K15_GUIContext* p_GUIContext, const char* p_Identifier, K15_GUIToolBarStyle* p_ToolBarStyle);

kg_b8 K15_GUIBeginWindow(K15_GUIContext* p_GUIContext, kg_s16* p_PosX, kg_s16* p_PosY,
	kg_u16* p_Width, kg_u16* p_Height, const char* p_Title, const char* p_Identifier);
kg_b8 K15_GUIBeginWindowEX(K15_GUIContext* p_GUIContext, kg_s16* p_PosX, kg_s16* p_PosY,
	kg_u16* p_Width, kg_u16* p_Height, const char* p_Title, const char* p_Identifier,
	K15_GUIWindowStyle* p_GUIWindowStyle);

kg_b8 K15_GUIBeginMenu(K15_GUIContext* p_GUIContext, const char* p_MenuText, const char* p_Identifier);
kg_b8 K15_GUIBeginMenuEX(K15_GUIContext* p_GUIContext, const char* p_MenuText, const char* p_Identifier, 
	K15_GUIMenuStyle* p_MenuStyle);

kg_b8 K15_GUIMenuItem(K15_GUIContext* p_GUIContext, const char* p_ItemText, const char* p_Identifier);
kg_b8 K15_GUIMenuItemEX(K15_GUIContext* p_GUIContext, const char* p_ItemText, const char* p_Identifier,
	K15_GUIMenuItemStyle* p_MenuItemStyle);

kg_b8 K15_GUIButton(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier);
kg_b8 K15_GUIButtonEX(K15_GUIContext* p_GUIContext, const char* p_ButtonText, const char* p_Identifier, 
	K15_GUIButtonStyle* p_GUIButtonStyle);

void K15_GUILabel(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier);
void K15_GUILabelEX(K15_GUIContext* p_GUIContext, const char* p_LabelText, const char* p_Identifier, 
	K15_GUILabelStyle* p_GUILabelStyle);

void K15_GUIPushVerticalLayout(K15_GUIContext* p_GUIContext, K15_GUIRectangle p_LayoutArea, 
	kg_u16 p_FixedWidthPerElement = 0);
void K15_GUIPushHorizontalLayout(K15_GUIContext* p_GUIContext, K15_GUIRectangle p_LayoutArea,
	kg_u16 p_FixedHeightPerElement = 0);

void K15_GUIPopLayout(K15_GUIContext* p_GUIContext);

void K15_GUIEndWindow(K15_GUIContext* p_GUIContext);
void K15_GUIEndMenu(K15_GUIContext* p_GUIContext);
void K15_GUIEndDockingArea(K15_GUIContext* p_GUIContext);
void K15_GUIEndToolBar(K15_GUIContext* p_GUIContext);

void K15_GUIFinishGUIFrame(K15_GUIContext* p_GUIContext);

void K15_GUIIterateElements(K15_GUIContext* p_GUIContext, K15_GUIIteratorFnc p_Function, void* p_UserData);
void K15_GUIIterateRetainedElements(K15_GUIContext* p_GUIContext, K15_GUIIteratorFnc p_Function, void* p_UserData);

void K15_GUIAddMouseInput(K15_GUIContextEvents* p_GUIContextEvents, K15_GUIMouseInput* p_MouseInput);
void K15_GUIAddKeyboardInput(K15_GUIContextEvents* p_GUIContextEvents, K15_GUIKeyboardInput* p_KeyboardInput);
void K15_GUIAddSystemEvent(K15_GUIContextEvents* p_GUIContextEvents, K15_GUISystemEvent* p_SystemEvent);

#endif //_K15_GUILayer_Context_h_