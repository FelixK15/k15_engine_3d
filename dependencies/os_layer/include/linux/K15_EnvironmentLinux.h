#ifndef K15_OSLayer_Environment_Linux_h_
#define K15_OSLayer_Environment_Linux_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_LinuxContext
{

};

//uint8 K15_AndroidInitializeOSLayer();

char* K15_LinuxGetError();
void K15_LinuxSleep(float p_SleepTimeInSeconds);
float K15_LinuxGetTicks();


#endif //K15_OSLayer_Environment_Linux_h_