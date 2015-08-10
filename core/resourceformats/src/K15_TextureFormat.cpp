#include "K15_TextureFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"
#include "enums/K15_ReturnValues.h"
#include "enums/K15_FormatValues.h"

/*********************************************************************************/
intern uint32 K15_InternalCalculateMemorySizeForTexture(K15_TextureFormat* p_TextureFormat)
{
	uint32 numMipMaps = p_TextureFormat->mipMapCount;
	uint32 memorySize = 0;

	for (uint32 mipMapIndex = 0;
		mipMapIndex < numMipMaps;
		++mipMapIndex)
	{
		memorySize += K15_GetTextureMipMapSize(p_TextureFormat, mipMapIndex);
	}

	return memorySize;
}
/*********************************************************************************/
intern uint8 K15_InternalSaveTextureFormat(K15_DataAccessContext* p_DataAccessContext, K15_TextureFormat* p_TextureFormat, uint32 p_SaveFlags)
{
	assert(p_TextureFormat && p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	K15_CreateHeader(&headerFormat, K15_RESOURCE_FORMAT_TEXTURE);

	//write header
	K15_WriteData(p_DataAccessContext, sizeof(K15_HeaderFormat), &headerFormat);

	//write texture name hash
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->textureNameHash);

	//write width
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->width);

	//write height
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->height);

	//write color component count
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->colorComponentCount);

	//write pixel format used
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->pixelFormat);

	//write mip map count
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->mipMapCount);

	//write texture memory size
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->actualTextureMemorySize);

	//write texture memory
	K15_WriteData(p_DataAccessContext, p_TextureFormat->actualTextureMemorySize, p_TextureFormat->textureMemory);

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeTextureFormat(*p_TextureFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalLoadTextureFormat(K15_DataAccessContext* p_DataAccessContext, K15_TextureFormat* p_TextureFormat)
{
	assert(p_TextureFormat && p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	uint8 headerResult = K15_ReadHeaderFormat(p_DataAccessContext, &headerFormat, K15_RESOURCE_FORMAT_TEXTURE);

	if(headerResult != K15_SUCCESS)
	{
		return headerResult;
	}

	//read texture name hash
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->textureNameHash);

	//read width
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->width);

	//read height
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->height);

	//read color component count
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->colorComponentCount);

	//read pixel format used
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->pixelFormat);

	//read mip map count
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_TextureFormat->mipMapCount);

	//read texture memory size
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_TextureFormat->actualTextureMemorySize);

	//allocate memory for texturedata
	byte* textureData = (byte*)K15_RF_MALLOC(p_TextureFormat->actualTextureMemorySize);

	if(!textureData)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	//read texture memory
	K15_ReadData(p_DataAccessContext, p_TextureFormat->actualTextureMemorySize, textureData);

	p_TextureFormat->textureMemory = textureData;

	return K15_SUCCESS;
}
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_SetTextureName(K15_TextureFormat* p_TextureFormat, const char* p_Name)
{
	assert(p_TextureFormat);
	assert(p_Name);

	p_TextureFormat->textureNameHash = K15_CreateHash(p_Name);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetTextureMipMap(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapIndex, byte* p_ImageData)
{
	assert(p_TextureFormat && p_ImageData);

	uint32 memoryOffset = p_MipMapIndex == 0 ? 0 : K15_GetTextureMipMapSize(p_TextureFormat, p_MipMapIndex - 1);
	uint32 textureSize = K15_GetTextureMipMapSize(p_TextureFormat, p_MipMapIndex);

	byte* imageData = p_TextureFormat->textureMemory + memoryOffset;

	//copy memory
	memcpy(imageData,
		   p_ImageData, 
		   textureSize);

	p_TextureFormat->actualTextureMemorySize += textureSize;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetTextureMipMapCount(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapCount)
{
	assert(p_TextureFormat && p_MipMapCount > 0);

	p_TextureFormat->mipMapCount = p_MipMapCount;

	uint32 width = p_TextureFormat->width;
	uint32 height = p_TextureFormat->height;

	//get total memory size for compressed memory
	uint32 memoryNeeded = K15_InternalCalculateMemorySizeForTexture(p_TextureFormat);
	byte* textureMemory = (byte*)K15_RF_MALLOC(memoryNeeded);

	if(!textureMemory)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	p_TextureFormat->textureMemory = textureMemory;
	p_TextureFormat->allocatedTextureMemorySize = memoryNeeded;
	p_TextureFormat->actualTextureMemorySize = 0;

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

	uint32 mipMapWidth = p_TextureFormat->width;
	uint32 mipMapHeight = p_TextureFormat->height;
	uint32 blockSize = p_TextureFormat->pixelFormat == K15_TEXTURE_DXT1_COMPRESSION ? 8 : 16;
	uint32 blockCount = 0;
	uint32 mipmapSize = 0;

	for(uint32 mipMapIndex = 0;
		mipMapIndex < p_MipMapIndex;
		++mipMapIndex)
	{
		mipMapWidth /= 2;
		mipMapHeight /= 2;
	}


	if (p_TextureFormat->pixelFormat == K15_TEXTURE_R8G8B8_UBYTE ||
		p_TextureFormat->pixelFormat == K15_TEXTURE_R8G8B8A8_UBYTE)
	{
		mipmapSize = mipMapWidth * mipMapHeight * p_TextureFormat->colorComponentCount;
	}
	else if(p_TextureFormat->pixelFormat == K15_TEXTURE_DXT1_COMPRESSION ||
			p_TextureFormat->pixelFormat == K15_TEXTURE_DXT3_COMPRESSION ||
			p_TextureFormat->pixelFormat == K15_TEXTURE_DXT5_COMPRESSION)
	{
		blockCount = ((mipMapWidth + 3) / 4) * ((mipMapHeight + 3) / 4); 
		
		mipmapSize = blockCount * blockSize;
	}

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

	uint32 memoryOffset = p_MipmapIndex == 0 ? 0 : K15_GetTextureMipMapSize(p_TextureFormat, p_MipmapIndex - 1);
	uint32 memorySize = K15_GetTextureMipMapSize(p_TextureFormat, p_MipmapIndex);

	byte* textureMemory = p_TextureFormat->textureMemory + memoryOffset;
	byte* returnTextureMemory = (byte*)malloc(memorySize);

	memcpy(returnTextureMemory, textureMemory, memorySize);

	return returnTextureMemory;
}
/*********************************************************************************/
uint8 K15_SaveTextureFormatToFile(K15_TextureFormat* p_TextureFormat, const char* p_File, uint32 p_SaveFlags)
{
	assert(p_TextureFormat && p_File);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_File, K15_DATA_ACCESS_USAGE_SAVE);

	if(result == K15_SUCCESS)
	{
		result = K15_InternalSaveTextureFormat(&accessContext, p_TextureFormat, p_SaveFlags);		
		K15_CloseDataAccessContext(&accessContext);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_LoadTextureFormatFromMemory(K15_TextureFormat* p_TextureFormat, byte* p_Memory, uint32 p_MemorySize)
{
	assert(p_TextureFormat && p_Memory);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory, p_MemorySize, K15_DATA_ACCESS_USAGE_LOAD);

	if(result == K15_SUCCESS)
	{
		result = K15_InternalLoadTextureFormat(&accessContext, p_TextureFormat);
		K15_CloseDataAccessContext(&accessContext);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_LoadTextureFormatFromFile(K15_TextureFormat* p_TextureFormat, const char* p_File)
{
	assert(p_TextureFormat && p_File);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_File, K15_DATA_ACCESS_USAGE_LOAD);

	if(result == K15_SUCCESS)
	{
		result = K15_InternalLoadTextureFormat(&accessContext, p_TextureFormat);
		K15_CloseDataAccessContext(&accessContext);
	}
	
	return result;
}
/*********************************************************************************/
void K15_FreeTextureFormat(K15_TextureFormat p_TextureFormat)
{
	K15_RF_FREE(p_TextureFormat.textureMemory);
}
/*********************************************************************************/