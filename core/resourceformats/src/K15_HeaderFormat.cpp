#include "K15_HeaderFormat.h"

#include "K15_DataAccessHelper.h"

#include "enums/K15_ReturnValues.h"

/*********************************************************************************/
// uint8 K15_ReadHeaderFromFile(const char* p_FileName, K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType)
// {
// 	assert(p_FileName && p_HeaderFormat);
// 
// 	K15_DataAccessContext accessContext;
// 
// 	K15_CreateDataAccessContextFromFile(&accessContext, p_FileName);
// 
// 	return K15_ReadHeaderFormat(&accessContext, p_HeaderFormat, p_ResourceType);
// }
// /*********************************************************************************/
// uint8 K15_ReadHeaderFromMemory(byte* p_Memory, K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType)
// {
// 	assert(p_Memory && p_HeaderFormat);
// 
// 	K15_DataAccessContext accessContext;
// 
// 	K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory);
// 
// 	return K15_ReadHeaderFormat(&accessContext, p_HeaderFormat, p_ResourceType);
// }
/*********************************************************************************/
uint8 K15_ReadHeaderFormat(K15_DataAccessContext* p_DataAcessContext, K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType)
{
	assert(p_DataAcessContext && p_HeaderFormat);

	uint8 result = K15_ReadData(p_DataAcessContext, sizeof(K15_HeaderFormat), p_HeaderFormat);

	if(result != K15_SUCCESS)
	{
		return result;
	}

	//TODO CHECK ENDIANESS
	if(memcmp(K15_RESOURCE_IDENTIFIER, p_HeaderFormat->identifier, K15_IDENTIFIER_LENGTH) != 0)
	{
		return K15_ERROR_WRONG_FILE_FORMAT;
	}

	if(p_ResourceType != p_HeaderFormat->resourceType)
	{
		return K15_ERROR_WRONG_FILE_FORMAT;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_CreateHeader(K15_HeaderFormat* p_HeaderFormat, uint32 p_ResourceType)
{
	assert(p_HeaderFormat && p_ResourceType);

	memcpy(p_HeaderFormat->identifier, K15_RESOURCE_IDENTIFIER, K15_IDENTIFIER_LENGTH);
	p_HeaderFormat->resourceType = p_ResourceType;

	switch(p_ResourceType)
	{
	case K15_RESOURCE_FORMAT_MESH:
		{
			p_HeaderFormat->majorVersion = K15_FORMAT_MESH_MAJOR_VERSION;
			p_HeaderFormat->minorVersion = K15_FORMAT_MESH_MINOR_VERSION;
			break;
		}

	default:
		{
			return K15_ERROR_WRONG_DATA_IDENTIFIER;
		}
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint32 K15_CreateHash(const char* p_Identifier)
{
	assert(p_Identifier);

	uint32 hash = 0;
	uint32 i;
	uint32 length = (uint32)strlen(p_Identifier);

	for (i = 0; 
		i < length; 
		i++) 
	{
		hash = 33*hash + 720 + p_Identifier[i];
	}

	return hash;
}
/*********************************************************************************/