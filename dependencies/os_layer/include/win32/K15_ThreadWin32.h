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

struct K15_Mutex
{
	//HANDLE handle;
	CRITICAL_SECTION criticalSection;
};

struct K15_Semaphore
{
	HANDLE handle;
	LONG count;
};

uint8 K15_Win32CreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter);
uint8 K15_Win32SetThreadName(K15_Thread* p_Thread);
K15_Thread* K15_Win32GetCurrentThread();

K15_Mutex* K15_CreateMutex();
uint8 K15_LockMutex(K15_Mutex* p_Mutex);
uint8 K15_UnlockMutex(K15_Mutex* p_Mutex);

K15_Semaphore* K15_CreateSemaphore();
K15_Semaphore* K15_CreateSemaphoreWithInitialValue(uint32 p_InitialValue);
uint8 K15_PostSemaphore(K15_Semaphore* p_Semaphore);
uint8 K15_WaitSemaphore(K15_Semaphore* p_Semaphore);


#endif //K15_OS_WINDOWS

#endif //_K15_OSLayer_Thread_Win32_h_