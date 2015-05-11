#ifndef _K15_Helper_Win32_h_
#define _K15_Helper_Win32_h_

#include "K15_Prerequisites.h"

uint8 K15_Win32ConvertStringToWString(const char* p_String, int p_StringLength, wchar_t* p_OutputString);
uint8 K15_Win32ConvertWStringToString(const wchar_t* p_String, int p_StringLength, char* p_OutputString);

#endif //_K15_Helper_Win32_h_
