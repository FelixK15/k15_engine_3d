#ifndef _K15_ResourceFormat_TextureFormat_h_
#define _K15_ResourceFormat_TextureFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"
#include "enums/K15_FormatValues.h"
#include "enums/K15_ReturnValues.h"

struct K15_TextureFormat
{
	byte* textureMemory;
	char* textureName;
	uint32 textureNameLength;
	uint32 width;
	uint32 height;
	uint8 colorComponentCount;
	uint8 compression;
	uint8 mipMapCount;
};

uint8 K15_SetTextureName(K15_TextureFormat* p_TextureFormat, const char* p_Name);
uint8 K15_SetTextureMipMap(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapIndex, byte* p_CompressedMipMapMemory);
uint8 K15_SetTextureMipMapCount(K15_TextureFormat* p_TextureFormat, uint8 p_MipMapCount);
byte* K15_GetTextureMipMap(K15_TextureFormat* p_TextureFormat, uint8 p_MipmapIndex);

uint8 K15_SaveTextureFormatToFile(K15_TextureFormat* p_TextureFormat, const char* p_File, uint32 p_SaveFlags);
uint8 K15_LoadTextureFormatFromFile(K15_TextureFormat* p_TextureFormat, const char* p_File);
uint8 K15_LoadTextureFormatFromMemory(K15_TextureFormat* p_TextureFormat, byte* p_Memory);


uint8 K15_InternalSaveTextureFormat(K15_DataAccessContext* p_DataAccessContext, K15_TextureFormat* p_TextureFormat, uint32 p_SaveFlags);
uint8 K15_InternalLoadTextureFormat(K15_DataAccessContext* p_DataAccessContext, K15_TextureFormat* p_TextureFormat);

void K15_FreeTextureFormat(K15_TextureFormat p_TextureFormat);

#endif //_K15_ResourceFormat_TextureFormat_h_