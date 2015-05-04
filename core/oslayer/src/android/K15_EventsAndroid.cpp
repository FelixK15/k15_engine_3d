#include "android/K15_EventsAndroid.h"
#include "android/K15_EnvironmentAndroid.h"
#include "posix/K15_ThreadPosix.h"

#include "K15_OSLayer_Thread.h"
#include "K15_OSLayer_OSContext.h"
#include "K15_OSLayer_SystemEvents.h"

#include <K15_Logging.h>

/*********************************************************************************/
uint8 K15_AndroidPumpSystemEvents(K15_OSLayerContext* p_OSContext)
{
	K15_AndroidContext* androidContext = (K15_AndroidContext*)p_OSContext->userData;
	//K15_PosixThread* posixMainThread = (K15_PosixThread*)androidContext->mainThread->userData;
	JNIEnv* jniEnv = androidContext->jniEnv;
	jobject engineThreadObject = androidContext->engineThreadObjectHandle;
	jmethodID pollSystemEventHandle = androidContext->pollEventsHandle;

 	jbyteArray systemEventByteArray = 0;
	
	K15_JNI_CALL(jniEnv, systemEventByteArray = (jbyteArray)jniEnv->CallObjectMethod(engineThreadObject, pollSystemEventHandle));

	if (systemEventByteArray)
	{
		jsize systemEventByteSize = 0;
		
		K15_JNI_CALL(jniEnv, systemEventByteSize = jniEnv->GetArrayLength(systemEventByteArray));

		jbyte* systemEventsByteData = (jbyte*)alloca(systemEventByteSize);

		K15_JNI_CALL(jniEnv, jniEnv->GetByteArrayRegion(systemEventByteArray, 0, systemEventByteSize, systemEventsByteData));
 
		uint32 systemEventCount = systemEventByteSize / sizeof(K15_SystemEvent);
		uint32 offset = 0;

		for (uint32 systemEventIndex = 0;
			 systemEventIndex < systemEventCount;
			 ++systemEventIndex)
		{
			K15_SystemEvent currentEvent = {};

			memcpy(&currentEvent.event, systemEventsByteData + offset, sizeof(uint32));
			offset += sizeof(uint32);

			memcpy(&currentEvent.eventFlags, systemEventsByteData + offset, sizeof(uint32));
			offset += sizeof(uint32);

			memcpy(&currentEvent.params.key, systemEventsByteData + offset, sizeof(uint32));
			offset += sizeof(uint32);

			K15_AddSystemEventToQueue(&currentEvent);
		}
	}

	K15_JNI_CALL(jniEnv, jniEnv->DeleteLocalRef(systemEventByteArray));

	return K15_SUCCESS;
}
/*********************************************************************************/