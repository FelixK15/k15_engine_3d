#ifndef _K15_Prerequisites_h_
#define _K15_Prerequisites_h_

#include "K15_Common.h"
#include "K15_Config.h"
#include "K15_ErrorCodes.h"

#define K15_DEFAULT_THREAD_SIZE 8
#define K15_DEFAULT_DYNAMIC_LIBRARY_SIZE 8

#define K15_MAIN_THREAD 0

#define K15_FUNCTION_DEBUG_PRINTF(func) printf("Before "#func"\n"); func; printf("After "#func"\n")

#define K15_FUNCTION_VARIABLE(returntype, type, arguments)		typedef returntype(*type) arguments;	\
	static returntype type##_stubFnc arguments {	\
	K15_LOG_ERROR_MESSAGE("Function \"%s\" not supported\\implemented on this platform.", #type);	\
	return(returntype)(0);	\
																};


#ifdef K15_OS_ANDROID
#ifdef K15_CHECK_JNI_CALLS
#define K15_JNI_CALL(jniEnv, call) {call; jthrowable ex = jniEnv->ExceptionOccurred(); if(ex){jniEnv->ExceptionDescribe(); jniEnv->ExceptionClear();}}
#else
#define K15_JNI_CALL(jniEnv, call) {call;}
#endif //K15_CHECK_JNI_CALLS
#endif //K15_OS_ANDROID
// 
// #if defined K15_OS_WINDOWS
// #define _WINSOCKAPI_				// stops windows.h including winsock.h
// #define DIRECTINPUT_VERSION 0x0800	// use directinput 8
//
// 
// //#define WIN32_LEAN_AND_MEAN
// #include <windows.h>
// #include <windowsx.h>
// #include <Wbemidl.h>
// #include <oleauto.h>
// #include <winnt.h>
// #include <xinput.h>
// #include <dinput.h>
// #include <dsound.h>
// #include <stdlib.h>
// #include <io.h>
// #include <Dbt.h>
// #endif // K15_OS_WINDOWS

// #ifdef K15_OS_ANDROID && defined K15_INCLUDE_SYSTEM_HEADER
// #include <android/log.h>
// #include <android/native_window_jni.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <time.h>
// #include <jni.h>
// #include <pthread.h>
// #include <alloca.h>
// #include <semaphore.h>	
// #include <errno.h>
// #include <stdlib.h>
// #include <ctype.h>
// #endif //K15_OS_ANDROID

// #ifdef K15_OS_LINUX && defined K15_INCLUDE_SYSTEM_HEADER
// #include <pthread.h>
// #include <time.h>
// #endif //K15_OS_LINUX


/*********************************************************************************/
enum K15_StretchBufferFlags
{
	K15_USE_EXTERNAL_BUFFER = 0x02
};
/*********************************************************************************/

// forward declaration
struct K15_Window;
struct K15_Thread;
struct K15_DynamicLibrary;
struct K15_Mutex;
struct K15_FileWatchEntry;
struct K15_DirectoryWatchEntry;
struct K15_ThreadContext;
struct K15_Semaphore;
struct K15_OSContext;
struct K15_CustomMemoryAllocator;

typedef uint8 (*K15_ThreadFnc)(void* p_ThreadParameter);

#endif //_K15_Prerequisites_h_
