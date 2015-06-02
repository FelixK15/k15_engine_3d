#include "K15_FrameCounter.h"

#include <K15_System.h>

/*********************************************************************************/
void K15_StartFrame(K15_FrameCounter* p_FrameCounter)
{
	double startTime = K15_GetElapsedSeconds();
	p_FrameCounter->startFrameTime = startTime;
}
/*********************************************************************************/
void K15_EndFrame(K15_FrameCounter* p_FrameCounter)
{
	double startTime = p_FrameCounter->startFrameTime;
	double endTime = K15_GetElapsedSeconds();
	
	double deltaFrameTime = endTime - startTime;
	double deltaTime = p_FrameCounter->deltaTime + deltaFrameTime;

	unsigned int frameCount = p_FrameCounter->frameCount + 1;
	
	if (deltaTime >= 1.0f)
	{
		deltaTime = 0.f;
		p_FrameCounter->framesPerSecond = frameCount;
		frameCount = 0;
	}

	p_FrameCounter->frameCount = frameCount;
	p_FrameCounter->deltaTime = deltaTime;
}
/*********************************************************************************/