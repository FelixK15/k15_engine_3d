#ifndef _K15_OSLayer_Window_Android_h_
#define _K15_OSLayer_Window_Android_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_AndroidWindow
{
	ANativeWindow* nativeWindow;
};


uint8 K15_AndroidCreateWindow(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, uint32 p_Flags, uint8 p_MonitorIndex);
uint8 K15_AndroidSetWindowDimension(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, uint32 p_Height, uint32 p_Width);
uint8 K15_AndroidSetWindowFullscreen(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, bool8 p_Fullscreen);
uint8 K15_AndroidSetWindowTitle(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, char* p_Title);
uint8 K15_AndroidCloseWindow(K15_OSLayerContext* p_OSContext, K15_Window* p_Window);
#endif //_K15_OSLayer_Window_Android_h_