#ifndef _K15_ResourceFormat_MaterialFormat_h_
#define _K15_ResourceFormat_MaterialFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

struct K15_MaterialPassDataFormat
{
	union 
	{
		uint32 texture;
		uint32 sampler;
	} data;


};

struct K15_MaterialPassTemplateFormat
{
	char* vertexShaderPath;
	char* fragmentShaderPath;

	uint32 vertexShaderPathLength;
	uint32 fragmentShaderPathLength;
};

struct K15_MaterialPassFormat
{
	K15_MaterialPassTemplateFormat* materialPassTemplate;
	K15_MaterialPassDataFormat* materialPassData;
};

struct K15_MaterialFormat
{
	uint32 materialNameHash;

	uint32 numMaterialPasses;
	uint32 currentNumMaterialPasses;

	K15_MaterialPassTemplateFormat* passTemplates;
};

uint8 K15_SetMaterialFormatName(K15_MaterialFormat* p_MaterialFormat, const char* p_Name);
uint8 K15_SetMaterialFormatPassCount(K15_MaterialFormat* p_MaterialFormat, uint32 p_AmountPasses);

uint8 K15_AddMaterialPassTemplateFormat(K15_MaterialFormat* p_MaterialFormat, K15_MaterialPassTemplateFormat* p_MaterialPassTemplateFormat);
K15_MaterialPassTemplateFormat* K15_GetMaterialPassTemplateFormat(K15_MaterialFormat* p_MaterialFormat, uint32 p_MaterialPassTemplateIndex);

uint8 K15_SetMaterialPassTemplateVertexShaderPath(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_VertexShaderPath);
uint8 K15_SetMaterialPassTemplateFragmentShaderPath(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_FragmentShaderPath);

uint8 K15_SaveMaterialFormatToFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path, uint32 p_SaveFlags);

uint8 K15_LoadMaterialFormatFromFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path);
uint8 K15_LoadMaterialFormatFromMemory(K15_MaterialFormat* p_MaterialFormat, byte* p_Memory, uint32 p_MemorySize);

void K15_FreeMaterialFormat(K15_MaterialFormat p_MaterialFormat);

#endif //_K15_ResourceFormat_MaterialFormat_h_