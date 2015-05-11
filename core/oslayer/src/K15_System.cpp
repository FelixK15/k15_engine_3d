#include "K15_System.h"

#include "K15_OSContext.h"

/*********************************************************************************/
void K15_SleepThreadForSeconds(double p_Seconds)
{
	K15Context* osContext = K15_GetOSLayerContext();

	osContext->system.sleep(p_Seconds);
}
/*********************************************************************************/
void K15_SleepThreadForMilliseconds(uint32 p_Milliseconds)
{
	K15Context* osContext = K15_GetOSLayerContext();

	double seconds = (double)p_Milliseconds / 1000.f;

	osContext->system.sleep(seconds);
}
/*********************************************************************************/
char* K15_GetSystemErrorMessage()
{
	K15Context* osContext = K15_GetOSLayerContext();

	return osContext->system.getError();
}
/*********************************************************************************/
double K15_GetElapsedSeconds()
{
	K15Context* osContext = K15_GetOSLayerContext();

	double seconds = osContext->system.getElapsedSeconds();
	seconds -= osContext->timeStarted;

	return seconds;
}
/*********************************************************************************/