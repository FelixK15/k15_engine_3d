#include "K15_System.h"

#include "K15_OSContext.h"

/*********************************************************************************/
double K15_GetElapsedSeconds()
{
	K15_OSContext* osContext = K15_GetOSLayerContext();

	double seconds = osContext->system.getElapsedSeconds();
	seconds -= osContext->timeStarted;

	return seconds;
}
/*********************************************************************************/