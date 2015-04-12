#include "win32/K15_ThreadWin32.h"
#include "win32/K15_EnvironmentWin32.h"

#include "K15_OSLayer_OSContext.h"
#include "K15_OSLayer_Thread.h"

#ifdef K15_OS_WINDOWS

/*********************************************************************************/
intern DWORD WINAPI K15_Win32ThreadWrapper(LPVOID p_Parameter)
{
	DWORD result = ERROR_SUCCESS;
	K15_ThreadFnc threadFunction = 0;
	void* threadParameter = 0;

	memcpy(&threadFunction, p_Parameter, K15_PTR_SIZE);
	memcpy(&threadParameter, ((byte*)p_Parameter + K15_PTR_SIZE), K15_PTR_SIZE);

	uint8 threadFunctionResult = threadFunction(threadParameter);

	if (threadFunctionResult != K15_SUCCESS
		&& threadFunctionResult == K15_OS_ERROR_SYSTEM)
	{
		result = GetLastError();
	}
	else if (threadFunctionResult != K15_SUCCESS)
	{
		result = 1;
	}

	return result;
}
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_Win32CreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)malloc(sizeof(K15_Win32Thread));
	byte* win32ThreadParameterBuffer = (byte*)malloc(K15_PTR_SIZE * 2);
	
	if (!win32Thread || ! win32ThreadParameterBuffer)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}
	
	memcpy(win32ThreadParameterBuffer, &p_ThreadFunction, K15_PTR_SIZE);
	memcpy(win32ThreadParameterBuffer + K15_PTR_SIZE, &p_ThreadParameter, K15_PTR_SIZE);

	DWORD threadIdentifier = 0;
	HANDLE threadHandle = CreateThread(0, size_megabyte(1), K15_Win32ThreadWrapper, (LPVOID)win32ThreadParameterBuffer, 0, &threadIdentifier);

	if (threadHandle == INVALID_HANDLE_VALUE)
	{
		free(win32Thread);
		free(win32ThreadParameterBuffer);

		return K15_OS_ERROR_SYSTEM;
	}

	win32Thread->handle = threadHandle;
	win32Thread->identifier = threadIdentifier;
	win32Thread->parameterBuffer = win32ThreadParameterBuffer;

	p_Thread->userData = (void*)win32Thread;

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_Thread* K15_Win32GetCurrentThread()
{
	HANDLE threadHandle = GetCurrentThread();
	K15_Win32Thread* win32Thread = 0;
	K15_Thread* currentThread = 0;
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

		win32Thread = (K15_Win32Thread*)currentThread->userData;

		if (win32Thread->handle == threadHandle)
		{
			return currentThread;
		}
	}

	return 0;
}
/*********************************************************************************/
K15_Mutex* K15_CreateMutex()
{
	K15_Mutex* win32Mutex = (K15_Mutex*)malloc(sizeof(K15_Mutex));

	assert(win32Mutex);

	InitializeCriticalSection(&win32Mutex->criticalSection);
	win32Mutex->locked = 0;

	return win32Mutex;
}
/*********************************************************************************/
uint8 K15_LockMutex(K15_Mutex* p_Mutex)
{
	uint8 returnValue = K15_SUCCESS;

	EnterCriticalSection(&p_Mutex->criticalSection);
	InterlockedIncrement(&p_Mutex->locked);

	return returnValue;
}
/*********************************************************************************/
uint8 K15_UnlockMutex(K15_Mutex* p_Mutex)
{
	uint8 returnValue = K15_SUCCESS;

	LeaveCriticalSection(&p_Mutex->criticalSection);
	InterlockedDecrement(&p_Mutex->locked);

	return returnValue;
}
/*********************************************************************************/
uint8 K15_Win32SetThreadName(K15_Thread* p_Thread)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)p_Thread->userData;
	K15_ThreadContext* threadContext = p_Thread->context;

	///http://blogs.msdn.com/b/stevejs/archive/2005/12/19/505815.aspx
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in user addr space)
		DWORD dwThreadID; // thread ID (-1=caller thread)
		DWORD dwFlags; // reserved for future use, must be zero
	} THREADNAME_INFO;

	
	THREADNAME_INFO info;

	info.dwType = 0x1000;
	info.szName = threadContext->name;
	info.dwThreadID = win32Thread->identifier;
	info.dwFlags = 0;

	__try
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (const ULONG_PTR*)&info );
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
		return K15_SUCCESS;
	}

	return K15_OS_ERROR_SYSTEM;
}
/*********************************************************************************/
K15_Semaphore* K15_CreateSemaphore()
{
	return K15_CreateSemaphoreWithInitialValue(0);
}
/*********************************************************************************/
K15_Semaphore* K15_CreateSemaphoreWithInitialValue(uint32 p_InitialValue)
{
	K15_Semaphore* win32Semaphore = (K15_Semaphore*)malloc(sizeof(K15_Semaphore));

	assert(win32Semaphore);

	HANDLE semaphoreHandle = CreateSemaphore(0, p_InitialValue, 100, 0);

	assert(semaphoreHandle != INVALID_HANDLE_VALUE);

	win32Semaphore->handle = semaphoreHandle;
	win32Semaphore->count = p_InitialValue;

	return win32Semaphore;
}
/*********************************************************************************/
uint8 K15_PostSemaphore(K15_Semaphore* p_Semaphore)
{
	assert(p_Semaphore);

	uint8 returnValue = K15_SUCCESS;

	if (ReleaseSemaphore(p_Semaphore->handle, 1, 0) != FALSE)
	{
		returnValue = K15_OS_ERROR_SYSTEM;
	}

	InterlockedIncrement(&p_Semaphore->count);

	return returnValue;
}
/*********************************************************************************/
uint8 K15_WaitSemaphore(K15_Semaphore* p_Semaphore)
{
	assert(p_Semaphore);

	DWORD returnValue = WaitForSingleObject(p_Semaphore->handle, INFINITE);
	InterlockedDecrement(&p_Semaphore->count);

	return K15_SUCCESS;
}
/*********************************************************************************/

#endif //K15_OS_WINDOWS