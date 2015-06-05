#ifndef _K15_Runtime_Async_Operation_h_
#define _K15_Runtime_Async_Operation_h_

#include "K15_Prerequisites_RT.h"

#include "generated/K15_ThreadFixedBuffer.h"
#include "generated/K15_AsyncOperationStretchBuffer.h"

typedef void (*K15_AsyncCallbackFnc)(K15_AsyncOperation*);
typedef void (*K15_AsyncFunctionFnc)(void*);

/*********************************************************************************/
enum K15_AsyncStatus
{
	K15_ASYNC_OPERATION_NOT_QUEUED = 0,
	K15_ASYNC_OPERATION_QUEUED,
	K15_ASYNC_OPERATION_FINISHED
};
/*********************************************************************************/
enum K15_AsyncOperationFlags
{
	K15_ASYNC_OPERATION_USER_DATA_COPY_FLAG = 0x01,		//Copy user data memory to own buffer
	K15_ASYNC_OPERATION_CLEAR_AFTER_PROCESS = 0x02		//Automatically clear the async operation memory after it has been processed
};
/*********************************************************************************/
struct K15_AsyncOperation
{
	uint32 flags;
	K15_AsyncStatus asyncStatus;
	K15_AsyncCallbackFnc asyncCallback;
	K15_AsyncFunctionFnc asyncFunction;
	void* userData;
};
/*********************************************************************************/
struct K15_AsyncContext
{
	K15_Semaphore* asyncWorkerSynchronizer;
	K15_ThreadFixedBuffer asyncThreads;
	K15_AsyncOperationStretchBuffer asyncOperations;

	K15_Mutex* asyncJobLock;
	K15_MallocFnc mallocFnc;
	K15_FreeFnc freeFnc;
};
/*********************************************************************************/

K15_AsyncContext* K15_CreateAsyncContext(K15_OSContext* p_OSContext, K15_MallocFnc p_MallocFnc = K15_DefaultMallocWrapper, K15_FreeFnc p_FreeFnc = K15_DefaultFreeWrapper);
K15_AsyncOperation* K15_CreateAsyncOperation(K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags);
K15_AsyncOperation* K15_CreateAsyncOperationAndCopyUserData(K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags);
void K15_InitializeAsyncOperation(K15_AsyncOperation* p_AsyncOperation, K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags);
void K15_ClearAsyncOperation(K15_AsyncOperation* p_AsyncOperation);
uint8 K15_AddAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncOperation* p_AsyncOperation);

#endif //_K15_Runtime_Async_Operation_h_