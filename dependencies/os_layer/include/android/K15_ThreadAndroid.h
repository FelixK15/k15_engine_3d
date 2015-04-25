#ifndef _K15_OSLayer_Thread_Android_h_
#define _K15_OSLayer_Thread_Android_h_

#include "K15_OSLayer_Prerequisites.h"

#ifndef _K15_OSLayer_Thread_Posix_h_
#define _K15_OSLayer_Thread_Posix_h_

#include "K15_OSLayer_Prerequisites.h"

#define K15_CreateMutex K15_AndroidCreateMutex
#define K15_LockMutex K15_PosixLockMutex
#define K15_UnlockMutex K15_PosixUnlockMutex
#define K15_CreateSemaphore K15_PosixCreateSemaphore
#define K15_CreateSemaphoreWithInitialValue K15_PosixCreateSemaphoreWithInitialValue
#define K15_PostSemaphore K15_PosixPostSemaphore
#define K15_WaitSemaphore K15_PosixWaitSemaphore

struct K15_AndroidThread
{
	pthread_t handle;
	JNIEnv* jniEnvironment;
};

struct K15_Mutex
{
	pthread_mutex_t handle;
};

struct K15_Semaphore
{
	sem_t handle;
};

uint8 K15_PosixCreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter);
uint8 K15_PosixSetThreadName(K15_Thread* p_Thread);
K15_Thread* K15_PosixGetCurrentThread();

K15_Mutex* K15_PosixCreateMutex();
uint8 K15_PosixLockMutex(K15_Mutex* p_Mutex);
uint8 K15_PosixUnlockMutex(K15_Mutex* p_Mutex);

K15_Semaphore* K15_PosixCreateSemaphore();
K15_Semaphore* K15_PosixCreateSemaphoreWithInitialValue(uint32 p_InitialValue);
uint8 K15_PosixPostSemaphore(K15_Semaphore* p_Semaphore);
uint8 K15_PosixWaitSemaphore(K15_Semaphore* p_Semaphore);

#endif //_K15_OSLayer_Thread_Posix_h_


#endif //_K15_OSLayer_Thread_Android_h_