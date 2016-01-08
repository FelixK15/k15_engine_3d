#ifndef _K15_ResourceFormat_MaterialFormat_h_
#define _K15_ResourceFormat_MaterialFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

#define K15_MATERIAL_DATA_TYPE_INT 1
#define K15_MATERIAL_DATA_TYPE_FLOAT 2
#define K15_MATERIAL_DATA_TYPE_TEXTURE 3
#define K15_MATERIAL_DATA_TYPE_SAMPLER 4

struct K15_MaterialPassDataValue
{
	union 
	{
		int asInt;
		float asFloat;
		char* asString;
	};

	uint32 dataType;
	char* name;
};

struct K15_MaterialPassTemplateFormat
{
	char* vertexShaderPath;
	char* fragmentShaderPath;

	uint32 vertexShaderPathLength;
	uint32 fragmentShaderPathLength;

	uint32 vertexShaderNameHash;
	uint32 fragmentShaderNameHash;
};

struct K15_MaterialPassDataFormat
{
	K15_MaterialPassDataValue* values;
	uint32 numValues;
};

struct K15_MaterialPassFormat
{
	K15_MaterialPassTemplateFormat materialPassTemplate;
	K15_MaterialPassDataFormat materialPassData;
};

struct K15_MaterialFormat
{
	char* materialName;
	uint32 materialNameHash;

	uint32 numMaterialPasses;
	K15_MaterialPassFormat* materialPasses;
};

uint8 K15_SetMaterialFormatName(K15_MaterialFormat* p_MaterialFormat, const char* p_Name);
uint8 K15_SetMaterialFormatPassCount(K15_MaterialFormat* p_MaterialFormat, uint32 p_AmountPasses);

uint8 K15_AddMaterialPassFormat(K15_MaterialFormat* p_MaterialFormat, K15_MaterialPassFormat* p_MaterialPassTemplateFormat, uint32 p_MaterialPassTemplateIndex);
K15_MaterialPassTemplateFormat* K15_GetMaterialPassTemplateFormat(K15_MaterialFormat* p_MaterialFormat, uint32 p_MaterialPassTemplateIndex);

uint8 K15_SetMaterialPassTemplateVertexShaderPath(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_VertexShaderPath);
uint8 K15_SetMaterialPassTemplateFragmentShaderPath(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_FragmentShaderPath);

uint8 K15_SetMaterialPassDataValueCount(K15_MaterialPassDataFormat* p_MaterialPassData, uint32 p_ValueCount);
uint8 K15_AddMaterialPassDataValue(K15_MaterialPassDataFormat* p_MaterialPassData, K15_MaterialPassDataValue* p_MaterialPassDataValue, uint32 p_MaterialPassDataValueIndex);
K15_MaterialPassDataValue* K15_GetMaterialPassDataValueByName(K15_MaterialPassDataFormat* p_MaterialPassData, const char* p_Name);

uint8 K15_SaveMaterialFormatToFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path, uint32 p_SaveFlags);

uint8 K15_LoadMaterialFormatFromFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path);
uint8 K15_LoadMaterialFormatFromMemory(K15_MaterialFormat* p_MaterialFormat, byte* p_Memory, uint32 p_MemorySize);

void K15_FreeMaterialFormat(K15_MaterialFormat p_MaterialFormat);

#endif //_K15_ResourceFormat_MaterialFormat_h_