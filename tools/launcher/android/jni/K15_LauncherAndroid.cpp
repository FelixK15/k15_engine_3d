#include <jni.h>

#include <android/K15_EnvironmentAndroid.h>

#include <K15_RenderContext.h>

#include <K15_OSLayer_SystemEvents.h>
#include <K15_OSLayer_System.h>
#include <K15_OSLayer_Thread.h>
#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <K15_Logging.h>


#ifndef _Included_k15games_engine_launcher_LauncherMainActivity
#define _Included_k15games_engine_launcher_LauncherMainActivity
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************/
JNIEXPORT jboolean JNICALL Java_k15games_engine_launcher_EngineMainThread_K15_1TryInitializeEngine
  (JNIEnv *jniEnv, jobject thisObj, jint width, jint height, jstring appDir, jobject surface)
{
	const char* appDirAsChar = jniEnv->GetStringUTFChars(appDir, 0);
	
	uint8 result = K15_AndroidInitializeOSLayer(jniEnv, thisObj, width, height, surface, appDirAsChar);

	jniEnv->ReleaseStringUTFChars(appDir, appDirAsChar);

	return result == K15_SUCCESS ? TRUE : FALSE;
}
/*********************************************************************************/
JNIEXPORT void JNICALL Java_k15games_engine_launcher_EngineMainThread_K15_1StartEngine
  (JNIEnv *jniEnv, jobject thisObj)
{
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();
	K15_AndroidContext* androidContext = (K15_AndroidContext*)osContext->userData;
	K15_RenderContext* renderContext = K15_CreateRenderContext(osContext);
	K15_RenderCommandBuffer* commandBuffer = K15_CreateRenderCommandBuffer(renderContext);

	K15_SystemEvent systemEvent;

	uint8 engineRunning = TRUE;
	double now = 0.0;
	double then = K15_GetElapsedSeconds();
	double delta = 0.0;
	double targetFrame = 0.016;

	while(engineRunning)
	{
		K15_PumpSystemEvents();

		now = K15_GetElapsedSeconds();

		while (K15_GetSystemEventFromQueue(&systemEvent, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
			if (systemEvent.event == K15_APPLICATION_QUIT)
			{
				engineRunning = FALSE;
			}
		}

		delta = now - then;

		if (delta < targetFrame)
		{
			K15_SleepThreadForSeconds(targetFrame - delta);
		}

		K15_ProcessDispatchedRenderCommandBuffers(renderContext);

		then = now;
	}
}
/*********************************************************************************/
JNIEXPORT void JNICALL Java_k15games_engine_launcher_EngineMainThread_K15_1ShutdownEngine
  (JNIEnv *jniEnv, jobject thisObj)
{

}
/*********************************************************************************/
JNIEXPORT jcharArray JNICALL Java_k15games_engine_launcher_EngineMainThread_K15_1GetErrorMessage
  (JNIEnv *jniEnv, jobject thisObj)
{

}
/*********************************************************************************/

#ifdef __cplusplus
}
#endif
#endif
