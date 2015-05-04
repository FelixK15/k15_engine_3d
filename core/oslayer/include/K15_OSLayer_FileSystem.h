#ifndef _K15_OSLayer_FileSystem_h_
#define _K15_OSLayer_FileSystem_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_FileSystemWin32.h"

	#define K15_GetFileSize K15_Win32GetFileSize
	#define K15_FileExists K15_Win32FileExists
	#define K15_ConvertToSystemPath K15_Win32ConvertToSystemPath
	#define K15_DIR_SEPARATOR '\\'
#elif defined K15_OS_ANDROID || defined K15_OS_LINUX
	#include "posix/K15_FileSystemPosix.h"

	#define K15_GetFileSize K15_PosixGetFileSize
	#define K15_FileExists K15_PosixFileExists
	#define K15_ConvertToSystemPath K15_PosixConvertToSystemPath
	#define K15_DIR_SEPARATOR '/'
#endif //K15_OS_WINDWS

char* K15_GetWholeFileContent(const char* p_FilePath);
char* K15_GetWholeFileContentWithFileSize(const char* p_FilePath, uint32* p_FileSize);
char* K15_GetPathWithoutFileName(const char* p_FilePath);
char* K15_GetFileName(const char* p_FilePath);
char* K15_GetFileNameWithoutExtension(const char* p_FilePath);
char* K15_GetFileExtension(const char* p_FilePath);

#endif //_K15_OSLayer_FileSystem_h_