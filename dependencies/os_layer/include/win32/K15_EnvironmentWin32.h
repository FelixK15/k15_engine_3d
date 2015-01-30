#ifndef _K15_OSLayer_Environment_Win32_h_
#define _K15_OSLayer_Environment_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_Win32Context
{
	HINSTANCE hInstance;
};

uint8 K15_Win32InitializeOSLayer(HINSTANCE p_hInstance);

#endif //_K15_OSLayer_Environment_Win32_h_