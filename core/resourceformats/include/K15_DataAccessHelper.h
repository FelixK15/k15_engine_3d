#ifndef _K15_ResourceFormat_DataAccessHelper_h_
#define _K15_ResourceFormat_DataAccessHelper_h_

#include "K15_ResourceFormats_Prerequisites.h"

typedef uint8(*_readData)(K15_DataAccessContext*, uint32, void*);
typedef uint8(*_writeData)(K15_DataAccessContext*, uint32, void*);
typedef uint8(*_skipData)(K15_DataAccessContext*, uint32);

struct K15_DataAccessContext
{
	FILE* accessFile;

	byte* memoryAccess;
	uint32 offset;

	_readData readData;
	_writeData writeData;
	_skipData skipData;
}; // end of K15_DataAccessContext struct

uint8 K15_CreateDataAccessContextFromFile(K15_DataAccessContext* p_DataAccessContext, const char* p_FileName);
uint8 K15_CreateDataAccessContextFromMemory(K15_DataAccessContext* p_DataAccessContext, byte* p_Memory);
uint8 K15_CloseDataAccessContext(K15_DataAccessContext* p_DataAccessContext);


uint8 K15_ReadData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination);
uint8 K15_WriteData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source);
uint8 K15_SkipData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size);

uint8 K15_ReadDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination);
uint8 K15_ReadDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination);

uint8 K15_WriteDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source);
uint8 K15_WriteDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source);

uint8 K15_SkipDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size);
uint8 K15_SkipDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size);

#endif //_K15_ResourceFormat_DataAccessHelper_h_