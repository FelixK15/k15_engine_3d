#ifndef _K15_Runtime_String_h_
#define _K15_Runtime_String_h_

#include "K15_Prerequisites.h"

uint32 K15_GetLines(const char* p_String, const char** p_Lines);
uint32 K15_GetLinesInplace(char* p_String, char** p_Lines);

#endif //_K15_Runtime_String_h_