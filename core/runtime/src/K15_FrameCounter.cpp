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

	double sumDeltaTime = p_FrameCounter->sumDeltaTime + deltaTime;

	if (sumDeltaTime >= 1.0)
	{
		int fpsSampleIndex = p_FrameCounter->numFrameSamples;
		p_FrameCounter->samplesFPS[fpsSampleIndex] = p_FrameCounter->frameCountPerSecond;
		p_FrameCounter->numFrameSamples = p_FrameCounter->numFrameSamples == K15_MAX_DELTA_TIME_SAMPLES ? 0 : p_FrameCounter->numFrameSamples + 1;
		p_FrameCounter->frameCountPerSecond = 0;
		sumDeltaTime = 0.0;
	}

	unsigned int sumFPSSamples = 0;
	for (unsigned int sampleIndex = 0;
		sampleIndex < K15_MAX_DELTA_TIME_SAMPLES;
		++sampleIndex)
	{
		sumFPSSamples += p_FrameCounter->samplesFPS[sampleIndex];
	}

	p_FrameCounter->sumDeltaTime = sumDeltaTime;
	p_FrameCounter->FPS = K15_SafeDivide(sumFPSSamples, K15_MAX_DELTA_TIME_SAMPLES);
	p_FrameCounter->frameCount += 1;
	p_FrameCounter->frameCountPerSecond += 1;
}
/*********************************************************************************/