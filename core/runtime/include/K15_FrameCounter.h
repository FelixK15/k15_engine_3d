#ifndef _K15_Runtime_Frame_Counter_h_
#define _K15_Runtime_Frame_Counter_h_

struct K15_FrameCounter
{
	unsigned int framesPerSecond;
	unsigned int frameCount;
	unsigned int seconds;
	double deltaTime;
	double startFrameTime;
	double endFrameTime;
};

void K15_StartFrame(K15_FrameCounter* p_FrameCounter);
void K15_EndFrame(K15_FrameCounter* p_FrameCounter);

#endif //_K15_Runtime_Frame_Counter_h_