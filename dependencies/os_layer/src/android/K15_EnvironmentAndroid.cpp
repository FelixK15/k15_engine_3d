#include "android/K15_EnvironmentAndroid.h"

#include "posix/K15_ThreadPosix.h"

#include <K15_OSLayer_OSContext.h>
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
intern char* K15_AndroidGetHomeDirectory(JNIEnv* p_JNIEnvironment)
{

}
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_AndroidInitializeOSLayer(JNIEnv* p_JNIEnvironment)
{
	K15_OSLayerContext androidContext;

	androidContext.threading.createThread = K15_PosixCreateThread;
	androidContext.threading.threads = (K15_Thread**)malloc(sizeof(K15_Thread*) * K15_MAX_THREADS);

	memset(androidContext.threading.threads, 0, sizeof(K15_Thread*) * K15_MAX_THREADS);

	androidContext.system.systemIdentifier = OS_ANDROID;
// 	androidContext.system.sleep = K15_LinuxSleep;
// 	androidContext.system.getError = K15_LinuxGetError;
	androidContext.system.homeDir = K15_AndroidGetHomeDirectory(p_JNIEnvironment);

	androidContext.commandLineArgCount = 0;
	androidContext.commandLineArgs = 0;
	androidContext.userData = 0;

	K15_AndroidContext* androidSpecificContext = (K15_AndroidContext*)K15_OS_MALLOC(sizeof(K15_AndroidContext));
	memset(androidSpecificContext, 0, sizeof(K15_AndroidContext));

	if (!androidSpecificContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	androidSpecificContext->jniEnvironment = p_JNIEnvironment;

	K15_LogRegisterLogFnc(K15_AndroidLog, K15_LOG_PRIORITY_DEFAULT);
}
/*********************************************************************************/