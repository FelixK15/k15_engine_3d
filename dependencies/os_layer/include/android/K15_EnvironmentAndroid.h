#ifndef _K15_OSLayer_Environment_Android_h_
#define _K15_OSLayer_Environment_Android_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_AndroidContext
{
	JavaVM* javaVM;
	jobject activityHandle;
	jmethodID pollEventsHandle;

	K15_Thread* mainThread;
};

uint8 K15_AndroidInitializeOSLayer(JNIEnv* p_JNIEnvironment, jobject p_ActivityHandle, int p_WidthInPixels, int p_HeightInPixels, const char* p_AppDirectory);

#endif //_K15_OSLayer_Environment_Android_h_