#include "K15_MaterialFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"

#include "K15_String.h"

#include "enums/K15_ReturnValues.h"
#include "enums/K15_FormatValues.h"

/*********************************************************************************/
intern uint8 K15_InternalLoadMaterialFormat(K15_DataAccessContext* p_DataAcessContext, K15_MaterialFormat* p_MaterialFormat)
{
	assert(p_MaterialFormat && p_DataAcessContext);

	K15_HeaderFormat headerFormat;

	uint8 headerResult = K15_ReadHeaderFormat(p_DataAcessContext, &headerFormat, K15_RESOURCE_FORMAT_MATERIAL);
	if(headerResult != K15_SUCCESS)
	{
		return headerResult;
	}

	//read material name length
	K15_ReadData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->materialNameLength);

	//create material name buffer
	p_MaterialFormat->materialName = (char*)malloc(p_MaterialFormat->materialNameLength);

	//Read material name 
	K15_ReadData(p_DataAcessContext, p_MaterialFormat->materialNameLength, p_MaterialFormat->materialName);

	//Read number of material passes
	K15_ReadData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->numMaterialPasses);

	//create material pass buffer
	p_MaterialFormat->passTemplates = (K15_MaterialPassTemplateFormat*)malloc(p_MaterialFormat->numMaterialPasses * sizeof(K15_MaterialPassTemplateFormat));

	//read material passes
	for (uint32 materialPassIndex = 0;
		materialPassIndex < p_MaterialFormat->numMaterialPasses;
		++materialPassIndex)
	{
		K15_MaterialPassTemplateFormat materialPassFormat = {};

		//read number of samplers
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &materialPassFormat.numSamplers);

		//read vertex shader path length
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &materialPassFormat.vertexShaderPathLength);

		//read fragment shader path length
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &materialPassFormat.fragmentShaderPathLength);

		//create vertex / fragment shader path buffer
		materialPassFormat.vertexShaderPath = (char*)malloc(materialPassFormat.vertexShaderPathLength);
		materialPassFormat.fragmentShaderPath = (char*)malloc(materialPassFormat.fragmentShaderPathLength);

		//read vertex shader path
		K15_ReadData(p_DataAcessContext, materialPassFormat.vertexShaderPathLength, materialPassFormat.vertexShaderPath);

		//read fragment shader path
		K15_ReadData(p_DataAcessContext, materialPassFormat.fragmentShaderPathLength, materialPassFormat.fragmentShaderPath);

		//create sampler buffer
		materialPassFormat.samplerFormats = (K15_MaterialPassTemplateSamplerFormat*)malloc(materialPassFormat.numSamplers * sizeof(K15_MaterialPassTemplateSamplerFormat));

		for (uint32 samplerIndex = 0;
			samplerIndex < materialPassFormat.numSamplers;
			++samplerIndex)
		{
			K15_MaterialPassTemplateSamplerFormat samplerFormat = {};
			
			//read sampler name length
			K15_ReadData(p_DataAcessContext, sizeof(uint32), &samplerFormat.samplerNameLength);

			//read sampler path length
			K15_ReadData(p_DataAcessContext, sizeof(uint32), &samplerFormat.samplerPathLength);

			//create sampler name and sampler path buffer
			samplerFormat.samplerName = (char*)malloc(samplerFormat.samplerNameLength);
			samplerFormat.samplerPath = (char*)malloc(samplerFormat.samplerPathLength);

			//read sampler name
			K15_ReadData(p_DataAcessContext, samplerFormat.samplerNameLength, samplerFormat.samplerName);

			//read sampler path
			K15_ReadData(p_DataAcessContext, samplerFormat.samplerPathLength, samplerFormat.samplerPath);

			K15_AddMaterialPassTemplateSampler(&materialPassFormat, samplerFormat.samplerName, samplerFormat.samplerPath);
		}

		K15_AddMaterialPassTemplateFormat(p_MaterialFormat, &materialPassFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalSaveMaterialFormat(K15_DataAccessContext* p_DataAcessContext, K15_MaterialFormat* p_MaterialFormat, uint32 p_SaveFlags)
{
	K15_HeaderFormat headerFormat;
	K15_CreateHeader(&headerFormat, K15_RESOURCE_FORMAT_MATERIAL);

	//write resource header
	K15_WriteData(p_DataAcessContext, sizeof(K15_HeaderFormat), &headerFormat);

	//write material name length
	K15_WriteData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->materialNameLength);

	//write material name 
	K15_WriteData(p_DataAcessContext, p_MaterialFormat->materialNameLength, p_MaterialFormat->materialName);

	//write number of material passes
	K15_WriteData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->numMaterialPasses);

	//write material passes
	for (uint32 materialPassIndex = 0;
		materialPassIndex < p_MaterialFormat->numMaterialPasses;
		++materialPassIndex)
	{
		K15_MaterialPassTemplateFormat* materialPassFormat = &p_MaterialFormat->passTemplates[materialPassIndex]; 

		//write number of samplers
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->numSamplers);

		//write vertex shader path length
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->vertexShaderPathLength);

		//write fragment shader path length
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->fragmentShaderPathLength);

		//write vertex shader path
		K15_WriteData(p_DataAcessContext, materialPassFormat->vertexShaderPathLength, materialPassFormat->vertexShaderPath);

		//write fragment shader path
		K15_WriteData(p_DataAcessContext, materialPassFormat->fragmentShaderPathLength, materialPassFormat->fragmentShaderPath);

		for (uint32 samplerIndex = 0;
			samplerIndex < materialPassFormat->numSamplers;
			++samplerIndex)
		{
			K15_MaterialPassTemplateSamplerFormat* samplerFormat = &materialPassFormat->samplerFormats[samplerIndex];

			//write sampler name length
			K15_WriteData(p_DataAcessContext, sizeof(uint32), &samplerFormat->samplerNameLength);

			//write sampler path length
			K15_WriteData(p_DataAcessContext, sizeof(uint32), &samplerFormat->samplerPathLength);;

			//write sampler name
			K15_WriteData(p_DataAcessContext, samplerFormat->samplerNameLength, samplerFormat->samplerName);

			//write sampler path
			K15_WriteData(p_DataAcessContext, samplerFormat->samplerPathLength, samplerFormat->samplerPath);
		}
	}

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeMaterialFormat(*p_MaterialFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern void K15_InternalFreeMaterialPassTemplateSamplerFormats(K15_MaterialPassTemplateFormat* p_MaterialPassTemplateFormat)
{
	uint32 numSamplers = p_MaterialPassTemplateFormat->currentNumSampler;

	for (uint32 samplerIndex = 0;
		samplerIndex < numSamplers;
		++samplerIndex)
	{
		K15_RF_FREE(p_MaterialPassTemplateFormat->samplerFormats[samplerIndex].samplerName);
		K15_RF_FREE(p_MaterialPassTemplateFormat->samplerFormats[samplerIndex].samplerPath);
	}
}
/*********************************************************************************/
intern void K15_InternalFreeMaterialPassTemplateFormats(K15_MaterialFormat* p_MaterialFormat)
{
	uint32 numMaterialPassTemplates = p_MaterialFormat->currentNumMaterialPasses;

	for (uint32 materialPassTemplateIndex = 0;
		materialPassTemplateIndex < numMaterialPassTemplates;
		++materialPassTemplateIndex)
	{
		K15_RF_FREE(p_MaterialFormat->passTemplates[materialPassTemplateIndex].vertexShaderPath);
		K15_RF_FREE(p_MaterialFormat->passTemplates[materialPassTemplateIndex].fragmentShaderPath);

		K15_InternalFreeMaterialPassTemplateSamplerFormats(&p_MaterialFormat->passTemplates[materialPassTemplateIndex]);
	}
}
/*********************************************************************************/




/*********************************************************************************/
uint8 K15_SetMaterialFormatName(K15_MaterialFormat* p_MaterialFormat, const char* p_Name)
{
	assert(p_MaterialFormat);
	assert(p_Name);

	uint32 nameLength = (uint32)strlen(p_Name);

	p_MaterialFormat->materialNameLength = nameLength;
	p_MaterialFormat->materialName = K15_CopyStringIntoBuffer(p_Name, (char*)K15_RF_MALLOC(nameLength + 1));

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetMaterialFormatPassCount(K15_MaterialFormat* p_MaterialFormat, uint32 p_AmountPasses)
{
	assert(p_MaterialFormat);
	assert(p_AmountPasses);

	K15_MaterialPassTemplateFormat* materialPassTemplateBuffer = (K15_MaterialPassTemplateFormat*)K15_RF_MALLOC(p_AmountPasses * sizeof(K15_MaterialPassTemplateFormat));
	memset(materialPassTemplateBuffer, 0, (p_AmountPasses * sizeof(K15_MaterialPassTemplateFormat)));

	if (p_MaterialFormat->passTemplates)
	{
		K15_InternalFreeMaterialPassTemplateFormats(p_MaterialFormat);
		K15_RF_FREE(p_MaterialFormat->passTemplates);
	}

	p_MaterialFormat->passTemplates = materialPassTemplateBuffer;
	p_MaterialFormat->numMaterialPasses = p_AmountPasses;
	p_MaterialFormat->currentNumMaterialPasses = 0;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddMaterialPassTemplateFormat(K15_MaterialFormat* p_MaterialFormat, K15_MaterialPassTemplateFormat* p_MaterialPassTemplateFormat)
{
	assert(p_MaterialFormat);
	assert(p_MaterialPassTemplateFormat);

	assert(p_MaterialFormat->numMaterialPasses != p_MaterialFormat->currentNumMaterialPasses);

	K15_MaterialPassTemplateFormat* materialPassTemplateBuffer = p_MaterialFormat->passTemplates;

	materialPassTemplateBuffer[p_MaterialFormat->currentNumMaterialPasses++] = *p_MaterialPassTemplateFormat;

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_MaterialPassTemplateFormat* K15_GetMaterialPassTemplateFormat(K15_MaterialFormat* p_MaterialFormat, uint32 p_MaterialPassTemplateIndex)
{
	assert(p_MaterialFormat);
	assert(p_MaterialPassTemplateIndex < p_MaterialFormat->numMaterialPasses);

	return &p_MaterialFormat->passTemplates[p_MaterialPassTemplateIndex];
}
/*********************************************************************************/
uint8 K15_SetMaterialPassTemplateVertexShaderPath(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_VertexShaderPath)
{
	assert(p_MaterialPassTemplate);
	assert(p_VertexShaderPath);

	if (p_MaterialPassTemplate->vertexShaderPath)
	{
		K15_RF_FREE(p_MaterialPassTemplate->vertexShaderPath);
	}

	uint32 vertexShaderPathLength = (uint32)strlen(p_VertexShaderPath);

	p_MaterialPassTemplate->vertexShaderPath = K15_CopyStringIntoBuffer(p_VertexShaderPath, (char*)K15_RF_MALLOC(vertexShaderPathLength + 1));
	p_MaterialPassTemplate->vertexShaderPathLength = vertexShaderPathLength;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetMaterialPassTemplateFragmentShaderPath(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_FragmentShaderPath)
{
	assert(p_MaterialPassTemplate);
	assert(p_FragmentShaderPath);

	if (p_MaterialPassTemplate->fragmentShaderPath)
	{
		K15_RF_FREE(p_MaterialPassTemplate->fragmentShaderPath);
	}

	uint32 fragmentShaderPathLength = (uint32)strlen(p_FragmentShaderPath);

	p_MaterialPassTemplate->fragmentShaderPath = K15_CopyStringIntoBuffer(p_FragmentShaderPath, (char*)K15_RF_MALLOC(fragmentShaderPathLength + 1));
	p_MaterialPassTemplate->fragmentShaderPathLength = fragmentShaderPathLength;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetMaterialPassTemplateSamplerCount(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, uint32 p_SamplerCount)
{
	assert(p_MaterialPassTemplate);
	assert(p_SamplerCount);

	K15_MaterialPassTemplateSamplerFormat* samplerFormats = (K15_MaterialPassTemplateSamplerFormat*)K15_RF_MALLOC(p_SamplerCount * sizeof(K15_MaterialPassTemplateSamplerFormat));
	memset(samplerFormats, 0, p_SamplerCount * sizeof(K15_MaterialPassTemplateSamplerFormat));

	if (p_MaterialPassTemplate->samplerFormats)
	{
		K15_InternalFreeMaterialPassTemplateSamplerFormats(p_MaterialPassTemplate);
		K15_RF_FREE(p_MaterialPassTemplate->samplerFormats);
	}

	p_MaterialPassTemplate->samplerFormats = samplerFormats;
	p_MaterialPassTemplate->numSamplers = p_SamplerCount;
	p_MaterialPassTemplate->currentNumSampler = 0;
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddMaterialPassTemplateSampler(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, const char* p_SamplerName, const char* p_SamplerPath)
{
	assert(p_MaterialPassTemplate);
	assert(p_SamplerName);
	assert(p_SamplerPath);

	assert(p_MaterialPassTemplate->numSamplers != p_MaterialPassTemplate->currentNumSampler);

	uint32 samplerNameLength = (uint32)strlen(p_SamplerName);
	uint32 samplerPathLength = (uint32)strlen(p_SamplerPath);

	p_MaterialPassTemplate->samplerFormats[p_MaterialPassTemplate->currentNumSampler].samplerName = K15_CopyStringIntoBuffer(p_SamplerName, (char*)K15_RF_MALLOC(samplerNameLength + 1));
	p_MaterialPassTemplate->samplerFormats[p_MaterialPassTemplate->currentNumSampler].samplerPath = K15_CopyStringIntoBuffer(p_SamplerPath, (char*)K15_RF_MALLOC(samplerPathLength + 1));

	p_MaterialPassTemplate->samplerFormats[p_MaterialPassTemplate->currentNumSampler].samplerNameLength = samplerNameLength;
	p_MaterialPassTemplate->samplerFormats[p_MaterialPassTemplate->currentNumSampler].samplerPathLength = samplerPathLength;

	p_MaterialPassTemplate->currentNumSampler += 1;

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_MaterialPassTemplateSamplerFormat* K15_GetMaterialPassTemplateSampler(K15_MaterialPassTemplateFormat* p_MaterialPassTemplate, uint32 p_MaterialPassTemplateSamplerIndex)
{
	assert(p_MaterialPassTemplate);
	assert(p_MaterialPassTemplateSamplerIndex < p_MaterialPassTemplate->numSamplers);

	return &p_MaterialPassTemplate->samplerFormats[p_MaterialPassTemplateSamplerIndex];
}
/*********************************************************************************/
uint8 K15_SaveMaterialFormatToFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path, uint32 p_SaveFlag)
{
	assert(p_MaterialFormat && p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_SAVE);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalSaveMaterialFormat(&accessContext, p_MaterialFormat, p_SaveFlag);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
void K15_FreeMaterialFormat(K15_MaterialFormat p_MaterialFormat)
{
	K15_RF_FREE(p_MaterialFormat.materialName);
	K15_InternalFreeMaterialPassTemplateFormats(&p_MaterialFormat);
}
/*********************************************************************************/
uint8 K15_LoadMaterialFormat(K15_MaterialFormat* p_MaterialFormat, const char* p_Path)
{
	assert(p_MaterialFormat && p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_LOAD);

	if (result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalLoadMaterialFormat(&accessContext, p_MaterialFormat);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadMaterialFormatFromMemory(K15_MaterialFormat* p_MaterialFormat, byte* p_Memory, uint32 p_MemorySize)
{
	assert(p_MaterialFormat && p_Memory);

	K15_DataAccessContext accessContext;

	K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory, p_MemorySize, K15_DATA_ACCESS_USAGE_LOAD);

	uint8 result = K15_InternalLoadMaterialFormat(&accessContext, p_MaterialFormat);

	K15_CloseDataAccessContext(&accessContext);

	return result;

}
/*********************************************************************************/