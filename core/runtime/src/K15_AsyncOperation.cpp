#include "K15_AsyncOperation.h"

#include "K15_Thread.h"
#include "K15_OSContext.h"
#include "K15_DefaultCLibraries.h"

#include <K15_Logging.h>

#include "generated/K15_AsyncOperationStretchBuffer.cpp"

/*********************************************************************************/
struct K15_AsyncThreadParameter
{
	K15_Semaphore* asyncWorkSynchronizer;
	K15_Mutex* asyncJobLock;
	K15_AsyncOperationStretchBuffer* asyncOperationBuffer;
};
/*********************************************************************************/

/*********************************************************************************/
intern void K15_InternalProcessAsyncOperation(K15_AsyncOperation* p_AsyncOperation)
{
	void* userData = p_AsyncOperation->userData;
	K15_AsyncFunctionFnc asyncFunction = p_AsyncOperation->asyncFunction;
	K15_AsyncCallbackFnc asyncCallback = p_AsyncOperation->asyncCallback;
	uint32 flags = p_AsyncOperation->flags;

	asyncFunction(userData);
	
	if (asyncCallback)
	{
		asyncCallback(p_AsyncOperation);
	}

	p_AsyncOperation->asyncStatus = K15_ASYNC_OPERATION_FINISHED;

	if ((flags & K15_ASYNC_OPERATION_CLEAR_AFTER_PROCESS) > 0)
	{
		K15_ClearAsyncOperation(p_AsyncOperation);
	}
}
/*********************************************************************************/
intern uint8 K15_InternalAsyncOperationThreadProc(void* p_Parameter)
{
	K15_AsyncThreadParameter* asyncParameter = (K15_AsyncThreadParameter*)p_Parameter;
	K15_AsyncOperationStretchBuffer* asyncOperationBuffer = asyncParameter->asyncOperationBuffer;
	K15_Mutex* asyncJobLock = asyncParameter->asyncJobLock;

	K15_Semaphore* asyncWorkerSynchronizer = asyncParameter->asyncWorkSynchronizer;
	
	while(true)
	{
		K15_WaitSemaphore(asyncWorkerSynchronizer);

		K15_LockMutex(asyncJobLock);

		K15_AsyncOperation** asyncOperationPtr = K15_GetAsyncOperationStretchBufferElementUnsafe(asyncOperationBuffer, 0);
		K15_AsyncOperation* asyncOperation = 0;

		if (asyncOperationPtr)
		{
			asyncOperation = *asyncOperationPtr;
			K15_PopAsyncOperationStretchBufferIndex(asyncOperationBuffer, 0);
		}

		K15_UnlockMutex(asyncJobLock);

		if (asyncOperation)
		{
			K15_InternalProcessAsyncOperation(asyncOperation);
		}
	}
}
/*********************************************************************************/



/*********************************************************************************/
K15_AsyncContext* K15_CreateAsyncContext(K15_OSContext* p_OSContext, K15_MallocFnc p_MallocFnc, K15_FreeFnc p_FreeFnc)
{
	K15_ASSERT_TEXT(p_OSContext, "OS Context is NULL.");

	K15_AsyncContext* asyncContext = (K15_AsyncContext*)p_MallocFnc(sizeof(K15_AsyncContext));

	K15_AsyncOperationStretchBuffer asyncOperationBuffer = {};
	K15_CreateAsyncOperationStretchBuffer(&asyncOperationBuffer);

	uint32 numHardwareThreads = p_OSContext->threading.numHardwareThreads;

	K15_ThreadFixedBuffer asyncThreadBuffer = {};
	K15_CreateThreadFixedBuffer(&asyncThreadBuffer, numHardwareThreads);

	K15_Semaphore* asyncWorkerSynchronizer = K15_CreateSemaphore();
	K15_Mutex* asyncJobLock = K15_CreateMutex();

	asyncContext->asyncJobLock = asyncJobLock;
 	asyncContext->asyncThreads = asyncThreadBuffer;
 	asyncContext->asyncOperations = asyncOperationBuffer;
 	asyncContext->asyncWorkerSynchronizer = asyncWorkerSynchronizer;

  	asyncContext->mallocFnc = p_MallocFnc;
  	asyncContext->freeFnc = p_FreeFnc;

	K15_AsyncThreadParameter* threadParameter = (K15_AsyncThreadParameter*)p_MallocFnc(sizeof(K15_AsyncThreadParameter));
	threadParameter->asyncOperationBuffer = &asyncContext->asyncOperations;
	threadParameter->asyncJobLock = asyncContext->asyncJobLock;
	threadParameter->asyncWorkSynchronizer = asyncWorkerSynchronizer;

	uint32 threadAffinityMask = 0x1;

	for (uint32 hardwareThreadIndex = 0;
		 hardwareThreadIndex < numHardwareThreads;
		 ++hardwareThreadIndex)
	{
		char* threadNameBuffer = (char*)alloca(32);
		sprintf(threadNameBuffer, "AsyncWorkerThread %d", hardwareThreadIndex + 1);

		K15_Thread* asyncWorkerThread = K15_CreateThread(K15_InternalAsyncOperationThreadProc, threadParameter, 0);
		K15_PushThreadFixedBufferElement(&asyncThreadBuffer, asyncWorkerThread);
		K15_StartThread(asyncWorkerThread);
		K15_SetThreadName(asyncWorkerThread, threadNameBuffer);
		K15_SetThreadAffinityMask(asyncWorkerThread, threadAffinityMask);

		threadAffinityMask = (threadAffinityMask << 1);
	}

	return asyncContext;
}
/*********************************************************************************/
K15_AsyncOperation* K15_CreateAsyncOperation(K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_AsyncFunction, "Async Operation Function is NULL.");

	K15_AsyncOperation* asyncOperation = (K15_AsyncOperation*)K15_RT_MALLOC(sizeof(K15_AsyncOperation));

	K15_InitializeAsyncOperation(asyncOperation, p_AsyncFunction, p_AsyncCallback, p_UserData, p_UserDataSize, p_Flags);

	return asyncOperation;
}
/*********************************************************************************/
K15_AsyncOperation* K15_CreateAsyncOperationAndCopyUserData(K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_AsyncFunction, "Async Operation Function is NULL.");
	K15_ASSERT_TEXT(p_UserData, "User Data is NULL.");
	K15_ASSERT_TEXT(p_UserDataSize, "User Data Size is 0.");

	uint32 flags = p_Flags | K15_ASYNC_OPERATION_USER_DATA_COPY_FLAG;

	return K15_CreateAsyncOperation(p_AsyncFunction, p_AsyncCallback, p_UserData, p_UserDataSize, flags);
}
/*********************************************************************************/
void K15_InitializeAsyncOperation(K15_AsyncOperation* p_AsyncOperation, K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_AsyncOperation, "Async Operation is NULL.");
	K15_ASSERT_TEXT(p_AsyncFunction, "Async Operation Function is NULL.");

	if ((p_Flags & K15_ASYNC_OPERATION_USER_DATA_COPY_FLAG) > 0)
	{
		K15_ASSERT_TEXT(p_UserData, "User Data is NULL.");
		K15_ASSERT_TEXT(p_UserDataSize, "User Data Size is 0.");

		byte* userDataCopy = (byte*)K15_RT_MALLOC(p_UserDataSize);
		memcpy(userDataCopy, p_UserData, p_UserDataSize);
		p_UserData = userDataCopy;
	}

	p_AsyncOperation->asyncCallback = p_AsyncCallback;
	p_AsyncOperation->asyncFunction = p_AsyncFunction;
	p_AsyncOperation->userData = p_UserData;
	p_AsyncOperation->asyncStatus = K15_ASYNC_OPERATION_NOT_QUEUED;
	p_AsyncOperation->flags = p_Flags;
}
/*********************************************************************************/
void K15_ClearAsyncOperation(K15_AsyncOperation* p_AsyncOperation)
{
	K15_ASSERT_TEXT(p_AsyncOperation, "Async Operation is NULL.");
	uint32 flags = p_AsyncOperation->flags;
	void* userData = p_AsyncOperation->userData;

	if ((flags & K15_ASYNC_OPERATION_USER_DATA_COPY_FLAG) > 0)
	{
		K15_RT_FREE(userData);
	}
}
/*********************************************************************************/
uint8 K15_AddAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncOperation* p_AsyncOperation)
{
	K15_ASSERT_TEXT(p_AsyncContext, "Async Context is NULL.");
	K15_ASSERT_TEXT(p_AsyncOperation, "Async Operation is NULL.");
	K15_ASSERT_TEXT(p_AsyncOperation->asyncFunction, "Async Operation Function is NULL.");

	K15_Semaphore* asyncWorkerSynchronizer = p_AsyncContext->asyncWorkerSynchronizer;
	K15_Mutex* asyncJobLock = p_AsyncContext->asyncJobLock;

 	K15_AsyncOperationStretchBuffer* asyncOperationBuffer = &p_AsyncContext->asyncOperations;

	K15_LockMutex(asyncJobLock);
	K15_PushAsyncOperationStretchBufferElement(asyncOperationBuffer, p_AsyncOperation);
	K15_UnlockMutex(asyncJobLock);

	p_AsyncOperation->asyncStatus = K15_ASYNC_OPERATION_QUEUED;

	K15_PostSemaphore(asyncWorkerSynchronizer);

	return K15_SUCCESS;
}
/*********************************************************************************/