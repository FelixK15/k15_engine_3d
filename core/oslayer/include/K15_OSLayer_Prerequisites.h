#ifndef _K15_OSLayer_Prerequisites_h_
#define _K15_OSLayer_Prerequisites_h_

#define _CRT_SECURE_NO_WARNINGS

#include "K15_OSLayer_ErrorCodes.h"
#include "K15_OSLayer_Config.h"

#include <K15_Logging.h>

#include <assert.h>
#include <memory.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#ifdef __ANDROID__
	#define K15_OS_ANDROID
#elif defined _WIN32
	#define K15_OS_WINDOWS
#elif defined __linux__
	#define K15_OS_LINUX
#elif defined __APPLE__
	#define K15_OS_APPLE
#endif //_WIN32

#ifdef K15_OS_ANDROID
#define K15_PLATFORM_MOBILE
#endif //K15_OS_ANDROID

#if defined (K15_OS_WINDOWS)
	#ifdef _WIN64
		#define K15_64_BIT
	#endif //_WIN64
#else
	#include <limits.h>
	#ifdef __LP64__
		#define K15_64_BIT
	#endif //__LP64__
#endif //K15_OS_WINDOWS

#ifdef __GNUC__
//http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
# define K15_GCC_VERSION (__GNUC__ * 10000 \
	+ __GNUC_MINOR__ * 100 \
	+ __GNUC_PATCHLEVEL__)
#endif //__GNUC__

#if __cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800
	#define K15_CPP11_SUPPORT
#endif //__cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800

#if defined _DEBUG || defined DEBUG
	#define K15_DEBUG
	#define K15_DEBUG_MRT //Multithreaded debugging
#endif //_DEBUG

#define K15_PTR_SIZE sizeof(void*)

#if defined (K15_CPP11_SUPPORT)
	#define OVERRIDE override
#else
	#define OVERRIDE
#endif //K15_CPP11_SUPPORT

#define GIGABYTE	1073741824
#define MEGABYTE	1048576
#define KILOBYTE	1024

#define size_gigabyte(s) (uint32)(s*GIGABYTE)
#define size_megabyte(s) (uint32)(s*MEGABYTE)
#define size_kilobyte(s) (uint32)(s*KILOBYTE)

#define intern static
#define global static

#define K15_Search bsearch
#define K15_Sort qsort

#define K15_MAX_THREADS 64

#define K15_FUNCTION_DEBUG_PRINTF(func) printf("Before "#func"\n"); func; printf("After "#func"\n")

#define K15_ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

#define K15_FUNCTION_VARIABLE(returntype, type, arguments)		typedef returntype(*type) arguments;	\
																static returntype type##_stubFnc arguments {	\
																	K15_LOG_ERROR_MESSAGE("Function \"%s\" not supported\\implemented on this platform.", #type);	\
																	return(returntype)(0);	\
																};

#ifdef K15_OS_WINDOWS
	#define _WINSOCKAPI_				// stops windows.h including winsock.h
	#define DIRECTINPUT_VERSION 0x0800	// use directinput 8
	//#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <windowsx.h>
	#include <winnt.h>
	#include <xinput.h>
	#include <dinput.h>
	#include <dsound.h>
	#include <stdlib.h>
	#include <io.h>

	//8 bit types
	typedef signed    __int8  int8;
	typedef unsigned  __int8  uint8;
	//16 bit types
	typedef signed    __int16 int16;
	typedef unsigned  __int16 uint16;
	//32 bit types
	typedef signed    __int32 int32;
	typedef unsigned  __int32 uint32;
	//64 bit types
	typedef signed    __int64 int64;
	typedef unsigned  __int64 uint64;
#else 
	//8 bit types
	typedef signed		char		int8;
	typedef unsigned	char		uint8;
	//16 bit types
	typedef signed		short		int16;
	typedef unsigned	short		uint16;
	//32 bit types
	typedef signed		int			int32;
	typedef unsigned	int			uint32;
	//64 bit types
	typedef signed		long long	int64;
	typedef unsigned	long long	uint64;
#endif // K15_OS_WINDOWS

#ifdef K15_OS_ANDROID
	#include <android/log.h>
	#include <android/native_window_jni.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <time.h>
	#include <jni.h>
	#include <pthread.h>
	#include <alloca.h>
	#include <semaphore.h>	
	#include <errno.h>
	#include <stdlib.h>
	#include <ctype.h>
	
	#define TRUE 1
	#define FALSE 0

#ifdef K15_CHECK_JNI_CALLS
	#define K15_JNI_CALL(jniEnv, call) {call; jthrowable ex = jniEnv->ExceptionOccurred(); if(ex){jniEnv->ExceptionDescribe(); jniEnv->ExceptionClear();}}
#else
	#define K15_JNI_CALL(jniEnv, call) {call;}
#endif //K15_CHECK_JNI_CALLS

#endif //K15_OS_ANDROID

#ifdef K15_OS_LINUX
	#include <pthread.h>
	#include <time.h>

	#define TRUE 1
	#define FALSE 0
#endif //K15_OS_LINUX

typedef uint8 bool8;
typedef uint8 byte;


// forward declaration
struct K15_Window;
struct K15_Thread;
struct K15_Mutex;
struct K15_ConditionVariable;
struct K15_ThreadContext;
struct K15_Semaphore;
struct K15_OSLayerContext;

typedef uint8 (*K15_ThreadFnc)(void*);

#endif //_K15_OSLayer_Prerequisites_h_