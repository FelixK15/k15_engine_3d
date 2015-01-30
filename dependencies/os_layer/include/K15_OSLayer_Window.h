#ifndef _K15_OSLayer_Window_h_
#define _K15_OSLayer_Window_h_

#include "K15_OSLayer_Prerequisites.h"

/*********************************************************************************/

enum K15_WindowFlags
{
	K15_WINDOW_FLAG_FULLSCREEN = 0x001,
	K15_WINDOW_FLAG_BORDERLESS = 0x002,
	K15_WINDOW_FLAG_NO_BUTTONS = 0x004,
	K15_WINDOW_FLAG_DEFAULT_DIMENISON = 0x008,
	K15_WINDOW_FLAG_HAS_FOCUS = 0x020
}; // end of K15_WindowFlags enums

enum K15_WindowDataFlags
{
	K15_WINDOW_DATA_FLAG_TITLE = 0x001,
	K15_WINDOW_DATA_FLAG_WIDTH = 0x002,
	K15_WINDOW_DATA_FLAG_HEIGHT = 0x004,
	K15_WINDOW_DATA_FLAG_FLAGS = 0x008,
	K15_WINDOW_DATA_FLAG_MONITOR_INDEX = 0x010
};

/*********************************************************************************/

struct K15_Window
{
	const char* title;
	uint32 width;
	uint32 height;
	uint32 flags;
	uint8 monitorIndex;
	void* userData;
};

/*********************************************************************************/

K15_Window* K15_CreateWindow(uint32 p_Flags = 0, uint8 p_MonitorIndex = 0);
bool8 K15_SetWindowDimension(K15_Window* p_Window, uint32 p_Height, uint32 p_Width);
bool8 K15_SetWindowFullscreen(K15_Window* p_Window, bool8 p_Fullscreen);
bool8 K15_SetWindowTitle(K15_Window* p_Window, char* p_Title);
void K15_CloseWindow(K15_Window* p_Window);


#endif //_K15_OSLayer_Window_h_