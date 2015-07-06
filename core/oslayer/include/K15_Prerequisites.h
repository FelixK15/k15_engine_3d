#ifndef _K15_Prerequisites_h_
#define _K15_Prerequisites_h_

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

#include "K15_Config.h"
#include "K15_ErrorCodes.h"

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

#ifdef _MSC_VER
	#pragma warning(disable : 4996) //function is deprecated, use safe function
#endif //_MSC_VER

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

#define K15_Search bsearch
#define K15_Sort qsort

#define K15_TRUE 1
#define K15_FALSE 0

#define K15_DEFAULT_THREAD_SIZE 8
#define K15_DEFAULT_DYNAMIC_LIBRARY_SIZE 8

#define K15_MAIN_THREAD 0

#define K15_FUNCTION_DEBUG_PRINTF(func) printf("Before "#func"\n"); func; printf("After "#func"\n")

#define K15_ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

#define K15_FUNCTION_VARIABLE(returntype, type, arguments)		typedef returntype(*type) arguments;	\
	static returntype type##_stubFnc arguments {	\
	K15_LOG_ERROR_MESSAGE("Function \"%s\" not supported\\implemented on this platform.", #type);	\
	return(returntype)(0);	\
																};

#ifdef K15_USE_CUSTOM_ASSERTS
#ifdef K15_OS_WINDOWS
typedef int (*MessageBoxAProc)(void*, const char*, const char*, unsigned int);
extern MessageBoxAProc _MessageBoxA;
#define K15_ASSERT_TEXT(expression, text, ...)	\
	{ \
		if (!(expression)) { \
			if (_MessageBoxA) \
			{ \
				char messageText[512]; \
				char customMessage[512]; \
				sprintf(customMessage, text, ##__VA_ARGS__);\
				sprintf(messageText, "Error: Expression \n\n\"%s\"\n\nevaluated to FALSE.\n---------------------------------------\nFile: %s\nLine: %d\nMessage: %s\n---------------------------------------\nPress IGNORE to continue execution.\nPress RETRY to trigger the debuffer.\nPress ABORT to close the application.", #expression, __FILE__, __LINE__, customMessage, ##__VA_ARGS__); \
				unsigned int style = 0x00000010L | 0x00000002L; \
				int result = _MessageBoxA(0, messageText, "Assert!", style); \
				if (result == 3){ /*IDABORT*/\
					exit(-1); \
				} else if(result == 4) { /*IDRETRY*/ \
					__debugbreak(); \
				}\
			} \
			else { \
				assert(expression && text); \
			} \
		} \
	}

#endif //K15_OS_WINDOWS
#else
#define K15_ASSERT_TEXT(expression, text, ...)	{ \
	assert(expression && text);\
	} 

#endif //K15_USE_CUSTOM_ASSERTS

#ifdef K15_OS_WINDOWS
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

#define K15_EXPORT_SYMBOL extern "C" __declspec(dllexport)

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

#define K15_TRUE 1
#define K15_FALSE 0
#endif //K15_OS_WINDOWS

typedef uint8 bool8;
typedef uint8 byte;
typedef float real32;

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

#ifndef K15_EXPORT_SYMBOL
	#define K15_EXPORT_SYMBOL
#endif 

#endif //_K15_Prerequisites_h_
