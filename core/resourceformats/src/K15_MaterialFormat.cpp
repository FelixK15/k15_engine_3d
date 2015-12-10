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

	//read material name hash
	K15_ReadData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->materialNameHash);

	//Read number of material passes
	K15_ReadData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->numMaterialPasses);

	//create material pass buffer
	p_MaterialFormat->materialPasses = (K15_MaterialPassFormat*)malloc(p_MaterialFormat->numMaterialPasses * sizeof(K15_MaterialPassFormat));

	//read material passes
	for (uint32 materialPassIndex = 0;
		materialPassIndex < p_MaterialFormat->numMaterialPasses;
		++materialPassIndex)
	{
		K15_MaterialPassFormat* materialPassFormat = &p_MaterialFormat->materialPasses[materialPassIndex];

		K15_MaterialPassTemplateFormat* templateFormat = &materialPassFormat->materialPassTemplate;
		K15_MaterialPassDataFormat* dataFormat = &materialPassFormat->materialPassData;

		//read vertex shader path length
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &templateFormat->vertexShaderPathLength);

		//read fragment shader path length
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &templateFormat->fragmentShaderPathLength);

		//create vertex / fragment shader path buffer
		templateFormat->vertexShaderPath = (char*)malloc(templateFormat->vertexShaderPathLength);
		templateFormat->fragmentShaderPath = (char*)malloc(templateFormat->fragmentShaderPathLength);
		templateFormat->vertexShaderPath[templateFormat->vertexShaderPathLength] = 0;
		templateFormat->fragmentShaderPath[templateFormat->fragmentShaderPathLength] = 0;

		//read vertex shader path
		K15_ReadData(p_DataAcessContext, templateFormat->vertexShaderPathLength, templateFormat->vertexShaderPath);

		//read fragment shader path
		K15_ReadData(p_DataAcessContext, templateFormat->fragmentShaderPathLength, templateFormat->fragmentShaderPath);

		//read vertex shader name hash
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &templateFormat->vertexShaderNameHash);

		//read fragment shader name hash
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &templateFormat->fragmentShaderNameHash);

		//write amount of material data values
		K15_ReadData(p_DataAcessContext, sizeof(uint32), &dataFormat->numValues);

		//allocate memory
		dataFormat->values = (K15_MaterialPassDataValue*)malloc(dataFormat->numValues * sizeof(K15_MaterialPassDataValue));

		//write material data values
		for (uint32 dataIndex = 0;
			dataIndex < dataFormat->numValues;
			++dataIndex)
		{
			K15_MaterialPassDataValue* dataValue = &dataFormat->values[dataIndex];
			uint32 nameLength = 0;

			//value name length
			K15_ReadData(p_DataAcessContext, sizeof(uint32), &nameLength);

			dataValue->name = (char*)malloc(nameLength);
			dataValue->name[nameLength] = 0;

			//value name
			K15_ReadData(p_DataAcessContext, nameLength, dataValue->name);

			//data type
			K15_ReadData(p_DataAcessContext, sizeof(uint32), &dataValue->dataType);

			//actual value
			if (dataValue->dataType == K15_MATERIAL_DATA_TYPE_INT)
			{
				K15_ReadData(p_DataAcessContext, sizeof(int), &dataValue->asInt);
			}
			else if (dataValue->dataType == K15_MATERIAL_DATA_TYPE_FLOAT)
			{
				K15_ReadData(p_DataAcessContext, sizeof(float), &dataValue->asFloat);
			}
			else
			{
				uint32 length = 0;

				//length
				K15_ReadData(p_DataAcessContext, sizeof(uint32), &length);

				dataValue->asString = (char*)malloc(length);
				dataValue->asString[length] = 0;

				//string
				K15_ReadData(p_DataAcessContext, length, dataValue->asString);
			}
		}
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

	//write material name hash
	K15_WriteData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->materialNameHash);

	//write number of material passes
	K15_WriteData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->numMaterialPasses);

	//write material passes
	for (uint32 materialPassIndex = 0;
		materialPassIndex < p_MaterialFormat->numMaterialPasses;
		++materialPassIndex)
	{
		K15_MaterialPassTemplateFormat* materialPassFormat = &p_MaterialFormat->materialPasses[materialPassIndex].materialPassTemplate; 
		K15_MaterialPassDataFormat* materialPassDataFormat = &p_MaterialFormat->materialPasses[materialPassIndex].materialPassData;

		//write vertex shader path length
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->vertexShaderPathLength);

		//write fragment shader path length
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->fragmentShaderPathLength);

		//write vertex shader path
		K15_WriteData(p_DataAcessContext, materialPassFormat->vertexShaderPathLength, materialPassFormat->vertexShaderPath);

		//write fragment shader path
		K15_WriteData(p_DataAcessContext, materialPassFormat->fragmentShaderPathLength, materialPassFormat->fragmentShaderPath);

		//write vertex shader name hash
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->vertexShaderNameHash);

		//write fragment shader name hash
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassFormat->fragmentShaderNameHash);

		//write amount of material data values
		K15_WriteData(p_DataAcessContext, sizeof(uint32), &materialPassDataFormat->numValues);

		//write material data values
		for (uint32 dataIndex = 0;
			dataIndex < materialPassDataFormat->numValues;
			++dataIndex)
		{
			K15_MaterialPassDataValue* dataValue = &materialPassDataFormat->values[dataIndex];
			uint32 nameLength = (uint32)strlen(dataValue->name);

			//value name length
			K15_WriteData(p_DataAcessContext, sizeof(uint32), &nameLength);

			//value name
			K15_WriteData(p_DataAcessContext, nameLength, dataValue->name);

			//data type
			K15_WriteData(p_DataAcessContext, sizeof(uint32), &dataValue->dataType);

			//actual value
			if (dataValue->dataType == K15_MATERIAL_DATA_TYPE_INT)
			{
				K15_WriteData(p_DataAcessContext, sizeof(int), &dataValue->asInt);
			}
			else if (dataValue->dataType == K15_MATERIAL_DATA_TYPE_FLOAT)
			{
				K15_WriteData(p_DataAcessContext, sizeof(float), &dataValue->asFloat);
			}
			else
			{
				uint32 length = (uint32)strlen(dataValue->asString);

				//length
				K15_WriteData(p_DataAcessContext, sizeof(uint32), &length);

				//string
				K15_WriteData(p_DataAcessContext, length, dataValue->asString);
			}
		}
	}

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeMaterialFormat(*p_MaterialFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern void K15_InternalFreeMaterialPassFormats(K15_MaterialFormat* p_MaterialFormat)
{
	uint32 numMaterialPassTemplates = p_MaterialFormat->numMaterialPasses;

	for (uint32 materialPassTemplateIndex = 0;
		materialPassTemplateIndex < numMaterialPassTemplates;
		++materialPassTemplateIndex)
	{
		K15_RF_FREE(p_MaterialFormat->materialPasses[materialPassTemplateIndex].materialPassTemplate.vertexShaderPath);
		K15_RF_FREE(p_MaterialFormat->materialPasses[materialPassTemplateIndex].materialPassTemplate.fragmentShaderPath);

		for (uint32 dataValueIndex = 0;
			dataValueIndex < p_MaterialFormat->materialPasses[materialPassTemplateIndex].materialPassData.numValues;
			++dataValueIndex)
		{
			uint32 dataType = p_MaterialFormat->materialPasses[materialPassTemplateIndex].materialPassData.values[dataValueIndex].dataType;

			if (dataType == K15_MATERIAL_DATA_TYPE_TEXTURE ||
				dataType == K15_MATERIAL_DATA_TYPE_SAMPLER)
			{
				K15_RF_FREE(p_MaterialFormat->materialPasses[materialPassTemplateIndex].materialPassData.values[dataValueIndex].asString);
			}
		}
	}
}
/*********************************************************************************/




/*********************************************************************************/
uint8 K15_SetMaterialFormatName(K15_MaterialFormat* p_MaterialFormat, const char* p_Name)
{
	assert(p_MaterialFormat);
	assert(p_Name);

	p_MaterialFormat->materialNameHash = K15_CreateHash(p_Name);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetMaterialFormatPassCount(K15_MaterialFormat* p_MaterialFormat, uint32 p_AmountPasses)
{
	assert(p_MaterialFormat);
	assert(p_AmountPasses);

	K15_MaterialPassFormat* materialPassBuffer = (K15_MaterialPassFormat*)K15_RF_MALLOC(p_AmountPasses * sizeof(K15_MaterialPassFormat));
	memset(materialPassBuffer, 0, (p_AmountPasses * sizeof(K15_MaterialPassFormat)));

	if (p_MaterialFormat->materialPasses)
	{
		K15_InternalFreeMaterialPassFormats(p_MaterialFormat);
		K15_RF_FREE(p_MaterialFormat->materialPasses);
	}

	p_MaterialFormat->materialPasses = materialPassBuffer;
	p_MaterialFormat->numMaterialPasses = p_AmountPasses;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddMaterialPassFormat(K15_MaterialFormat* p_MaterialFormat, K15_MaterialPassFormat* p_MaterialPassFormat, uint32 p_MaterialPassIndex)
{
	assert(p_MaterialFormat);
	assert(p_MaterialPassFormat);
	assert(p_MaterialFormat->numMaterialPasses > p_MaterialPassIndex);

	K15_MaterialPassFormat* materialPassBuffer = p_MaterialFormat->materialPasses;
	materialPassBuffer[p_MaterialPassIndex] = *p_MaterialPassFormat;

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_MaterialPassTemplateFormat* K15_GetMaterialPassTemplateFormat(K15_MaterialFormat* p_MaterialFormat, uint32 p_MaterialPassTemplateIndex)
{
	assert(p_MaterialFormat);
	assert(p_MaterialPassTemplateIndex < p_MaterialFormat->numMaterialPasses);

	return &p_MaterialFormat->materialPasses[p_MaterialPassTemplateIndex].materialPassTemplate;
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
	p_MaterialPassTemplate->vertexShaderNameHash = K15_CreateHash(p_VertexShaderPath);

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
	p_MaterialPassTemplate->fragmentShaderNameHash = K15_CreateHash(p_FragmentShaderPath);


	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetMaterialPassDataValueCount(K15_MaterialPassDataFormat* p_MaterialPassData, uint32 p_ValueCount)
{
	assert(p_MaterialPassData);
	
	K15_MaterialPassDataValue* valueBuffer = (K15_MaterialPassDataValue*)K15_RF_MALLOC(p_ValueCount * sizeof(K15_MaterialPassDataValue));

	if (p_MaterialPassData->values)
	{
		K15_RF_FREE(p_MaterialPassData->values);
	}

	p_MaterialPassData->values = valueBuffer;
	p_MaterialPassData->numValues = p_ValueCount;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddMaterialPassDataValue(K15_MaterialPassDataFormat* p_MaterialPassData, K15_MaterialPassDataValue* p_MaterialPassDataValue, uint32 p_MaterialPassDataValueIndex)
{
	assert(p_MaterialPassData);
	assert(p_MaterialPassDataValue);
	assert(p_MaterialPassData->numValues > p_MaterialPassDataValueIndex);

	p_MaterialPassData->values[p_MaterialPassDataValueIndex] = *p_MaterialPassDataValue;

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_MaterialPassDataValue* K15_GetMaterialPassDataValueByName(K15_MaterialPassDataFormat* p_MaterialPassData, const char* p_Name)
{
	K15_MaterialPassDataValue* dataValue = 0;

	for (uint32 valueIndex = 0;
		valueIndex < p_MaterialPassData->numValues;
		++valueIndex)
	{
		if (strcmp(p_MaterialPassData->values[valueIndex].name, p_Name) == 0)
		{
			dataValue = &p_MaterialPassData->values[valueIndex];
			break;
		}
	}

	return dataValue;
}
/*********************************************************************************/
uint8 K15_SaveMaterialFormatToFile(K15_MaterialFormat* p_MaterialFormat, const char* p_Path, uint32 p_SaveFlag)
{
	assert(p_MaterialFormat && p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_SAVE);

	if(result == K15_SUCCESS)
	{
		result = K15_InternalSaveMaterialFormat(&accessContext, p_MaterialFormat, p_SaveFlag);
		K15_CloseDataAccessContext(&accessContext);
	}
	
	return result;
}
/*********************************************************************************/
void K15_FreeMaterialFormat(K15_MaterialFormat p_MaterialFormat)
{
	K15_InternalFreeMaterialPassFormats(&p_MaterialFormat);
}
/*********************************************************************************/
uint8 K15_LoadMaterialFormat(K15_MaterialFormat* p_MaterialFormat, const char* p_Path)
{
	assert(p_MaterialFormat && p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_LOAD);

	if (result != K15_SUCCESS)
	{
		result = K15_InternalLoadMaterialFormat(&accessContext, p_MaterialFormat);
		K15_CloseDataAccessContext(&accessContext);
	}

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