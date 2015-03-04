#include "linux/K15_ThreadLinux.h"

#ifdef K15_OS_LINUX

#include "K15_OSLayer_Thread.h"

/*********************************************************************************/
intern int K15_LinuxThreadWrapper(void* p_Parameter)
{
	K15_ThreadFnc threadFunction = 0;
	void* threadParameter = 0;
	int result = 0;

	memcpy(&threadFunction, p_Parameter, K15_PTR_SIZE);
	memcpy(&threadParameter, ((byte*)p_Parameter + K15_PTR_SIZE), K15_PTR_SIZE);

	uint8 threadFunctionResult = threadFunction(threadParameter);

	free(threadParameter);

	if (threadFunctionResult != K15_SUCCESS
		&& threadFunctionResult == K15_ERROR_SYSTEM)
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
uint8 K15_LinuxCreateThread(K15_OSLayerContext* p_OSContext, K15_Thread* p_Thread, K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter)
{
	pthread_t threadIdentifier = 0;

	byte* parameterBuffer = malloc(K15_PTR_SIZE * 2);

	memcpy(parameterBuffer, &p_ThreadFunction, K15_PTR_SIZE);
	memcpy(parameterBuffer + K15_PTR_SIZE, &p_ThreadParameter, K15_PTR_SIZE);

	if (pthread_create(&threadIdentifier, NULL, K15_LinuxThreadWrapper, (void*)parameterBuffer) != 0)
	{
		return K15_ERROR_SYSTEM;
	}

	K15_LinuxThread* linuxThread = (K15_LinuxThread*)malloc(sizeof(K15_LinuxThread));
	linuxThread->threadIdentifier = threadIdentifier;

	p_Thread->userData = (void*)linuxThread;

	return K15_SUCCESS;
}
/*********************************************************************************/

#endif //K15_OS_LINUX