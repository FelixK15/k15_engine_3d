#ifndef _K15_FileSystem_Win32_h_
#define _K15_FileSystem_Win32_h_

#include "K15_Prerequisites.h"

uint32 K15_Win32GetFileSize(const char* p_FilePath);
uint8 K15_Win32FileExists(const char* p_FilePath);
uint8 K15_Win32CopyFile(const char* p_SourcePath, const char* p_DestinationPath);
byte* K15_Win32ReadWholeFile(const char* p_FilePath);
char* K15_Win32ConvertToSystemPath(const char* p_FilePath);
char* K15_Win32GetWorkingDirectory();
#endif //_K15_FileSystem_Win32_h_