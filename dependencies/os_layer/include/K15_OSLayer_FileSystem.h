#ifndef _K15_OSLayer_FileSystem_h_
#define _K15_OSLayer_FileSystem_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_FileSystemWin32.h"
#endif //K15_OS_WINDWS

//uint32 K15_GetFileSize(const char* p_FilePath);

#endif //_K15_OSLayer_FileSystem_h_