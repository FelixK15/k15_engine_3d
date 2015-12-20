#ifndef _K15_Runtime_Frame_Counter_h_
#define _K15_Runtime_Frame_Counter_h_

#include "K15_RuntimePrerequisites.h"

struct K15_FrameCounter
{
	int sampleIndex;
	float FPS;
	float avgDeltaTime;
	uint32 deltaSamples[K15_MAX_DELTA_TIME_SAMPLES];
	uint32 deltaTime;
	uint32 startFrameTime;
	uint32 endFrameTime;
	uint32 numSamples;
};

K15_FrameCounter K15_CreateFrameCounter();

void K15_StartFrame(K15_FrameCounter* p_FrameCounter);
void K15_EndFrame(K15_FrameCounter* p_FrameCounter);

#endif //_K15_Runtime_Frame_Counter_h_