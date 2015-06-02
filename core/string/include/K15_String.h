#ifndef _K15_Runtime_String_h_
#define _K15_Runtime_String_h_

unsigned int K15_GetLines(const char* p_String, const char** p_Lines);
unsigned int  K15_GetLinesInplace(char* p_String, char** p_Lines);

char* K15_CopyString(const char* p_String, unsigned int  p_StringLength = 0);
char* K15_CopyStringIntoBuffer(const char* p_String, char* p_Buffer, unsigned int  p_StringLength = 0);
#endif //_K15_Runtime_String_h_