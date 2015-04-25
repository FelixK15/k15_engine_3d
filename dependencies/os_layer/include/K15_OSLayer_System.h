#ifndef _K15_OSLayer_System_h_
#define _K15_OSLayer_System_h_

#include "K15_OSLayer_Prerequisites.h"

void K15_SleepThreadForSeconds(double p_Seconds);
void K15_SleepThreadForMilliseconds(uint32 p_Milliseconds);

double K15_GetElapsedSeconds();

char* K15_GetSystemErrorMessage();

#endif //_K15_OSLayer_System_h_