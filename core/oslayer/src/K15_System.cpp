#include "K15_System.h"

#include "K15_OSContext.h"

/*********************************************************************************/
uint32 K15_GetElapsedMilliseconds()
{
	K15_OSContext* osContext = K15_GetOSLayerContext();

	uint32 milliseconds = osContext->system.getElapsedMilliseconds();
	milliseconds -= osContext->timeStartedInMilliseconds;

	return milliseconds;
}
/*********************************************************************************/
double K15_GetElapsedSeconds()
{
	uint32 elapesMilliseconds = K15_GetElapsedMilliseconds();
	return ((double)elapesMilliseconds / 1000.0);
}
/*********************************************************************************/
char* K15_GetSystemErrorMessage()
{
	char* messageBuffer = (char*)K15_OS_MALLOC(K15_ERROR_MESSAGE_LENGTH);
	
	return K15_CopySystemErrorMessageIntoBuffer(messageBuffer);
}
/*********************************************************************************/