#ifndef _K15_ResourceFormat_MaterialFormat_h_
#define _K15_ResourceFormat_MaterialFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

struct K15_MaterialPassTemplateSamplerFormat
{
	char* samplerName;
	char* samplerPath;

	uint32 samplerNameLength;
	uint32 samplerPathLength;
};

struct K15_MaterialPassTemplateFormat
{
	char* vertexShaderPath;
	char* fragmentShaderPath;

	uint32 vertexShaderPathLength;
	uint32 fragmentShaderPathLength;

	uint32 numSamplers;
	uint32 currentNumSampler;

	K15_MaterialPassTemplateSamplerFormat* samplerFormats;
};

struct K15_MaterialFormat
{
	char* materialName;

	uint32 materialNameLength;
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

uint8 K15_SetMaterialPassTemplateSamplerCount(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, uint32 p_SamplerCount);
uint8 K15_AddMaterialPassTemplateSampler(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_SamplerName, const char* p_SamplerPath);
K15_MaterialPassTemplateSamplerFormat* K15_GetMaterialPassTemplateSampler(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, uint32 p_MaterialPassTemplateSamplerIndex);

uint8 K15_SaveMaterialFormatToFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path, uint32 p_SaveFlags);

uint8 K15_LoadMaterialFormatFromFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path);
uint8 K15_LoadMaterialFormatFromMemory(K15_MaterialFormat* p_MaterialFormat, byte* p_Memory, uint32 p_MemorySize);

void K15_FreeMaterialFormat(K15_MaterialFormat p_MaterialFormat);

#endif //_K15_ResourceFormat_MaterialFormat_h_