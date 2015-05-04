#ifndef _K15_OSLayer_FileSystem_Posix_h_
#define _K15_OSLayer_FileSystem_Posix_h_

#include "K15_OSLayer_Prerequisites.h"

uint32 K15_PosixGetFileSize(const char* p_FilePath);
uint8 K15_PosixFileExists(const char* p_FilePath);
char* K15_PosixConvertToSystemPath(const char* p_FilePath);


#endif //_K15_OSLayer_FileSystem_Posix_h_