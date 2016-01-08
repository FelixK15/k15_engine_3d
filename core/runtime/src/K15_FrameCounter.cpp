#include "K15_FrameCounter.h"

#include <K15_Math.h>
#include <K15_System.h>

/*********************************************************************************/
K15_FrameCounter K15_CreateFrameCounter()
{
	K15_FrameCounter frameCounter = {};
	return frameCounter;
}
/*********************************************************************************/
void K15_StartFrame(K15_FrameCounter* p_FrameCounter)
{
	p_FrameCounter->startFrameTime = K15_GetElapsedMilliseconds();
}
/*********************************************************************************/
void K15_EndFrame(K15_FrameCounter* p_FrameCounter)
{
	uint32 startTime = p_FrameCounter->startFrameTime;
	uint32 endTime = K15_GetElapsedMilliseconds();

	if (endTime < startTime)
	{
		return;
	}

	uint32 deltaFrameTime = endTime - startTime;
	uint32 deltaTime = deltaFrameTime;
	uint32 sumDeltaTime = 0;
	uint32 numSamples = p_FrameCounter->numSamples;

	int sampleIndex = p_FrameCounter->sampleIndex;

	p_FrameCounter->deltaSamples[sampleIndex] = deltaTime;

	for (uint32 deltaIndex = 0;
		deltaIndex < numSamples;
		++deltaIndex)
	{
		sumDeltaTime += p_FrameCounter->deltaSamples[deltaIndex];
	}

	float avgDeltaTime = (float)sumDeltaTime / (float)numSamples;

	sampleIndex = (sampleIndex + 1) % K15_MAX_DELTA_TIME_SAMPLES;

	p_FrameCounter->numSamples = numSamples + 1 > K15_MAX_DELTA_TIME_SAMPLES ? K15_MAX_DELTA_TIME_SAMPLES : numSamples+1;
	p_FrameCounter->sampleIndex = sampleIndex;
	p_FrameCounter->FPS = K15_SafeDivide(1000, avgDeltaTime); 
	p_FrameCounter->avgDeltaTime = avgDeltaTime;
}
/*********************************************************************************/