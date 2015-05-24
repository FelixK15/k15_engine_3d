#ifndef _K15_OSLayer_Dynamic_Library_Win32_h_
#define _K15_OSLayer_Dynamic_Library_Win32_h_

#include "K15_Prerequisites.h"

#define K15_LIBRARY_FILE_EXTENSION ".dll"
#define K15_LIBRARY_FILE_EXTENSION_LENGTH 4

uint8 K15_Win32LoadDynamicLibrary(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary, const char* p_Path);
uint8 K15_Win32UnloadDynamicLibrary(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary);
void* K15_Win32GetProcAddress(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary, const char* p_ProcName);

#endif //_K15_OSLayer_Dynamic_Library_Win32_h_