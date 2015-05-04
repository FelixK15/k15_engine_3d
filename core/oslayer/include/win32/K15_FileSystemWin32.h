#ifndef _K15_OSLayer_FileSystem_Win32_h_
#define _K15_OSLayer_FileSystem_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

uint32 K15_Win32GetFileSize(const char* p_FilePath);
uint8 K15_Win32FileExists(const char* p_FilePath);
char* K15_Win32ConvertToSystemPath(const char* p_FilePath);
#endif //_K15_OSLayer_FileSystem_Win32_h_