#include "posix/K15_SystemPosix.h"

/*********************************************************************************/
double K15_PosixGetElapsedSeconds()
{
	double seconds = 0.f;
	uint32 milliseconds = 0;
	timespec time = {};

	clock_gettime(CLOCK_MONOTONIC, &time);

	milliseconds = (time.tv_nsec / 1000000) + (time.tv_sec * 1000);
	seconds = (double)milliseconds / 1000.0;
	
	return seconds;
}
/*********************************************************************************/
void K15_PosixSleep(double p_SleepTimeInSeconds)
{
	static double secondsToMilliseconds = 1000.0;
	unsigned long seconds = (unsigned long)(p_SleepTimeInSeconds * secondsToMilliseconds);
	unsigned long microSeconds = seconds * 1000;

	usleep(microSeconds);
}
/*********************************************************************************/
char* K15_PosixGetError()
{
	int error = errno;

	return strerror(error);
}
/*********************************************************************************/