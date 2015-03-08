#ifndef _K15_OSLayer_Thread_h_
#define _K15_OSLayer_Thread_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_ThreadWin32.h"
#elif K15_OS_LINUX || K15_OS_ANDROID
	#include "posix/K15_ThreadPosix.h"
#endif 

struct K15_ThreadContext
{
	K15_ThreadFnc function;
	void* parameter;

	char* name;
	uint32 nameLength;
};

struct K15_Thread
{
	K15_ThreadContext* context;

	void* userData;
};

K15_Thread* K15_CreateThread(K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter);
K15_Thread* K15_GetCurrentThread();
uint8 K15_SetThreadName(K15_Thread* p_Thread, const char* p_ThreadName);

#endif //_K15_OSLayer_Thread_h_