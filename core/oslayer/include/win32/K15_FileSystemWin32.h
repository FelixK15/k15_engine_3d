#ifndef _K15_FileSystem_Win32_h_
#define _K15_FileSystem_Win32_h_

#include "K15_Prerequisites.h"

bool8 K15_Win32IsRelativePath(const char* p_Path);
char** K15_Win32GetFilesInDirectory(const char* p_Path, uint32* p_FileCount, const char* p_Filter = "*.*", bool8 p_Recursively = K15_FALSE);
uint32 K15_Win32GetFileSize(const char* p_FilePath);
uint8 K15_Win32FileExists(const char* p_FilePath);
uint8 K15_Win32CopyFile(const char* p_SourcePath, const char* p_DestinationPath);
uint8 K15_Win32DirectoryExists(const char* p_Path);
byte* K15_Win32ReadWholeFileIntoBuffer(const char* p_FilePath, byte* p_Buffer);
char* K15_Win32ConvertToSystemPath(const char* p_FilePath);
char* K15_Win32GetWorkingDirectory();
#endif //_K15_FileSystem_Win32_h_