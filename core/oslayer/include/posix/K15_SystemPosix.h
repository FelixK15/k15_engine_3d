#ifndef _K15_OSLayer_System_Posix_h_
#define _K15_OSLAyer_System_Posix_h_

#include "K15_OSLayer_Prerequisites.h"

uint32 K15_PosixGetElapsedMilliseconds();
void K15_PosixSleep(uint32 p_SleepTimeInMilliseconds);
char* K15_PosixGetError();

#endif //_K15_OSLAyer_System_Posix_h_