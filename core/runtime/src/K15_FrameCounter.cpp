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
	p_FrameCounter->startFrameTime = K15_GetElapsedSeconds();
}
/*********************************************************************************/
void K15_EndFrame(K15_FrameCounter* p_FrameCounter)
{
	double startTime = p_FrameCounter->startFrameTime;
	double endTime = K15_GetElapsedSeconds();

	if (endTime < startTime)
	{
		return;
	}

	double deltaFrameTime = endTime - startTime;
	double deltaTime = deltaFrameTime;
	double sumDeltaTime = 0.0;

	int sampleIndex = p_FrameCounter->sampleIndex;

	p_FrameCounter->deltaSamples[sampleIndex] = deltaTime;

	for (int deltaIndex = 0;
		deltaIndex < K15_MAX_DELTA_TIME_SAMPLES;
		++deltaIndex)
	{
		sumDeltaTime += p_FrameCounter->deltaSamples[deltaIndex];
	}

	double avgDeltaTime = sumDeltaTime / K15_MAX_DELTA_TIME_SAMPLES;

	sampleIndex = (sampleIndex + 1) % K15_MAX_DELTA_TIME_SAMPLES;

	p_FrameCounter->sampleIndex = sampleIndex;
	p_FrameCounter->FPS = floor(1.0f / avgDeltaTime); 
	p_FrameCounter->avgDeltaTime = avgDeltaTime;
}
/*********************************************************************************/