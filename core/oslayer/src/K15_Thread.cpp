#include "K15_Thread.h"

#include "K15_DefaultCLibraries.h"

#include "K15_OSContext.h"
#include "K15_System.h"

#include "generated/K15_ThreadStretchBuffer.cpp"
#include "generated/K15_ThreadFixedBuffer.cpp"

#include <K15_Logging.h>

/*********************************************************************************/
intern void K15_InternalAddThreadToOSContext(K15_OSContext* p_OSContext, K15_Thread* p_Thread)
{
	K15_ThreadStretchBuffer* threadBuffer = &p_OSContext->threading.threads;

	K15_PushThreadStretchBufferElement(threadBuffer, p_Thread);
}
/*********************************************************************************/
intern void K15_InternalRemoveThreadFromOSContext(K15_OSContext* p_OSContext, K15_Thread* p_Thread)
{
	K15_ThreadStretchBuffer* threadBuffer = &p_OSContext->threading.threads;

	K15_PopThreadStretchBufferElement(threadBuffer, p_Thread);
}
/*********************************************************************************/
K15_Thread* K15_CreateThread(K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter, uint32 p_ThreadFlags)
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_Thread* thread = (K15_Thread*)K15_OS_MALLOC(sizeof(K15_Thread));
	K15_ThreadContext* threadContext = (K15_ThreadContext*)K15_OS_MALLOC(sizeof(K15_ThreadContext));

	threadContext->function = p_ThreadFunction;
	threadContext->parameter = p_ThreadParameter;
	threadContext->name = 0;
	threadContext->nameLength = 0;
	threadContext->startFlags = p_ThreadFlags;
	threadContext->state = K15_THREAD_STATE_NOT_STARTED;
	threadContext->threadAffintyMask = 0xffffffff;

	thread->context = threadContext;
	
	uint8 result = osContext->threading.createThread(osContext, thread, threadContext->function, threadContext->parameter, p_ThreadFlags);

	//start thread
	if ((p_ThreadFlags & K15_THREAD_START_FLAG) > 0)
	{
		uint8 result = K15_StartThread(thread);

		if (result != K15_SUCCESS)
		{
			K15_OS_FREE(thread);
			K15_OS_FREE(threadContext);

			thread = 0;
		}
	}

	return thread;
}
/*********************************************************************************/
K15_Thread* K15_GetCurrentThread()
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_Thread* currentThread = osContext->threading.getCurrentThread();

	return currentThread;
}
/*********************************************************************************/
uint8 K15_FreeThread(K15_Thread* p_Thread)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");

	K15_OSContext* osContext = K15_GetOSLayerContext();

	if (p_Thread->context->state == K15_THREAD_STATE_RUNNING)
	{
		if (K15_TryJoinThread(p_Thread, 0) != K15_SUCCESS)
		{
			K15_InterruptThread(p_Thread);
		}
	}

	uint8 result = osContext->threading.freeThread(osContext, p_Thread);

	K15_OS_FREE(p_Thread);
	K15_OS_FREE(p_Thread->context);

	return result;
}
/*********************************************************************************/
uint8 K15_StartThread(K15_Thread* p_Thread)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");

	if (p_Thread->context->state != K15_THREAD_STATE_NOT_STARTED)
	{
		return K15_OS_ERROR_THREAD_ALREADY_STARTED;
	}

	K15_OSContext* osContext = K15_GetOSLayerContext();

	K15_ThreadContext* threadContext = p_Thread->context;

	uint8 result = osContext->threading.startThread(osContext, p_Thread);
	
	if (result == K15_SUCCESS)
	{
		p_Thread->context->state = K15_THREAD_STATE_RUNNING;

		K15_InternalAddThreadToOSContext(osContext, p_Thread);
	}
	else if (result == K15_OS_ERROR_SYSTEM)
	{
		char* error = K15_GetSystemErrorMessage();
		K15_LOG_ERROR_MESSAGE("K15_StartThread (System Error): %s", error);
		free(error);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_JoinThread(K15_Thread* p_Thread)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");

	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;

	if (threadContext->state != K15_THREAD_STATE_RUNNING)
	{
		return K15_OS_ERROR_THREAD_NOT_JOINABLE;
	}

	uint8 result = osContext->threading.joinThread(osContext, p_Thread);

	if (result == K15_SUCCESS)
	{
		p_Thread->context->state = K15_THREAD_STATE_JOINED;

		K15_InternalRemoveThreadFromOSContext(osContext, p_Thread);
	}
	else if (result == K15_OS_ERROR_SYSTEM)
	{
		char* error = K15_GetSystemErrorMessage();
		K15_LOG_ERROR_MESSAGE("K15_JoinThread (System Error): %s", error);
		free(error);
	}
	
	return result;
}
/*********************************************************************************/
uint8 K15_TryJoinThread(K15_Thread* p_Thread, uint32 p_MilliSeconds)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");

	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;

	if (threadContext->state != K15_THREAD_STATE_RUNNING && threadContext->state != K15_THREAD_STATE_FINISHED)
	{
		return K15_OS_ERROR_THREAD_NOT_JOINABLE;
	}

	uint8 result = osContext->threading.tryJoinThread(osContext, p_Thread, p_MilliSeconds);

	if (result == K15_SUCCESS)
	{
		p_Thread->context->state = K15_THREAD_STATE_JOINED;

		K15_InternalRemoveThreadFromOSContext(osContext, p_Thread);
	}
	else if (result == K15_OS_ERROR_SYSTEM)
	{
		char* error = K15_GetSystemErrorMessage();
		K15_LOG_ERROR_MESSAGE("K15_TryJoinThread (System Error): %s", error);
		free(error);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_InterruptThread(K15_Thread* p_Thread)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");

	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;

	if (threadContext->state != K15_THREAD_STATE_RUNNING)
	{
		return K15_OS_ERROR_THREAD_NOT_JOINABLE;
	}

	uint8 result = osContext->threading.interruptThread(osContext, p_Thread);

	if (result == K15_SUCCESS)
	{
		p_Thread->context->state = K15_THREAD_STATE_INTERRUPTED;

		K15_InternalRemoveThreadFromOSContext(osContext, p_Thread);
	}
	else if (result == K15_OS_ERROR_SYSTEM)
	{
		char* error = K15_GetSystemErrorMessage();
		K15_LOG_ERROR_MESSAGE("K15_InterruptThread (System Error): %s", error);
		free(error);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_SetThreadAffinityMask(K15_Thread* p_Thread, uint64 p_AffintyMask)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");

	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;

	uint8 result = osContext->threading.setThreadAffinityMask(osContext, p_Thread, p_AffintyMask);

	if (result == K15_OS_ERROR_SYSTEM)
	{
		K15_LOG_ERROR_MESSAGE("K15_SetThreadAffinityMask (System Error): %s", K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
	}

	return result;
}
/*********************************************************************************/
uint8 K15_SetThreadName(K15_Thread* p_Thread, const char* p_ThreadName)
{
	K15_ASSERT_TEXT(p_Thread, "Thread is NULL");
	K15_ASSERT_TEXT(p_ThreadName, "Thread name is NULL");

	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;
	size_t newThreadNameLength = strlen(p_ThreadName);

	if (threadContext->nameLength < newThreadNameLength)
	{
		//delete existing name buffer
		K15_OS_FREE(threadContext->name);
		threadContext->name = (char*)K15_OS_MALLOC(newThreadNameLength + 1); //+1 for null terminator
	}

	memcpy(threadContext->name, p_ThreadName, newThreadNameLength);
	threadContext->name[newThreadNameLength] = 0;
	threadContext->nameLength = (uint32)newThreadNameLength;

	return osContext->threading.setThreadName(p_Thread);
}
/*********************************************************************************/