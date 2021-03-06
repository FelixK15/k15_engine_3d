#ifndef _K15_FileSystem_h_
#define _K15_FileSystem_h_

#include "K15_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_FileSystemWin32.h"

	#define K15_GetFileLastAccessTimeStamp		K15_Win32GetFileLastAccessTimeStamp
	#define K15_GetFileSize						K15_Win32GetFileSize
	#define K15_FileExists						K15_Win32FileExists
	#define K15_CopyWholeFileContentIntoBuffer	K15_Win32ReadWholeFileIntoBuffer
	#define K15_CopyFile						K15_Win32CopyFile
	#define K15_DirectoryExists					K15_Win32DirectoryExists
	#define K15_GetFilesInDirectory				K15_Win32GetFilesInDirectory
	#define K15_IsRelativePath					K15_Win32IsRelativePath

	#define K15_DIR_SEPARATOR '/'
#elif defined K15_OS_ANDROID || defined K15_OS_LINUX
	#include "posix/K15_FileSystemPosix.h"

	#define K15_GetFileSize K15_PosixGetFileSize
	#define K15_FileExists K15_PosixFileExists
	#define K15_DIR_SEPARATOR '/'
#endif //K15_OS_WINDWS

const char* K15_GetWorkingDirectory();
const char* K15_SetWorkingDirectory(const char* p_WorkingDirectory);

byte* K15_GetWholeFileContent(const char* p_FilePath);
byte* K15_GetWholeFileContentWithFileSize(const char* p_FilePath, uint32* p_FileSize);

bool8 K15_IsAbsolutePath(const char* p_FilePath);


char* K15_GetPathWithoutFileName(const char* p_FilePath);
char* K15_GetPathWithoutExtension(const char* p_FilePath);
char* K15_GetFileNameWithoutPath(const char* p_FilePath);
char* K15_GetFileNameWithoutExtension(const char* p_FilePath);
char* K15_GetFileExtension(const char* p_FilePath);
char* K15_ConvertToDirectoryPath(const char* p_String);
char* K15_ConvertToAbsolutePath(const char* p_FilePath);
char* K15_ConvertToSystemPath(const char* p_FilePath);
char* K15_ConvertToSystemPathInplace(char* p_FilePath);
char* K15_ConvertToSystemPathIntoBuffer(const char* p_FilePath, char* p_Buffer);

#endif //_K15_FileSystem_h_
