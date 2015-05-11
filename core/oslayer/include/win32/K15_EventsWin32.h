#ifndef _K15_Events_Win32_h_
#define _K15_Events_Win32_h_

#include "K15_Prerequisites.h"
#include "win32/K15_HeaderDefaultWin32.h"

LRESULT CALLBACK K15_Win32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

uint8 K15_Win32PumpSystemEvents(K15Context* p_OSContext);

#endif //_K15_Events_Win32_h_
