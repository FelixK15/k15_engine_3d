#include "posix/K15_SystemPosix.h"

/*********************************************************************************/
uint32 K15_PosixGetElapsedSeconds()
{
	uint32 milliseconds = 0;
	timespec time = {};

	clock_gettime(CLOCK_MONOTONIC, &time);

	milliseconds = (time.tv_nsec / 1000000) + (time.tv_sec * 1000);
	
	return milliseconds;
}
/*********************************************************************************/
void K15_PosixSleep(uint32 p_SleepTimeInMilliseconds)
{
	unsigned long microSeconds = p_SleepTimeInMilliseconds * 1000;

	usleep(microSeconds);
}
/*********************************************************************************/
char* K15_PosixGetError()
{
	int error = errno;

	return strerror(error);
}
/*********************************************************************************/