#ifndef _K15_OSLayer_Thread_Win32_h_
#define _K15_OSLayer_Thread_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_LinuxThread
{
	pthread_t threadIdentifier;
};

uint8 K15_LinuxCreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter);

#endif //_K15_OSLayer_Thread_Win32_h_