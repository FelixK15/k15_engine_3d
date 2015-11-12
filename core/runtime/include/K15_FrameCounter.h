#ifndef _K15_Runtime_Frame_Counter_h_
#define _K15_Runtime_Frame_Counter_h_

#include "K15_RuntimePrerequisites.h"

struct K15_FrameCounter
{
	unsigned int FPS;
	unsigned int samplesFPS[K15_MAX_DELTA_TIME_SAMPLES];
	unsigned int frameCount;
	unsigned int frameCountPerSecond;
	unsigned int numFrameSamples;
	unsigned int frameSampleIndex;
	double sumDeltaTime;
	double deltaTime;
	double startFrameTime;
	double endFrameTime;
};

K15_FrameCounter K15_CreateFrameCounter();

void K15_StartFrame(K15_FrameCounter* p_FrameCounter);
void K15_EndFrame(K15_FrameCounter* p_FrameCounter);

#endif //_K15_Runtime_Frame_Counter_h_