#include "android/K15_EnvironmentAndroid.h"
#include "android/K15_EventsAndroid.h"
#include "android/K15_ThreadAndroid.h"
#include "android/K15_WindowAndroid.h"

#include "posix/K15_SystemPosix.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <K15_OSLayer_Thread.h>
#include <K15_Logging.h>

/*********************************************************************************/
intern void K15_AndroidLog(const char* p_LogMessage, LogPriority p_LogPriority)
{
	int prio = ANDROID_LOG_INFO;

	switch (p_LogPriority)
	{
	case K15_LOG_PRIORITY_NORMAL:
		{
			prio = ANDROID_LOG_INFO;
			break;
		}

	case K15_LOG_PRIORITY_DEBUG:
		{
			prio = ANDROID_LOG_DEBUG;
			break;
		}

	case K15_LOG_PRIORITY_ERROR:
		{
			prio = ANDROID_LOG_ERROR;
			break;
		}

	case K15_LOG_PRIORITY_WARNING:
		{
			prio = ANDROID_LOG_WARN;
			break;
		}

	case K15_LOG_PRIORITY_SUCCESS:
		{
			prio = ANDROID_LOG_VERBOSE;
			break;
		}
	}

	__android_log_print(prio, "K15_Engine", p_LogMessage);
}
/*********************************************************************************/

/*********************************************************************************/
intern inline K15_Window* K15_InternalCreateWindow(int p_HeightInPixels, int p_WidthInPixels)
{
	K15_Window* window = K15_CreateWindow(0, 0);

	window->height = p_HeightInPixels;
	window->width = p_WidthInPixels;
	window->flags = 0;
	window->title = 0;
	window->monitorIndex = 0;

#ifdef K15_DEBUG_MRT
	window->debugging.assignedThread = K15_GetCurrentThread();
#endif // K15_DEBUG_MRT

	return window;
}
/*********************************************************************************/
uint8 K15_AndroidInitializeOSLayer(JNIEnv* p_JNIEnvironment, jobject p_EngineThreadHandle, int p_WidthInPixels, int p_HeightInPixels, jobject p_SurfaceHandle, const char* p_AppDirectory)
{
	K15_LogRegisterLogFnc(K15_AndroidLog, K15_LOG_PRIORITY_DEFAULT);

	K15_OSLayerContext androidContext;

	//allocate memory for homedir char*
	uint32 homeDirLength = strlen(p_AppDirectory);
	char* homeDir = (char*)malloc(homeDirLength + 1); //+1 for 0 terminator
	homeDir[homeDirLength] = 0;

	//copy homedir
	memcpy(homeDir, p_AppDirectory, homeDirLength);

	timespec timeInitialized = {};
	clock_gettime(CLOCK_MONOTONIC, &timeInitialized);

	uint32 milliseconds = (timeInitialized.tv_nsec / 1000000) + (timeInitialized.tv_sec * 1000);
	double timeStarted = (double)milliseconds / 1000.f;

	androidContext.timeStarted = timeStarted;

	//Set Function pointer
	androidContext.system.systemIdentifier = OS_ANDROID;
 	androidContext.system.sleep = K15_PosixSleep;
 	androidContext.system.getError = K15_PosixGetError;
	androidContext.system.homeDir = homeDir;
	androidContext.system.getElapsedSeconds = K15_PosixGetElapsedSeconds;
	androidContext.events.pumpSystemEvents = K15_AndroidPumpSystemEvents;
	androidContext.threading.createThread = K15_PosixCreateThread;
	androidContext.threading.getCurrentThread = K15_PosixGetCurrentThread;
	androidContext.threading.setThreadName = K15_PosixSetThreadName;
	androidContext.threading.threads = (K15_Thread**)malloc(sizeof(K15_Thread*) * K15_MAX_THREADS);
	androidContext.window.createWindow = K15_AndroidCreateWindow;
	memset(androidContext.threading.threads, 0, sizeof(K15_Thread*) * K15_MAX_THREADS);
	androidContext.commandLineArgCount = 0;
	androidContext.commandLineArgs = 0;
	androidContext.userData = 0;

	K15_AndroidContext* androidSpecificContext = (K15_AndroidContext*)K15_OS_MALLOC(sizeof(K15_AndroidContext));
	memset(androidSpecificContext, 0, sizeof(K15_AndroidContext));

	if (!androidSpecificContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	jobject engineThreadHandle = 0;
	jobject surfaceHandle = 0;
	jclass engineThreadClassHandle = 0;
	jmethodID pollEventsFunctionHandle = 0;

	K15_JNI_CALL(p_JNIEnvironment, engineThreadHandle = p_JNIEnvironment->NewGlobalRef(p_EngineThreadHandle));
	K15_JNI_CALL(p_JNIEnvironment, surfaceHandle = p_JNIEnvironment->NewGlobalRef(p_SurfaceHandle));
	K15_JNI_CALL(p_JNIEnvironment, engineThreadClassHandle = p_JNIEnvironment->GetObjectClass(engineThreadHandle));
	K15_JNI_CALL(p_JNIEnvironment, pollEventsFunctionHandle = p_JNIEnvironment->GetMethodID(engineThreadClassHandle, "getSystemEventsAsByteBuffer", "()[B"));

	androidSpecificContext->surfaceHandle = surfaceHandle;
	//androidSpecificContext->nativeWindow = nativeWindow;
	androidSpecificContext->javaVM = 0;
	androidSpecificContext->jniEnv = p_JNIEnvironment;

	K15_JNI_CALL(p_JNIEnvironment, p_JNIEnvironment->GetJavaVM(&androidSpecificContext->javaVM));
	
	androidSpecificContext->pollEventsHandle = pollEventsFunctionHandle;
	androidSpecificContext->engineThreadObjectHandle = engineThreadHandle;
	//androidSpecificContext->surfaceHandle = p_JNIEnvironment->NewGlobalRef(p_SurfaceHandle);

	androidContext.userData = (void*)androidSpecificContext;
	K15_InternalSetOSLayerContext(androidContext);

	androidContext.window.window = K15_InternalCreateWindow(p_HeightInPixels, p_WidthInPixels);

	return K15_SUCCESS;
}
/*********************************************************************************/