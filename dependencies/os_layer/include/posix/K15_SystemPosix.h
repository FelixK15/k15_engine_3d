#ifndef _K15_OSLayer_System_Posix_h_
#define _K15_OSLAyer_System_Posix_h_

#include "K15_OSLayer_Prerequisites.h"

double K15_PosixGetElapsedSeconds();
void K15_PosixSleep(double p_SleepTimeInSeconds);
char* K15_PosixGetError();

#endif //_K15_OSLAyer_System_Posix_h_