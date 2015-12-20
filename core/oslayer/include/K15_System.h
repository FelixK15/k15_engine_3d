#ifndef _K15_System_h_
#define _K15_System_h_

#include "K15_Prerequisites.h"

#define K15_ERROR_MESSAGE_LENGTH 256

#ifdef K15_OS_WINDOWS
	#include "win32/K15_OSContextWin32.h"

	#define K15_SleepThreadForSeconds(seconds)				K15_Win32Sleep(seconds)
	#define K15_SleepThreadForMilliseconds(milliseconds)	K15_Win32Sleep(milliseconds)
	#define K15_CopySystemErrorMessageIntoBuffer			K15_Win32GetError
#elif defined K15_OS_ANDROID || defined K15_OS_LINUX
	#include "posix/K15_SystemPosix.h"

	#define K15_SleepThreadForSeconds(seconds)				K15_PosixSleep(seconds)
	#define K15_SleepThreadForMilliseconds(milliseconds)	K15_PosixSleep(milliseconds)
	#define K15_GetSystemErrorMessage						K15_PosixGetError
#endif //K15_OS_WINDOWS

uint32 K15_GetElapsedMilliseconds();
double K15_GetElapsedSeconds();
char* K15_GetSystemErrorMessage();

#endif //_K15_System_h_
