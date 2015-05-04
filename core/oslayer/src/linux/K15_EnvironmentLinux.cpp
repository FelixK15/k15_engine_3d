#include "linux/K15_EnvironmentLinux.h"

#ifdef K15_OS_LINUX

/*********************************************************************************/
char* K15_LinuxGetError()
{
	int errorNumber = errno;

	return strerror(errorNumber);
}
/*********************************************************************************/
void K15_LinuxSleep(float p_SleepTimeInSeconds)
{
	timespec sleepTimeSpec = {};
	long nanoSeconds = (long)(p_SleepTimeInSeconds * 1000.f); // ms
	nanoSeconds *= 1000000;
	sleepTimeSpec.tv_nsec = nanoseconds;

	nanosleep(&sleepTimeSpec, 0);
}
/*********************************************************************************/
float K15_LinuxGetTicks()
{

}
/*********************************************************************************/

#endif //K15_OS_LINUX