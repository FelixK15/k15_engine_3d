#include "K15_DataAccessHelper.h"

/*********************************************************************************/
uint8 K15_CreateDataAccessContextFromFile(K15_DataAccessContext* p_DataAccessContext, const char* p_FileName)
{
	assert(p_DataAccessContext && p_FileName);

	FILE *file = fopen(p_FileName, "rb+");

	if (!file)
	{
		return K15_ERROR_FILE;
	}

	p_DataAccessContext->accessFile = file;
	p_DataAccessContext->memoryAccess = 0;
	p_DataAccessContext->offset = 0;

	p_DataAccessContext->readData = K15_ReadDataFile;
	p_DataAccessContext->writeData = K15_WriteDataFile;
	p_DataAccessContext->skipData = K15_SkipDataFile;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_CreateDataAccessContextFromMemory(K15_DataAccessContext* p_DataAccessContext, byte* p_Memory)
{
	assert(p_DataAccessContext && p_Memory);

	p_DataAccessContext->accessFile = 0;
	p_DataAccessContext->memoryAccess = p_Memory;
	p_DataAccessContext->offset = 0;

	p_DataAccessContext->readData = K15_ReadDataMemory;
	p_DataAccessContext->writeData = K15_WriteDataMemory;
	p_DataAccessContext->skipData = K15_SkipDataMemory;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_ReadDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination)
{
	byte* sourceMemory = p_DataAccessContext->memoryAccess;
	sourceMemory += p_DataAccessContext->offset;

	memcpy(p_Destination, sourceMemory, p_Size);

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_ReadDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination)
{
	FILE* sourceFile = p_DataAccessContext->accessFile;

	size_t elementsRead = fread(p_Destination, 1, p_Size, sourceFile);
	assert(elementsRead == p_Size);

	if(elementsRead != p_Size)
	{
		return K15_ERROR_END_OF_FILE;
	}

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_WriteDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source)
{
	byte* destinationMemory = p_DataAccessContext->memoryAccess;
	destinationMemory += p_DataAccessContext->offset;

	memcpy(destinationMemory, p_Source, p_Size);

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_WriteDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source)
{
	FILE* sourceFile = p_DataAccessContext->accessFile;

	fwrite(p_Source, 1, p_Size, sourceFile);

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SkipDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size)
{
	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SkipDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size)
{
	if(fseek(p_DataAccessContext->accessFile, p_Size, SEEK_CUR) != 0)
	{
		return K15_ERROR_END_OF_FILE;
	}

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}

/*********************************************************************************/
uint8 K15_ReadData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination)
{
	return p_DataAccessContext->readData(p_DataAccessContext, p_Size, p_Destination);
}
/*********************************************************************************/
uint8 K15_WriteData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source)
{
	return p_DataAccessContext->writeData(p_DataAccessContext, p_Size, p_Source);
}
/*********************************************************************************/
uint8 K15_SkipData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size)
{
	return p_DataAccessContext->skipData(p_DataAccessContext, p_Size);
}
/*********************************************************************************/
uint8 K15_CloseDataAccessContext(K15_DataAccessContext* p_DataAccessContext)
{
	if(p_DataAccessContext->accessFile)
	{
		fflush(p_DataAccessContext->accessFile);
		fclose(p_DataAccessContext->accessFile);
		p_DataAccessContext->accessFile = 0;
		p_DataAccessContext->offset = 0;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
