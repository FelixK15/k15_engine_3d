#ifndef _K15_OSLayer_FileSystem_Win32_h_
#define _K15_OSLayer_FileSystem_Win32_h_

#include "K15_OSLayer_Prerequisites.h"

uint32 K15_GetFileSize(const char* p_FilePath);
uint8 K15_FileExists(const char* p_FilePath);

#endif //_K15_OSLayer_FileSystem_Win32_h_