#ifndef _K15_Runtime_String_h_
#define _K15_Runtime_String_h_

#include "K15_Prerequisites.h"

uint32 K15_GetLines(const char* p_String, const char** p_Lines);
uint32 K15_GetLinesInplace(char* p_String, char** p_Lines);

char* K15_CopyString(const char* p_String, uint32 p_StringLength = 0);
char* K15_CopyStringIntoBuffer(const char* p_String, char* p_Buffer, uint32 p_StringLength = 0);
#endif //_K15_Runtime_String_h_