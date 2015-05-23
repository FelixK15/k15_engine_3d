#ifndef _K15_OSLayer_Dynamic_Library_h_
#define _K15_OSLayer_Dynamic_Library_h_

#include "K15_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_DynamicLibraryWin32.h"
#elif defined K15_OS_LINUX || defined K15_OS_ANDROID
	#include "posix/K15_DynamicLibraryPosix.h"
#endif

struct K15_DynamicLibrary
{
	char* name;
	char* path;
	void* userData;
};

K15_DynamicLibrary* K15_LoadDynamicLibrary(const char* p_LibraryName);

#endif //_K15_OSLayer_Dynamic_Library_h_