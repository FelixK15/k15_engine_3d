#include "android/K15_EnvironmentAndroid.h"

#include "linux/K15_ThreadLinux.h"
#include "linux/K15_EnvironmentLinux.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_Logging.h>

/*********************************************************************************/
intern void K15_AndroidLogLogcat(const char* p_Message, LogPriority p_Priority)
{
	android_LogPriority prio;

	switch (p_Priority)
	{
	case K15_LOG_PRIORITY_NORMAL:
		{
			prio = ANDROID_LOG_VERBOSE;
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
			prio = ANDROID_LOG_INFO;
			break;
		}

	case K15_LOG_PRIORITY_DEBUG:
		{
			prio = ANDROID_LOG_DEBUG;
			break;
		}
	}

	__android_log_write(prio, "K15_Engine", p_Message);
}
/*********************************************************************************/



/*********************************************************************************/
intern char* K15_AndroidGetHomeDirectory(JNIEnv* p_JNIEnvironment)
{

}
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_AndroidInitializeOSLayer(JNIEnv* p_JNIEnvironment)
{
	K15_OSLayerContext androidContext;

	androidContext.threading.createThread = K15_LinuxCreateThread;
	androidContext.threading.threads = (K15_Thread**)malloc(sizeof(K15_Thread*) * K15_MAX_THREADS);

	memset(androidContext.threading.threads, 0, sizeof(K15_Thread*) * K15_MAX_THREADS);

	androidContext.system.systemIdentifier = OS_ANDROID;
	androidContext.system.sleep = K15_LinuxSleep;
	androidContext.system.getError = K15_LinuxGetError;
	androidContext.system.homeDir = K15_AndroidGetHomeDirectory(p_JNIEnvironment);

	androidContext.commandLineArgCount = 0;
	androidContext.commandLineArgs = 0;
	androidContext.userData = 0;

	K15_LogRegisterLogFnc(K15_AndroidLogLogcat, K15_LOG_PRIORITY_DEFAULT);
}
/*********************************************************************************/