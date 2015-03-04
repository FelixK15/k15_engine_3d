#ifndef _K15_OSLayer_Thread_Win32_h_
#define _K15_OSLayer_Thread_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS

struct K15_Win32Thread
{
	HANDLE handle;
	DWORD identifier;

	byte* parameterBuffer;
};

uint8 K15_Win32CreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter);
uint8 K15_Win32SetThreadName(K15_Thread* p_Thread);

#endif //K15_OS_WINDOWS

#endif //_K15_OSLayer_Thread_Win32_h_