#include "K15_OSLayer_Thread.h"

#include "K15_OSLayer_OSContext.h"

/*********************************************************************************/
intern void K15_InternalAddThreadToOSContext(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread)
{
	for (uint32 threadIndex = 0;
		threadIndex < K15_MAX_THREADS;
		++threadIndex)
	{
		if (!p_OSContext->threading.threads[threadIndex])
		{
			p_OSContext->threading.threads[threadIndex] = p_Thread;
			break;
		}
	}
}
/*********************************************************************************/
intern void K15_InternalRemoveThreadFromOSContext(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread)
{
	for (uint32 threadIndex = 0;
		threadIndex < K15_MAX_THREADS;
		++threadIndex)
	{
		if (p_OSContext->threading.threads[threadIndex] == p_Thread)
		{
			p_OSContext->threading.threads[threadIndex] = 0;
			break;
		}
	}
}
/*********************************************************************************/
K15_Thread* K15_CreateThread(K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter, uint32 p_ThreadFlags)
{
	K15_Thread* thread = (K15_Thread*)K15_OS_MALLOC(sizeof(K15_Thread));
	K15_ThreadContext* threadContext = (K15_ThreadContext*)K15_OS_MALLOC(sizeof(K15_ThreadContext));

	threadContext->function = p_ThreadFunction;
	threadContext->parameter = p_ThreadParameter;
	threadContext->name = 0;
	threadContext->nameLength = 0;
	threadContext->startFlags = p_ThreadFlags;
	threadContext->state = K15_THREAD_STATE_NOT_STARTED;

	thread->context = threadContext;
	
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
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_Thread* currentThread = osContext->threading.getCurrentThread();

	return currentThread;
}
/*********************************************************************************/
uint8 K15_FreeThread(K15_Thread* p_Thread)
{
	assert(p_Thread);

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

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
	assert(p_Thread);

	if (p_Thread->context->state != K15_THREAD_STATE_NOT_STARTED)
	{
		return K15_OS_ERROR_THREAD_ALREADY_STARTED;
	}

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	K15_ThreadContext* threadContext = p_Thread->context;

	uint8 result = osContext->threading.createThread(osContext, p_Thread, threadContext->function, threadContext->parameter);
	
	if (result == K15_SUCCESS)
	{
		p_Thread->context->state = K15_THREAD_STATE_RUNNING;

		K15_InternalAddThreadToOSContext(osContext, p_Thread);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_JoinThread(K15_Thread* p_Thread)
{
	assert(p_Thread);

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
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
	
	return result;
}
/*********************************************************************************/
uint8 K15_TryJoinThread(K15_Thread* p_Thread, uint32 p_MilliSeconds)
{
	assert(p_Thread);

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;

	if (threadContext->state != K15_THREAD_STATE_RUNNING)
	{
		return K15_OS_ERROR_THREAD_NOT_JOINABLE;
	}

	uint8 result = osContext->threading.tryJoinThread(osContext, p_Thread, p_MilliSeconds);

	if (result == K15_SUCCESS)
	{
		p_Thread->context->state = K15_THREAD_STATE_JOINED;

		K15_InternalRemoveThreadFromOSContext(osContext, p_Thread);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_InterruptThread(K15_Thread* p_Thread)
{
	assert(p_Thread);

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
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

	return result;
}
/*********************************************************************************/
uint8 K15_SetThreadName(K15_Thread* p_Thread, const char* p_ThreadName)
{
	assert(p_Thread && p_ThreadName);

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
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

	return osContext->threading.setThreadName(p_Thread);
}

/*********************************************************************************/