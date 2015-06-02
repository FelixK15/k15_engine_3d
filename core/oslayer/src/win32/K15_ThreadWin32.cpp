#include "win32/K15_ThreadWin32.h"
#include "win32/K15_OSContextWin32.h"

#include "K15_OSContext.h"
#include "K15_Thread.h"
#include "K15_DefaultCLibraries.h"

#include <K15_Logging.h>

/*********************************************************************************/
intern DWORD WINAPI K15_Win32ThreadWrapper(LPVOID p_Parameter)
{
	DWORD result = ERROR_SUCCESS;
	K15_Thread* thread = 0;
	K15_ThreadFnc threadFunction = 0;
	void* threadParameter = 0;

	memcpy(&threadFunction, p_Parameter, K15_PTR_SIZE);
	memcpy(&threadParameter, ((byte*)p_Parameter + K15_PTR_SIZE), K15_PTR_SIZE);
	memcpy(&thread, ((byte*)p_Parameter + K15_PTR_SIZE * 2), K15_PTR_SIZE);

	thread->context->state = K15_THREAD_STATE_RUNNING;
	uint8 threadFunctionResult = threadFunction(threadParameter);
	thread->context->state = K15_THREAD_STATE_FINISHED;;

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
intern uint8 K15_Win32InternalCurrentThreadCompare(K15_Thread** p_Thread, void* p_UserData)
{
	DWORD threadIdentifier = (DWORD)p_UserData;
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)(*p_Thread)->userData;

	return win32Thread->identifier == threadIdentifier ? 0 : 1;
}
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_Win32CreateThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter, uint32 p_ThreadFlags)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)K15_OS_MALLOC(sizeof(K15_Win32Thread));
	byte* win32ThreadParameterBuffer = (byte*)K15_OS_MALLOC(K15_PTR_SIZE * 3);
	
	if (!win32Thread || ! win32ThreadParameterBuffer)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}
	
	memcpy(win32ThreadParameterBuffer, &p_ThreadFunction, K15_PTR_SIZE);
	memcpy(win32ThreadParameterBuffer + K15_PTR_SIZE, &p_ThreadParameter, K15_PTR_SIZE);
	memcpy(win32ThreadParameterBuffer + K15_PTR_SIZE * 2, &p_Thread, K15_PTR_SIZE);

	DWORD creationFlags = 0;

	if ((p_ThreadFlags & K15_THREAD_START_FLAG) == 0)
	{
		creationFlags = CREATE_SUSPENDED;
	}

	DWORD threadIdentifier = 0;
	HANDLE threadHandle = CreateThread(0, size_megabyte(1), K15_Win32ThreadWrapper, (LPVOID)win32ThreadParameterBuffer, creationFlags, &threadIdentifier);

	if (threadHandle == INVALID_HANDLE_VALUE)
	{
		K15_OS_FREE(win32Thread);
		K15_OS_FREE(win32ThreadParameterBuffer);

		return K15_OS_ERROR_SYSTEM;
	}

	win32Thread->handle = threadHandle;
	win32Thread->identifier = threadIdentifier;
	win32Thread->parameterBuffer = win32ThreadParameterBuffer;

	p_Thread->userData = (void*)win32Thread;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32StartThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)p_Thread->userData;

	DWORD result = ResumeThread(win32Thread->handle);

	return result == ((DWORD) - 1) ? K15_OS_ERROR_SYSTEM : K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32TryJoinThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread, uint32 p_MilliSeconds)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)p_Thread->userData;
	BOOL alertableThread = K15_GetCurrentThread() == K15_MAIN_THREAD ? TRUE : FALSE;

	DWORD result = WaitForSingleObjectEx(win32Thread->handle, DWORD(p_MilliSeconds), alertableThread);

	if (result == WAIT_FAILED)
	{
		return K15_OS_ERROR_SYSTEM;
	}
	else if (result == WAIT_TIMEOUT || result == WAIT_IO_COMPLETION)
	{
		return K15_OS_TIMEOUT;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32JoinThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread)
{
	return K15_Win32TryJoinThread(p_OSContext, p_Thread, INFINITE);
}
/*********************************************************************************/
uint8 K15_Win32InterruptThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)p_Thread->userData;

	BOOL threadInterrupted = TerminateThread(win32Thread->handle, 0);

	if (threadInterrupted == K15_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_Thread* K15_Win32GetCurrentThread()
{
	//HANDLE threadHandle = GetCurrentThread();
	DWORD threadIdentifier = GetCurrentThreadId();
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_ThreadStretchBuffer* threadBuffer = &osContext->threading.threads;

	//iterate through all threads and check the thread handle
// 	for (uint32 threadIndex = 0;
// 		 threadIndex < K15_MAX_THREADS;
// 		 ++threadIndex)
// 	{
// 		currentThread = osContext->threading.threads[threadIndex];
// 
// 		if (!currentThread)
// 		{
// 			break;
// 		}
// 
// 		win32Thread = (K15_Win32Thread*)currentThread->userData;
// 
// 		if (win32Thread->identifier == threadIdentifier)
// 		{
// 			return currentThread;
// 		}
// 	}

	K15_Thread** currentThread = K15_GetThreadStretchBufferElementConditional(threadBuffer, K15_Win32InternalCurrentThreadCompare, &threadIdentifier);

	return currentThread == 0 ? 0 : (*currentThread);
}
/*********************************************************************************/
K15_Mutex* K15_Win32CreateMutex()
{
	K15_Mutex* win32Mutex = (K15_Mutex*)K15_OS_MALLOC(sizeof(K15_Mutex));

	InitializeCriticalSectionAndSpinCount(&win32Mutex->criticalSection, 2000);

	return win32Mutex;
}
/*********************************************************************************/
uint8 K15_Win32LockMutex(K15_Mutex* p_Mutex)
{
	uint8 returnValue = K15_SUCCESS;

	EnterCriticalSection(&p_Mutex->criticalSection);

	return returnValue;
}
/*********************************************************************************/
uint8 K15_Win32TryLockMutex(K15_Mutex* p_Mutex)
{
	uint8 returnValue = K15_SUCCESS;

	BOOL success = TryEnterCriticalSection(&p_Mutex->criticalSection);

	if (success != TRUE)
	{
		returnValue = K15_OS_TIMEOUT;
	}

	return returnValue;
}
/*********************************************************************************/
uint8 K15_Win32UnlockMutex(K15_Mutex* p_Mutex)
{
	uint8 returnValue = K15_SUCCESS;

	LeaveCriticalSection(&p_Mutex->criticalSection);

	return returnValue;
}
/*********************************************************************************/
uint8 K15_Win32FreeMutex(K15_Mutex* p_Mutex)
{
	DeleteCriticalSection(&p_Mutex->criticalSection);

	K15_OS_FREE(p_Mutex);

	return K15_SUCCESS;
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
uint8 K15_Win32FreeThread(K15_OSContext* p_OSContext, K15_Thread* p_Thread)
{
	K15_Win32Thread* win32Thread = (K15_Win32Thread*)p_Thread->userData;

	BOOL handleClosed = CloseHandle(win32Thread->handle);

	if (handleClosed == K15_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_OS_FREE(win32Thread);

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_Semaphore* K15_Win32CreateSemaphore()
{
	return K15_Win32CreateSemaphoreWithInitialValue(0);
}
/*********************************************************************************/
K15_Semaphore* K15_Win32CreateSemaphoreWithInitialValue(uint32 p_InitialValue)
{
	K15_Semaphore* win32Semaphore = (K15_Semaphore*)K15_OS_MALLOC(sizeof(K15_Semaphore));

	HANDLE semaphoreHandle = CreateSemaphoreA(0, p_InitialValue, 100, 0);

	if (semaphoreHandle == INVALID_HANDLE_VALUE)
	{
		K15_LOG_ERROR_MESSAGE("K15_Win32CreateSemaphoreWithInitialValue (System Error): %s", K15_Win32GetError((char*)alloca(256)));
		return 0;
	}

	win32Semaphore->handle = semaphoreHandle;
	win32Semaphore->count = p_InitialValue;

	return win32Semaphore;
}
/*********************************************************************************/
uint8 K15_Win32PostSemaphore(K15_Semaphore* p_Semaphore)
{
	uint8 returnValue = K15_SUCCESS;

	if (ReleaseSemaphore(p_Semaphore->handle, 1, 0) != K15_FALSE)
	{
		returnValue = K15_OS_ERROR_SYSTEM;
	}

	InterlockedIncrement(&p_Semaphore->count);

	return returnValue;
}
/*********************************************************************************/
uint8 K15_Win32TryWaitSemaphore(K15_Semaphore* p_Semaphore, uint32 p_MilliSeconds)
{
	uint8 result = K15_SUCCESS;

	DWORD returnValue = WaitForSingleObjectEx(p_Semaphore->handle, (DWORD)p_MilliSeconds, FALSE);

	if (returnValue == WAIT_TIMEOUT || returnValue == WAIT_IO_COMPLETION) 
	{
		result = K15_OS_TIMEOUT;
	}
	else if(returnValue == WAIT_OBJECT_0)
	{
		InterlockedDecrement(&p_Semaphore->count);
	}
	else if(returnValue == WAIT_FAILED)
	{
		result = K15_OS_ERROR_SYSTEM;
	}

	return result;
}
/*********************************************************************************/
uint8 K15_Win32WaitSemaphore(K15_Semaphore* p_Semaphore)
{
	return K15_Win32TryWaitSemaphore(p_Semaphore, INFINITE);
}
/*********************************************************************************/
uint8 K15_Win32FreeSemaphore(K15_Semaphore* p_Semaphore)
{
	K15_ASSERT_TEXT(p_Semaphore->count == 0, "Count of semaphore is 0.");

	BOOL handleClosed = CloseHandle(p_Semaphore->handle);

	if (handleClosed == K15_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_OS_FREE(p_Semaphore);

	return K15_SUCCESS;
}
/*********************************************************************************/