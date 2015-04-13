#include "K15_TextureFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"
#include "enums/K15_ReturnValues.h"
#include "enums/K15_FormatValues.h"


//internal
uint32 K15_Internal_CalculateCompressedMemorySize(uint32 p_Width, uint32 p_Height, uint32 p_MipMapCount)
{
	uint32 blockCount = 0;
	uint32 blockSize = 16;
	uint32 memoryNeeded = 0;

	for(uint32 mipmapIndex = 0;
		mipmapIndex < p_MipMapCount + 1; //+1 because base image gets also saved
		++mipmapIndex)
	{
		blockCount = ((p_Width + 3) / 4) * ((p_Height + 3) / 4); 
		memoryNeeded += blockCount * blockSize;

		p_Width /= 2;
		p_Height /= 2;
	}

	return memoryNeeded;
}

/*********************************************************************************/
uint8 K15_SetTextureName(K15_TextureFormat* p_TextureFormat, const char* p_Name)
{
	assert(p_TextureFormat && p_Name);

	size_t nameLength = strlen(p_Name);
	char* nameBuffer = (char*)K15_RF_MALLOC(nameLength);

	if(!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	memcpy(nameBuffer, p_Name, nameLength);

	p_TextureFormat->textureNameLength = (uint32)nameLength;
	p_TextureFormat->textureName = nameBuffer;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetTextureMipMap(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapIndex, byte* p_CompressedMipMapMemory)
{
	assert(p_TextureFormat && p_CompressedMipMapMemory);

	uint32 mipMapWidth = p_TextureFormat->width;
	uint32 mipMapHeight = p_TextureFormat->height;
	uint32 blockCount = 0;
	uint32 blockSize = 16;

	uint32 textureMemoryOffset = 0;

	//if we dont want to write the base image, we need to compute the offset into the texture memory
	//as base image and mip map data is saved tightly packed
	for(uint32 mipMapIndex = 0;
		mipMapIndex < p_MipMapIndex;
		++mipMapIndex)
	{
		blockCount = ((mipMapWidth + 3) / 4) * ((mipMapHeight + 3) / 4); 
		textureMemoryOffset += blockCount * blockSize;

		mipMapWidth /= 2;
		mipMapHeight /= 2;
	}

	blockCount = ((mipMapWidth + 3) / 4) * ((mipMapHeight + 3) / 4); 
	uint32 compressedTextureSize = blockCount * blockSize;


	//copy memory
	memcpy(p_TextureFormat->textureMemory + textureMemoryOffset,
		   p_CompressedMipMapMemory, 
		   compressedTextureSize);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetTextureMipMapCount(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapCount)
{
	assert(p_TextureFormat && p_MipMapCount > 0);

	uint32 width = p_TextureFormat->width;
	uint32 height = p_TextureFormat->height;

	//get total memory size for compressed memory
	uint32 memoryNeeded = K15_Internal_CalculateCompressedMemorySize(width, height, p_MipMapCount);
	byte* textureMemory = (byte*)K15_RF_MALLOC(memoryNeeded);

	if(!textureMemory)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	p_TextureFormat->textureMemory = textureMemory;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint32 K15_GetTextureMipMapSize(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapIndex)
{
	assert(p_TextureFormat);

	if (p_MipMapIndex > p_TextureFormat->mipMapCount)
	{
		return 0;
	}

	uint32 width = K15_MAX(1, p_TextureFormat->width / (uint32)pow(2.0, p_MipMapIndex - 1));
	uint32 height = K15_MAX(1, p_TextureFormat->height / (uint32)pow(2.0, p_MipMapIndex - 1));
	
	uint32 blockCount = ((width + 3) / 4) * ((height + 3) / 4);
	uint32 blockSize = 16;
	uint32 mipmapSize = blockCount * blockSize;

	return mipmapSize;
}
/*********************************************************************************/
byte* K15_GetTextureMipMap(K15_TextureFormat* p_TextureFormat, uint8 p_MipmapIndex)
{
	assert(p_TextureFormat);

	if(p_MipmapIndex > p_TextureFormat->mipMapCount)
	{
		return 0;
	}
	
	uint32 width = p_TextureFormat->width;
	uint32 height = p_TextureFormat->height;
	uint32 blockCount = 0;
	uint32 blockSize = 16;
	uint32 memoryOffset = 0;

	for(uint32 mipmapIndex = 0;
		mipmapIndex < p_MipmapIndex; //+1 because base image gets also saved
		++mipmapIndex)
	{
		blockCount = ((width + 3) / 4) * ((height + 3) / 4); 
		memoryOffset += blockCount * blockSize;

		width /= 2;
		height /= 2;
	}

	return p_TextureFormat->textureMemory + memoryOffset;
}
/*********************************************************************************/
uint8 K15_SaveTextureFormatToFile(K15_TextureFormat* p_TextureFormat, const char* p_File, uint32 p_SaveFlags)
{
	assert(p_TextureFormat && p_File);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_File);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalSaveTextureFormat(&accessContext, p_TextureFormat, p_SaveFlags);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadTextureFormatFromMemory(K15_TextureFormat* p_TextureFormat, byte* p_Memory)
{
	assert(p_TextureFormat && p_Memory);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalLoadTextureFormat(&accessContext, p_TextureFormat);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadTextureFormatFromFile(K15_TextureFormat* p_TextureFormat, const char* p_File)
{
	assert(p_TextureFormat && p_File);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_File);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalLoadTextureFormat(&accessContext, p_TextureFormat);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
void K15_FreeTextureFormat(K15_TextureFormat p_TextureFormat)
{
	K15_RF_FREE(p_TextureFormat.textureName);
	K15_RF_FREE(p_TextureFormat.textureMemory);
}
/*********************************************************************************/
uint8 K15_InternalSaveTextureFormat(K15_DataAccessContext* p_DataAccessContext, K15_TextureFormat* p_TextureFormat, uint32 p_SaveFlags)
{
	assert(p_TextureFormat && p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	K15_CreateHeader(&headerFormat, K15_RESOURCE_FORMAT_TEXTURE);

	//write header
	K15_WriteData(p_DataAccessContext, sizeof(K15_HeaderFormat), &headerFormat);

	//write texture name length
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->textureNameLength);

	//write texture name
	K15_WriteData(p_DataAccessContext, p_TextureFormat->textureNameLength, p_TextureFormat->textureName);

	//write width
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->width);

	//write height
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->height);

	//write color component count
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->colorComponentCount);

	//write compression used
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->compression);

	//write mip map count
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->mipMapCount);

	//calculate amount of data for base image + mip maps
	uint32 textureMemorySize = K15_Internal_CalculateCompressedMemorySize(p_TextureFormat->width,
		p_TextureFormat->height, 
		p_TextureFormat->mipMapCount);

	//write texture memory
	K15_WriteData(p_DataAccessContext, textureMemorySize, p_TextureFormat->textureMemory);

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeTextureFormat(*p_TextureFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_InternalLoadTextureFormat(K15_DataAccessContext* p_DataAccessContext, K15_TextureFormat* p_TextureFormat)
{
	assert(p_TextureFormat && p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	uint8 headerResult = K15_InternalReadHeaderFormat(p_DataAccessContext, &headerFormat, K15_RESOURCE_FORMAT_TEXTURE);

	if(headerResult != K15_SUCCESS)
	{
		return headerResult;
	}

	//read texture name
	uint32 nameLength = 0;
	char* nameBuffer = 0;

	//read texture name length
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &nameLength);

	nameBuffer = (char*)K15_RF_MALLOC(nameLength + 1); // +1 for 0 terminator

	if (!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	//read texture name
	K15_ReadData(p_DataAccessContext, nameLength, nameBuffer);
	nameBuffer[nameLength] = 0;
	p_TextureFormat->textureName = nameBuffer;

	//read width
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->width);

	//read height
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->height);

	//read color component count
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->colorComponentCount);

	//read compression used
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->compression);

	//read mip map count
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->mipMapCount);

	//calculate amount of data for base image + mip maps
	uint32 textureMemorySize = K15_Internal_CalculateCompressedMemorySize(p_TextureFormat->width,
		p_TextureFormat->height, 
		p_TextureFormat->mipMapCount);

	//allocate memory for texturedata
	byte* textureData = (byte*)K15_RF_MALLOC(textureMemorySize);

	if(!textureData)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	//read texture memory
	K15_ReadData(p_DataAccessContext, textureMemorySize, textureData);

	p_TextureFormat->textureMemory = textureData;

	return K15_SUCCESS;
}
/*********************************************************************************/