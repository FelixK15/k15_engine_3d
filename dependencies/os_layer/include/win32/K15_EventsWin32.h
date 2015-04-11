#ifndef _K15_OSLayer_Events_Win32_h_
#define _K15_OSLayer_Events_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS

LRESULT CALLBACK K15_Win32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

uint8 K15_Win32PumpSystemEvents(K15_OSLayerContext* p_OSContext);

#endif //K15_OS_WINDOWS

#endif //_K15_OSLayer_Events_Win32_h_