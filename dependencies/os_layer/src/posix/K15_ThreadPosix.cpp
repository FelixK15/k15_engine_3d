#include "posix/K15_ThreadPosix.h"

#include "K15_OSLayer_OSContext.h"
#include "K15_OSLayer_Thread.h"

/*********************************************************************************/
intern void* K15_PosixThreadWrapper(void* p_Parameter)
{
	int result = 0;
	K15_ThreadFnc threadFunction = 0;
	void* threadParameter = 0;

	memcpy(&threadFunction, p_Parameter, K15_PTR_SIZE);
	memcpy(&threadParameter, ((byte*)p_Parameter + K15_PTR_SIZE), K15_PTR_SIZE);

	uint8 threadFunctionResult = threadFunction(threadParameter);

	if (threadFunctionResult != K15_SUCCESS
		&& threadFunctionResult == K15_OS_ERROR_SYSTEM)
	{
		result = errno;
	}
	else if (threadFunctionResult != K15_SUCCESS)
	{
		result = 1;
	}

	pthread_exit((void*)result);
}
/*********************************************************************************/

/*********************************************************************************/
uint8 K15_PosixCreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter)
{
	K15_PosixThread* posixThread = (K15_PosixThread*)K15_OS_MALLOC(sizeof(K15_PosixThread));
	byte* posixThreadParameterBuffer = (byte*)K15_OS_MALLOC(K15_PTR_SIZE * 2);

	if (!posixThread || !posixThreadParameterBuffer)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	memcpy(posixThreadParameterBuffer, &p_ThreadFunction, K15_PTR_SIZE);
	memcpy(posixThreadParameterBuffer + K15_PTR_SIZE, &p_ThreadParameter, K15_PTR_SIZE);

	pthread_t threadHandle = 0;

	int result = pthread_create(&threadHandle, 0, K15_PosixThreadWrapper, posixThreadParameterBuffer);

	if (result != 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	posixThread->handle = threadHandle;

	p_Thread->userData = (void*)posixThread;

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_Thread* K15_PosixGetCurrentThread()
{
	pthread_t threadHandle = pthread_self();
	K15_Thread* currentThread = 0;
	K15_PosixThread* currentPosixThread = 0;
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	//iterate through all threads and check the thread handle
	for (uint32 threadIndex = 0;
		threadIndex < K15_MAX_THREADS;
		++threadIndex)
	{
		currentThread = osContext->threading.threads[threadIndex];
		
		if (!currentThread)
		{
			break;
		}

		currentPosixThread = (K15_PosixThread*)currentThread->userData;

		if (currentPosixThread->handle == threadHandle)
		{
			return currentThread;
		}
	}

	return 0;
}
/*********************************************************************************/
K15_Mutex* K15_PosixCreateMutex()
{
	K15_Mutex* posixMutex = (K15_Mutex*)K15_OS_MALLOC(sizeof(K15_Mutex));
	assert(posixMutex);

	pthread_mutex_t handle = {};

	int result = pthread_mutex_init(&handle, 0);

	if (result != 0)
	{
		K15_OS_FREE(posixMutex);

		return 0;
	}

	posixMutex->handle = handle;

	return posixMutex;
}
/*********************************************************************************/
uint8 K15_PosixLockMutex(K15_Mutex* p_Mutex)
{
	int result = pthread_mutex_lock(&p_Mutex->handle);

	if (result != 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_PosixUnlockMutex(K15_Mutex* p_Mutex)
{
	int result = pthread_mutex_unlock(&p_Mutex->handle);

	if (result != 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_Semaphore* K15_PosixCreateSemaphore()
{
	return K15_PosixCreateSemaphoreWithInitialValue(0);
}
/*********************************************************************************/
K15_Semaphore* K15_PosixCreateSemaphoreWithInitialValue(uint32 p_InitialValue)
{
	K15_Semaphore* posixSemaphore = (K15_Semaphore*)K15_OS_MALLOC(sizeof(K15_Semaphore));

	assert(posixSemaphore);

	sem_t handle = {};

	int result = sem_init(&handle, 0, p_InitialValue);

	if (result != 0)
	{
		K15_OS_FREE(posixSemaphore);

		return 0;
	}

	posixSemaphore->handle = handle;

	return posixSemaphore;
}
/*********************************************************************************/
uint8 K15_PosixPostSemaphore(K15_Semaphore* p_Semaphore)
{
	int result = sem_post(&p_Semaphore->handle);

	if (result != 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_PosixWaitSemaphore(K15_Semaphore* p_Semaphore)
{
	int result = sem_wait(&p_Semaphore->handle);

	if (result != 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}
	
	return K15_SUCCESS;
}
/*********************************************************************************/
