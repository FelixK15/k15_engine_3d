#include "K15_DataAccessHelper.h"

#ifdef K15_RESOURCEFORMATS_USE_ZLIB
	#include "zlib.h"
#endif //K15_RESOURCEFORMATS_USE_ZLIB

/*********************************************************************************/
intern uint8 K15_ReadDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination)
{
	byte* sourceMemory = p_DataAccessContext->memoryAccess;
	sourceMemory += p_DataAccessContext->offset;

	memcpy(p_Destination, sourceMemory, p_Size);

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_ReadDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Destination)
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
intern uint8 K15_WriteDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source)
{
	byte* destinationMemory = p_DataAccessContext->memoryAccess;
	destinationMemory += p_DataAccessContext->offset;

	memcpy(destinationMemory, p_Source, p_Size);

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_WriteDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size, void* p_Source)
{
	FILE* sourceFile = p_DataAccessContext->accessFile;

	fwrite(p_Source, 1, p_Size, sourceFile);

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_SkipDataMemory(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size)
{
	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_SkipDataFile(K15_DataAccessContext* p_DataAccessContext, uint32 p_Size)
{
	if(fseek(p_DataAccessContext->accessFile, p_Size, SEEK_CUR) != 0)
	{
		return K15_ERROR_END_OF_FILE;
	}

	p_DataAccessContext->offset += p_Size;

	return K15_SUCCESS;
}
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_CreateDataAccessContextFromFile(K15_DataAccessContext* p_DataAccessContext, const char* p_FileName, K15_DataAccessUsage p_Usage)
{
	assert(p_DataAccessContext);
	assert(p_FileName);

	FILE *file = fopen(p_FileName, "wb+");

	if (!file)
	{
		return K15_ERROR_FILE;
	}

#ifdef K15_RESOURCEFORMATS_USE_ZLIB
	if (p_Usage == K15_DATA_ACCESS_USAGE_LOAD)
	{
		//create memory buffer with uncompressed data and use the memory data access context
		uint32 compressedFileSize = 0;

		fseek(file, 0, SEEK_END);
		compressedFileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		byte* compressedData = (byte*)K15_RF_MALLOC(compressedFileSize);

		//read compressed data into memory buffer
		fread(compressedData, 1, compressedFileSize, file);

		//we don't need the file anymore
		fclose(file);

		uint8 result = K15_CreateDataAccessContextFromMemory(p_DataAccessContext, compressedData, compressedFileSize, p_Usage);

		//got uncompressed in the last function
		K15_RF_FREE(compressedData);

		return result;
	}
#endif //K15_RESOURCEFORMATS_USE_ZLIB

	p_DataAccessContext->accessFile = file;
	p_DataAccessContext->memoryAccess = 0;
	p_DataAccessContext->offset = 0;
	p_DataAccessContext->usage = p_Usage;

	p_DataAccessContext->readData = K15_ReadDataFile;
	p_DataAccessContext->writeData = K15_WriteDataFile;
	p_DataAccessContext->skipData = K15_SkipDataFile;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_CreateDataAccessContextFromMemory(K15_DataAccessContext* p_DataAccessContext, byte* p_Memory, uint32 p_MemorySize, K15_DataAccessUsage p_Usage)
{
	assert(p_DataAccessContext);
	assert(p_Memory);
	assert(p_Usage == K15_DATA_ACCESS_USAGE_LOAD);

#ifdef K15_RESOURCEFORMATS_USE_ZLIB
	if(p_Usage == K15_DATA_ACCESS_USAGE_LOAD)
	{
		//uncompressed file size should be at the start of the memory block
		uint32 uncompressedFileSize = *(uint32*)p_Memory;
		byte* uncompressedData = (byte*)K15_RF_MALLOC(uncompressedFileSize);
		uLongf uncompressedDataSize = uncompressedFileSize;

		uncompress(uncompressedData, &uncompressedDataSize, p_Memory + sizeof(uint32), p_MemorySize - sizeof(uint32));

		p_Memory = uncompressedData;
	}
#endif //K15_RESOURCEFORMATS_USE_ZLIB

	p_DataAccessContext->accessFile = 0;
	p_DataAccessContext->memoryAccess = p_Memory;
	p_DataAccessContext->offset = 0;
	p_DataAccessContext->usage = p_Usage;

	p_DataAccessContext->readData = K15_ReadDataMemory;
	p_DataAccessContext->writeData = K15_WriteDataMemory;
	p_DataAccessContext->skipData = K15_SkipDataMemory;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_CloseDataAccessContext(K15_DataAccessContext* p_DataAccessContext)
{
	if(p_DataAccessContext->accessFile)
	{
		FILE* file = p_DataAccessContext->accessFile;

#ifdef K15_RESOURCEFORMATS_USE_ZLIB
		if (p_DataAccessContext->usage == K15_DATA_ACCESS_USAGE_SAVE)
		{
			uint32 uncompressedFileSize = p_DataAccessContext->offset;

			uLongf compressedDataSize = compressBound(uncompressedFileSize);
			byte* uncompressedData = (byte*)K15_RF_MALLOC(uncompressedFileSize);
			byte* compressedData = (byte*)K15_RF_MALLOC(compressedDataSize);

			fflush(file);

			//jump to start the read the written data to the uncompressedData buffer
			fseek(file, 0, SEEK_SET);
			fread(uncompressedData, uncompressedFileSize, 1, file);
			
			//compress data from the file
			compress(compressedData, &compressedDataSize, uncompressedData, uncompressedFileSize);

			//free the uncompressed data...no longer needed.
			K15_RF_FREE(uncompressedData);

			//change file size according to the compressed data size (+ size of the uncompressed data)
#ifdef _WIN32
			int fileDescriptor = _fileno(file);
			_chsize_s(fileDescriptor, compressedDataSize + sizeof(uint32));
#else 
	#error missing platform implementation
#endif 

			//jump back to file start
			fseek(file, 0, SEEK_SET);

			//write uncompressed file size first
			fwrite(&uncompressedFileSize, sizeof(uint32), 1, file);

			//write compressed data
			fwrite(compressedData, compressedDataSize, 1, file);

			//free the compressed data...no longer needed
			K15_RF_FREE(compressedData);
		}
#endif //K15_RESOURCEFORMATS_USE_ZLIB

		fclose(file);
		p_DataAccessContext->accessFile = 0;
		p_DataAccessContext->offset = 0;
	}

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