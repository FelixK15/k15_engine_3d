#ifndef _K15_Runtime_Async_Operation_h_
#define _K15_Runtime_Async_Operation_h_

#include "K15_RuntimePrerequisites.h"

#include "generated/K15_ThreadFixedBuffer.h"
#include "generated/K15_AsyncOperationStretchBuffer.h"
#include "generated/K15_AsyncOperationMemoryPool.h"

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
	K15_ASYNC_OPERATION_CLEAR_AFTER_PROCESS_FLAG	= 0x01,		//Automatically clear the async operation memory after it has been processed
	K15_ASYNC_OPERATION_FIRE_AND_FORGET_FLAG		= 0x02,		//Async Context cleans up automatically. If you don't care about the async operation after it has been issued, use this
	K15_ASYNC_OPERATION_USER_DATA_COPY_FLAG			= 0x04
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
	K15_AsyncOperationMemoryPool asyncMemoryPool;

	K15_Mutex* asyncJobLock;
	K15_CustomMemoryAllocator memoryAllocator;

	uint32 numAsyncOperations;
};
/*********************************************************************************/

K15_AsyncContext* K15_CreateAsyncContext(K15_OSContext* p_OSContext);
K15_AsyncContext* K15_CreateAsyncContextWithCustomAllocator(K15_OSContext* p_OSContext, K15_CustomMemoryAllocator p_CustomMemoryAllocator);
K15_AsyncOperation* K15_CreateSimpleAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncFunctionFnc p_AsyncFunction, uint32 p_Flags);
K15_AsyncOperation* K15_CreateAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags);
K15_AsyncOperation* K15_CreateAndIssueAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags);
void K15_RemoveAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncOperation* p_AsyncOperation);
uint8 K15_IssueAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncOperation* p_AsyncOperation);
void K15_InitializeAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncOperation* p_AsyncOperation, K15_AsyncFunctionFnc p_AsyncFunction, K15_AsyncCallbackFnc p_AsyncCallback, void* p_UserData, uint32 p_UserDataSize, uint32 p_Flags);
uint8 K15_AddAsyncOperation(K15_AsyncContext* p_AsyncContext, K15_AsyncOperation* p_AsyncOperation);
bool8 K15_HasAsyncOperationsLeft(K15_AsyncContext* p_AsyncContext);

#endif //_K15_Runtime_Async_Operation_h_