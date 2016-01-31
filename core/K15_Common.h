#ifndef _K15_Common_h_
#define _K15_Common_h_

#ifdef __ANDROID__
#	define K15_OS_ANDROID
#elif defined _WIN32
#	define K15_OS_WINDOWS
#elif defined __linux__
#	define K15_OS_LINUX
#elif defined __APPLE__
#	define K15_OS_APPLE
#endif //_WIN32

#ifdef K15_OS_ANDROID
#	define K15_PLATFORM_MOBILE
#endif //K15_OS_ANDROID

#if defined (K15_OS_WINDOWS)
#	ifdef _WIN64
#		define K15_64_BIT
#	endif //_WIN64
#else
#	include <limits.h>
#	ifdef __LP64__
#		define K15_64_BIT
#	endif //__LP64__
#endif //K15_OS_WINDOWS

#ifdef __GNUC__
//http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
#	define K15_GCC_VERSION (__GNUC__ * 10000 \
		+ __GNUC_MINOR__ * 100 \
		+ __GNUC_PATCHLEVEL__)
#endif //__GNUC__

#if defined _DEBUG || defined DEBUG
#	define K15_DEBUG
#	define K15_DEBUG_MRT //Multithreaded debugging
#endif //_DEBUG

#ifdef _MSC_VER
#	pragma warning(disable : 4996) //function is deprecated, use safe function
#	pragma warning(disable : 4474) //too many arguments passed for format string (seems buggy in VS 2015)
#	pragma warning(disable : 4312) //conversion from 'x' to 'y' of greater size - I know what I'm doing
#	pragma warning(disable : 4311) //pointer truncation
#	pragma warning(disable : 4302) //pointer truncation
#	pragma warning(disable : 4838) //pointer truncation
#endif //_MSC_VER

#define K15_PTR_SIZE sizeof(void*)

#define GIGABYTE	1073741824
#define MEGABYTE	1048576
#define KILOBYTE	1024

#define size_gigabyte(s) (uint32)(s*GIGABYTE)
#define size_megabyte(s) (uint32)(s*MEGABYTE)
#define size_kilobyte(s) (uint32)(s*KILOBYTE)

#define intern static

#define K15_TRUE 1
#define K15_FALSE 0

#define K15_SAFE_FREE(ptr) free(ptr); ptr = 0;

#define K15_ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

#define K15_ASSERT(expr) assert(expr)
#define K15_ASSERT_TEXT_SIMPLE(expr, text) assert(expr && text)

#ifdef K15_OS_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	define K15_ASSERT_TEXT(expression, text, ...)	\
	{ \
		if (!(expression)) { \
			char messageText[512]; \
			char customMessage[512]; \
			sprintf(customMessage, text, ##__VA_ARGS__);\
			sprintf(messageText, "Error: Expression \n\n\"%s\"\n\nevaluated to FALSE.\n---------------------------------------\nFile: %s\nLine: %d\nMessage: %s\n---------------------------------------\nPress IGNORE to continue execution.\nPress RETRY to trigger the debuffer.\nPress ABORT to close the application.", #expression, __FILE__, __LINE__, customMessage, ##__VA_ARGS__); \
			unsigned int style = 0x00000010L | 0x00000002L; \
			int result = MessageBoxA(0, messageText, "Assert!", style); \
			if (result == 3){ /*IDABORT*/\
			exit(-1); \
			} else if(result == 4) { /*IDRETRY*/ \
			__debugbreak(); \
			}\
		} \
	}
#endif //K15_OS_WINDOWS

#define K15_FUNCTION_VARIABLE(returntype, type, arguments)		typedef returntype(*type) arguments;	\
	static returntype type##_stubFnc arguments {	\
	K15_ASSERT_TEXT(false, "Function \"%s\" not supported\\implemented on this platform.", #type);	\
	return(returntype)(0);}	\

#define K15_FUNCTION_VARIABLE_CUSTOM_RETURN(returntype, type, arguments) typedef returntype(*type) arguments;	\
	static returntype type##_stubFnc arguments {	\
	returntype r = {}; \
	K15_ASSERT_TEXT(false, "Function \"%s\" not supported\\implemented on this platform.", #type);	\
	return r;}	\

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

#	define K15_EXPORT_SYMBOL extern "C" __declspec(dllexport)

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
#endif //K15_OS_WINDOWS

typedef uint8 bool8;
typedef uint8 byte;
typedef uint8 result8;
typedef float real32;

#define K15_PTR_DIFF(s, e) ((size_t)(e) - (size_t)(s))

#ifndef K15_EXPORT_SYMBOL
#	define K15_EXPORT_SYMBOL
#endif 

#endif //_K15_Common_h_