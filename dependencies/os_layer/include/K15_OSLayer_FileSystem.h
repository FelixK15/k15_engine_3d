#ifndef _K15_OSLayer_FileSystem_h_
#define _K15_OSLayer_FileSystem_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_FileSystemWin32.h"

	#define K15_GetFileSize(filePath) K15_Win32GetFileSize(filePath)
	#define K15_FileExists(filePath) K15_Win32FileExists(filePath)
	#define K15_ConvertToSystemPath(filePath) K15_Win32ConvertToSystemPath(filePath)
	#define K15_DIR_SEPARATOR '\\'
#elif defined K15_OS_ANDROID || defined K15_OS_LINUX
	#define K15_DIR_SEPARATOR '/'
#endif //K15_OS_WINDWS

char* K15_GetWholeFileContent(const char* p_FilePath);
char* K15_GetWholeFileContentWithFileSize(const char* p_FilePath, uint32* p_FileSize);
char* K15_GetPathWithoutFileName(const char* p_FilePath);
char* K15_GetFileName(const char* p_FilePath);
char* K15_GetFileNameWithoutExtension(const char* p_FilePath);
char* K15_GetFileExtension(const char* p_FilePath);

#endif //_K15_OSLayer_FileSystem_h_