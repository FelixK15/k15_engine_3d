#ifndef _K15_ResourceFormat_DataAccessHelper_h_
#define _K15_ResourceFormat_DataAccessHelper_h_

#include "K15_ResourceFormats_Prerequisites.h"

typedef uint8(*_readData)(K15_DataAccessContext*, uint32, void*);
typedef uint8(*_writeData)(K15_DataAccessContext*, uint32, void*);
typedef uint8(*_skipData)(K15_DataAccessContext*, uint32);

enum K15_DataAccessUsage : uint32
{
	K15_DATA_ACCESS_USAGE_SAVE,
	K15_DATA_ACCESS_USAGE_LOAD
};

struct K15_DataAccessContext
{
	FILE* accessFile;

	byte* memoryAccess;

	K15_DataAccessUsage usage;
	uint32 offset;

	_readData readData;
	_writeData writeData;
	_skipData skipData;
}; // end of K15_DataAccessContext struct

uint8 K15_CreateDataAccessContextFromFile(K15_DataAccessContext* p_DataAccessContext, const char* p_FileName, K15_DataAccessUsage p_Usage);
uint8 K15_CreateDataAccessContextFromMemory(K15_DataAccessContext* p_DataAccessContext, byte* p_Memory, uint32 p_MemorySize, K15_DataAccessUsage p_Usage);
uint8 K15_CloseDataAccessContext(K15_DataAccessContext* p_DataAccessContext);

uint8 K15_ReadData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination);
uint8 K15_WriteData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source);
uint8 K15_SkipData(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size);

#endif //_K15_ResourceFormat_DataAccessHelper_h_