#ifndef _K15_OSLayer_Thread_h_
#define _K15_OSLayer_Thread_h_

#include "K15_OSLayer_Prerequisites.h"

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

uint8 K15_SetThreadName(K15_Thread* p_Thread, char* p_ThreadName);

#endif //_K15_OSLayer_Thread_h_