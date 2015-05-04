#ifndef _K15_OSLayer_Environment_Android_h_
#define _K15_OSLayer_Environment_Android_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_AndroidContext
{
	JavaVM* javaVM;
	JNIEnv* jniEnv;
	jobject engineThreadObjectHandle;
	jobject surfaceHandle;
	jmethodID pollEventsHandle;
};

uint8 K15_AndroidInitializeOSLayer(JNIEnv* p_JNIEnvironment, jobject p_EngineThreadHandle, int p_WidthInPixels, int p_HeightInPixels, jobject p_SurfaceHandle, const char* p_AppDirectory);

#endif //_K15_OSLayer_Environment_Android_h_