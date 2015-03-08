#include "K15_OSLayer_Thread.h"

#include "K15_OSLayer_OSContext.h"

/*********************************************************************************/
K15_Thread* K15_CreateThread(K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter)
{
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_Thread* thread = (K15_Thread*)malloc(sizeof(K15_Thread));
	K15_ThreadContext* threadContext = (K15_ThreadContext*)malloc(sizeof(K15_ThreadContext));

	threadContext->function = p_ThreadFunction;
	threadContext->parameter = p_ThreadParameter;
	threadContext->name = 0;
	threadContext->nameLength = 0;

	thread->context = threadContext;

	uint8 result = osContext->threading.createThread(osContext, thread, p_ThreadFunction, p_ThreadParameter);

	if (result != K15_SUCCESS)
	{
		free(thread);
		free(threadContext);

		thread = 0;
	}

	for (uint32 threadIndex = 0;
		 threadIndex < K15_MAX_THREADS;
		 ++threadIndex)
	{
		if (!osContext->threading.threads[threadIndex])
		{
			osContext->threading.threads[threadIndex] = thread;
			break;
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
uint8 K15_SetThreadName(K15_Thread* p_Thread, const char* p_ThreadName)
{
	assert(p_Thread && p_ThreadName);

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_ThreadContext* threadContext = p_Thread->context;
	size_t newThreadNameLength = strlen(p_ThreadName);

	if (threadContext->nameLength < newThreadNameLength)
	{
		//delete existing name buffer
		free(threadContext->name);
		threadContext->name = (char*)malloc(newThreadNameLength + 1); //+1 for null terminator
	}

	memcpy(threadContext->name, p_ThreadName, newThreadNameLength);
	threadContext->name[newThreadNameLength] = 0;

	return osContext->threading.setThreadName(p_Thread);
}
/*********************************************************************************/