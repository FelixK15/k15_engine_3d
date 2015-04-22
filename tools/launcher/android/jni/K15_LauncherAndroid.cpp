#include "K15_AndroidGlue.h"

#include <K15_Logging.h>
#include <K15_OSLayer_SystemEvents.h>
#include <android/K15_EnvironmentAndroid.h>

/*********************************************************************************/
JNIEXPORT void JNICALL Java_k15games_engine_launcher_LauncherThread_K15_1InitEngine(JNIEnv* p_JNIEnvironment, jobject p_This)
{
	uint8 result = K15_AndroidInitializeOSLayer(p_JNIEnvironment);
}
/*********************************************************************************/
JNIEXPORT jboolean JNICALL Java_k15games_engine_launcher_LauncherThread_K15_1TickEngine(JNIEnv* p_JNIEnvironment, jobject p_This, jbyteArray p_SystemEventArray)
{
	static uint32 SYSTEM_EVENT_BYTE_SIZE = 12;

	jboolean continueTicking = TRUE;

	jsize byteCount = p_JNIEnvironment->GetArrayLength(p_SystemEventArray);
	jbyte* systemEventBytes = p_JNIEnvironment->GetByteArrayElements(p_SystemEventArray, 0);

	uint32 amountSystemEvents = byteCount / SYSTEM_EVENT_BYTE_SIZE;
	uint32 offset = 0;

	for (uint32 systemEventIndex = 0;
		 systemEventIndex < amountSystemEvents;
		 ++systemEventIndex)
	{
		K15_SystemEvent systemEvent = {};

		memcpy(&systemEvent.event, systemEventBytes + offset, sizeof(uint32));
		memcpy(&systemEvent.eventFlags, systemEventBytes + offset + sizeof(uint32), sizeof(uint32));
		memcpy(&systemEvent.params.key, systemEventBytes + offset + sizeof(uint32) + sizeof(uint32), sizeof(uint32));

		K15_AddSystemEventToQueue(&systemEvent);

		offset += SYSTEM_EVENT_BYTE_SIZE;
	}

	p_JNIEnvironment->ReleaseByteArrayElements(p_SystemEventArray, systemEventBytes, 0);

	K15_SystemEvent systemEvent = {};

	while(K15_GetSystemEventFromQueue(&systemEvent, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
	{
		if (systemEvent.event == K15_APPLICATION_QUIT)
		{
			continueTicking = FALSE;
		}
	}

	return continueTicking;
}
/*********************************************************************************/
JNIEXPORT void JNICALL Java_k15games_engine_launcher_LauncherThread_K15_1ShutdownEngine(JNIEnv* p_JNIEnvironment, jobject p_This)
{
	
}
/*********************************************************************************/