#ifndef _K15_ResourceFormat_MaterialFormat_h_
#define _K15_ResourceFormat_MaterialFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

struct K15_SubMaterialTextureFormat
{
	char* textureName;
	uint32 textureNameLength;
	
	float blendStrength;
	uint8 blendOperation;
	uint8 textureMappingU;
	uint8 textureMappingV;
	uint8 hasAlpha;
};

struct K15_SubMaterialFormat
{
	K15_SubMaterialTextureFormat* diffuseTextureFormat;

	float diffuseColor[3];
	float specularColor[3];
	float ambientColor[3];

	float shininess;
	float sininessStrength;

	uint8 diffuseTextureCount;
};

struct K15_MaterialFormat
{
	K15_SubMaterialFormat* subMaterials;
	char* materialName;

	uint32 materialNameLength;
	uint16 submaterialCount;
};

uint8 K15_SetMaterialName(K15_MaterialFormat* p_MaterialFormat, const char* p_Name);

uint8 K15_SetSubMaterialCount(K15_MaterialFormat* p_MaterialFormat, uint16 p_SubMaterialCount);

uint8 K15_SetSubMaterialTextureCount(K15_SubMaterialFormat* p_SubMaterialFormat, uint32 p_TextureCount, uint32 p_TextureIdentifier);
uint8 K15_SetSubMaterialTextureFormatTextureName(K15_SubMaterialTextureFormat* p_SubMaterialTextureFormat, const char* p_TextureName);

uint8 K15_SaveMaterialFormatgToFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path, uint32 p_SaveFlags);

uint8 K15_LoadMaterialFormatFromFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path);
uint8 K15_LoadMaterialFormatFromMemory(K15_MaterialFormat* p_MaterialFormat, byte* p_Memory, uint32 p_MemorySize);

void K15_FreeMaterialFormat(K15_MaterialFormat p_MaterialFormat);

#endif //_K15_ResourceFormat_MaterialFormat_h_