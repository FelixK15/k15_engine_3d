#ifndef _K15_Thread_Win32_h_
#define _K15_Thread_Win32_h_

#include "K15_Prerequisites.h"
#include "win32/K15_HeaderDefaultWin32.h"

#define K15_CreateMutex K15_Win32CreateMutex
#define K15_LockMutex K15_Win32LockMutex
#define K15_UnlockMutex K15_Win32UnlockMutex
#define K15_FreeMutex K15_Win32FreeMutex
#define K15_CreateSemaphore K15_Win32CreateSemaphore
#define K15_CreateSemaphoreWithInitialValue K15_Win32CreateSemaphoreWithInitialValue
#define K15_PostSemaphore K15_Win32PostSemaphore
#define K15_TryWaitSemaphore K15_Win32TryWaitSemaphore
#define K15_WaitSemaphore K15_Win32WaitSemaphore
#define K15_FreeSemaphore K15_Win32FreeSemaphore

#define K15_InterlockedIncrement						InterlockedIncrement
#define K15_InterlockedDecrement						InterlockedDecrement
#define K15_InterlockedCompareExchange					InterlockedCompareExchange
#define K15_InterlockedCompareExchangePointer			InterlockedCompareExchangePointer
#define K15_InterlockedCompareExchangePointerAcquire	InterlockedCompareExchangePointerAcquire
#define K15_InterlockedCompareExchangePointerRelease	InterlockedCompareExchangePointerRelease

/*********************************************************************************/
struct K15_Mutex
{
	//HANDLE handle;
	CRITICAL_SECTION criticalSection;
};
/*********************************************************************************/
struct K15_Semaphore
{
	HANDLE handle;
	LONG count;
};
/*********************************************************************************/
struct K15_Win32Thread
{
	HANDLE handle;
	unsigned long identifier;

	byte* parameterBuffer;
};
/*********************************************************************************/

uint8 K15_Win32CreateThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter, uint32 p_ThreadFlags);
uint8 K15_Win32StartThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread);
uint8 K15_Win32TryJoinThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread, uint32 p_MilliSeconds);
uint8 K15_Win32SetThreadAffinityMask(K15_OSContext* p_OSContext, K15_Thread* p_Thread, uint64 p_AffinityMask);
uint8 K15_Win32JoinThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread);
uint8 K15_Win32InterruptThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread);
uint8 K15_Win32SetThreadName(K15_Thread* p_Thread);
uint8 K15_Win32FreeThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread);
K15_Thread* K15_Win32GetCurrentThread();

K15_Mutex* K15_Win32CreateMutex();
uint8 K15_Win32LockMutex(K15_Mutex* p_Mutex);
uint8 K15_Win32TryLockMutex(K15_Mutex* p_Mutex);
uint8 K15_Win32UnlockMutex(K15_Mutex* p_Mutex);
uint8 K15_Win32FreeMutex(K15_Mutex* p_Mutex);

K15_Semaphore* K15_Win32CreateSemaphore();
K15_Semaphore* K15_Win32CreateSemaphoreWithInitialValue(uint32 p_InitialValue);
uint8 K15_Win32PostSemaphore(K15_Semaphore* p_Semaphore);
uint8 K15_Win32TryWaitSemaphore(K15_Semaphore* p_Semaphore, uint32 p_MilliSeconds);
uint8 K15_Win32WaitSemaphore(K15_Semaphore* p_Semaphore);
uint8 K15_Win32FreeSemaphore(K15_Semaphore* p_Semaphore);

#endif //_K15_Thread_Win32_h_