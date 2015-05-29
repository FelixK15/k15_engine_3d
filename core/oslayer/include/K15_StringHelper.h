#ifndef _K15_OSContext_String_Helper_h_
#define _K15_OSContext_String_Helper_h_

//Allocated p_StringLenght + 1 bytes and creates a copy of p_String
char* K15_OS_CopyString(const char* p_String, unsigned int p_StringLength = 0, char* p_OutputBuffer = 0);

#endif //_K15_OSContext_String_Helper_h_