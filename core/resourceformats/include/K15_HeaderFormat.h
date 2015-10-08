#ifndef _K15_ResourceDesc_HeaderDescription_h_
#define _K15_ResourceDesc_HeaderDescription_h_

#include "K15_ResourceFormats_Prerequisites.h"

struct K15_HeaderFormat
{
	char identifier[K15_IDENTIFIER_LENGTH];
	uint8 majorVersion;
	uint8 minorVersion;
	uint32 resourceType;
};

//uint8 K15_ReadHeaderFromFile(const char* p_FileName, K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType);
//uint8 K15_ReadHeaderFromMemory(byte* p_Memory, K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType);
uint8 K15_ReadHeaderFormat(K15_DataAccessContext* p_DataAcessContext, K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType);

uint8 K15_CreateHeader(K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType);
uint32 K15_CreateHash(const char* p_Identifier);

#endif //_K15_ResourceDesc_HeaderDescription_h_