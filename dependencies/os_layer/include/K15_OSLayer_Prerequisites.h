#ifndef _K15_OSLayer_Prerequisites_h_
#define _K15_OSLayer_Prerequisites_h_

#include "K15_OSLayer_ErrorCodes.h"

#include <assert.h>
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#ifdef __ANDROID__
	#define K15_OS_ANDROID
#elif defined _WIN32
	#define K15_OS_WINDOWS
#elif defined __linux__
	#define K15_OS_LINUX
#elif defined __APPLE__
	#define K15_OS_APPLE
#endif //_WIN32

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

#if __cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800
	#define K15_CPP11_SUPPORT
#endif //__cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800

#ifdef _DEBUG
	#define K15_DEBUG
#endif //_DEBUG

#define internal static
#define global static

#ifdef K15_OS_WINDOWS
	#define _WINSOCKAPI_    // stops windows.h including winsock.h
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	#include "windowsx.h"
	#include "winnt.h"

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

typedef uint8 bool8;

// forward declaration
struct K15_Window;
struct K15_OSLayerContext;

#endif //_K15_OSLayer_Prerequisites_h_