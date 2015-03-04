#ifndef _K15_OSLayer_Environment_Android_h_
#define _K15_OSLayer_Environment_Android_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_AndroidContext
{
	JNIEnv* jniEnvironment;
};

uint8 K15_AndroidInitializeOSLayer(JNIEnv* p_JNIEnvironment);

#endif //_K15_OSLayer_Environment_Android_h_