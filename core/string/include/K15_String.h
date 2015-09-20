#ifndef _K15_Runtime_String_h_
#define _K15_Runtime_String_h_

unsigned int K15_GetLines(const char* p_String, const char** p_Lines);
unsigned int K15_GetLinesInplace(char* p_String, char** p_Lines);

char* K15_CopyString(const char* p_String, unsigned int  p_StringLength = 0);
char* K15_CopyStringIntoBuffer(const char* p_String, char* p_Buffer, unsigned int  p_StringLength = 0);

int K15_IsSubstring(const char* p_String, const char* p_Needle);
int K15_IsSubstringR(const char* p_String, const char* p_Needle);

char* K15_ConcatStrings(const char* p_String1, const char* p_String2);
char* K15_ConcatStringsIntoBuffer(const char* p_String1, const char* p_String2, char* p_Buffer);

char* K15_GenerateString(const char* p_Format, char* p_Buffer, ...);
char** K15_CreateStringArray(unsigned int p_NumStrings, ...);
char** K15_CreateStringArrayIntoBuffer(char** p_Buffer, unsigned int p_NumStrings, ...);

char* K15_ConvertToLower(const char* p_String);
char* K15_ConvertToLowerIntoBuffer(const char* p_String, char* p_Buffer);

char* K15_ConvertToUpper(const char* p_String);
char* K15_ConvertToUpperIntoBuffer(const char* p_String, char* p_Buffer);

char* K15_ConcatStringArray(const char** p_StringArray, unsigned int p_ArrayLength);
char* K15_ConcatStringArrayIntoBuffer(const char** p_StringArray, char* p_Buffer, unsigned int p_ArrayLength);

unsigned int K15_GenerateDataHash(const unsigned char* p_Data, unsigned int p_DataSize);
unsigned int K15_GenerateStringHash(const char* p_String, unsigned int p_StringLength = 0);

#endif //_K15_Runtime_String_h_