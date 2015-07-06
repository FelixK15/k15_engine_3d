#include "K15_MaterialFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"

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

	//material name
	uint32 nameLength = 0;
	char* nameBuffer = 0;

	//name length
	K15_ReadData(p_DataAcessContext, sizeof(uint32), &nameLength);

	nameBuffer = (char*)K15_RF_MALLOC(nameLength + 1); //+1 for 0 terminator

	if(!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	//material name
	K15_ReadData(p_DataAcessContext, nameLength, nameBuffer);

	nameBuffer[nameLength] = 0;

	p_MaterialFormat->materialName = nameBuffer;
	p_MaterialFormat->materialNameLength = nameLength;

	//submaterial count
	K15_ReadData(p_DataAcessContext, sizeof(uint16), &p_MaterialFormat->submaterialCount);

	K15_SubMaterialFormat* submaterialMemory = (K15_SubMaterialFormat*)K15_RF_MALLOC(sizeof(K15_SubMaterialFormat) * p_MaterialFormat->submaterialCount);

	if(!submaterialMemory)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	p_MaterialFormat->subMaterials = submaterialMemory;

	for(uint32 submaterialIndex = 0;
		submaterialIndex < p_MaterialFormat->submaterialCount;
		++submaterialIndex)
	{
		K15_SubMaterialFormat* currentSubmaterial = &p_MaterialFormat->subMaterials[submaterialIndex];

		//diffuse color
		K15_ReadData(p_DataAcessContext, sizeof(float) * 3, &currentSubmaterial->diffuseColor);

		//specular color
		K15_ReadData(p_DataAcessContext, sizeof(float) * 3, &currentSubmaterial->specularColor);

		//ambient color
		K15_ReadData(p_DataAcessContext, sizeof(float) * 3, &currentSubmaterial->ambientColor);

		//shininess
		K15_ReadData(p_DataAcessContext, sizeof(float), &currentSubmaterial->shininess);

		//shininess power
		K15_ReadData(p_DataAcessContext, sizeof(float), &currentSubmaterial->sininessStrength);

		//diffuse texture count
		K15_ReadData(p_DataAcessContext, sizeof(uint8), &currentSubmaterial->diffuseTextureCount);

		K15_SubMaterialTextureFormat* textureFormatMemory = (K15_SubMaterialTextureFormat*)K15_RF_MALLOC(sizeof(K15_SubMaterialTextureFormat) * currentSubmaterial->diffuseTextureCount);
		if(!textureFormatMemory)
		{
			K15_RF_FREE(submaterialMemory);
			return K15_ERROR_OUT_OF_MEMORY;
		}

		currentSubmaterial->diffuseTextureFormat = textureFormatMemory;

		for(uint32 diffuseTextureIndex = 0;
			diffuseTextureIndex < currentSubmaterial->diffuseTextureCount;
			++diffuseTextureIndex)
		{
			K15_SubMaterialTextureFormat* submaterialTextureFormat = &currentSubmaterial->diffuseTextureFormat[diffuseTextureIndex];

			//texture name length
			K15_ReadData(p_DataAcessContext, sizeof(uint32), &submaterialTextureFormat->textureNameLength);

			//create buffer for texture name
			char* textureName = (char*)K15_RF_MALLOC(submaterialTextureFormat->textureNameLength + 1); //+1 for null terminator

			//texture name
			K15_ReadData(p_DataAcessContext, submaterialTextureFormat->textureNameLength, textureName);

			textureName[submaterialTextureFormat->textureNameLength] = 0;

			submaterialTextureFormat->textureName = textureName;

			//blend strength
			K15_ReadData(p_DataAcessContext, sizeof(float), &submaterialTextureFormat->blendStrength);

			//blend operation
			K15_ReadData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->blendOperation);

			//texture mapping (U)
			K15_ReadData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->textureMappingU);

			//texture mapping (V)
			K15_ReadData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->textureMappingV);

			//alpha usage
			K15_ReadData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->hasAlpha);
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

	//material name lenght
	K15_WriteData(p_DataAcessContext, sizeof(uint32), &p_MaterialFormat->materialNameLength);

	//material name
	K15_WriteData(p_DataAcessContext, p_MaterialFormat->materialNameLength, p_MaterialFormat->materialName);

	//submaterial count
	K15_WriteData(p_DataAcessContext, sizeof(uint16), &p_MaterialFormat->submaterialCount);

	for(uint16 submaterialIndex = 0;
		submaterialIndex < p_MaterialFormat->submaterialCount;
		++submaterialIndex)
	{
		K15_SubMaterialFormat* submaterialFormat = &p_MaterialFormat->subMaterials[submaterialIndex];

		//diffuse color
		K15_WriteData(p_DataAcessContext, sizeof(float) * 3, &submaterialFormat->diffuseColor);

		//specular color
		K15_WriteData(p_DataAcessContext, sizeof(float) * 3, &submaterialFormat->specularColor);

		//ambient color
		K15_WriteData(p_DataAcessContext, sizeof(float) * 3, &submaterialFormat->ambientColor);

		//shininess
		K15_WriteData(p_DataAcessContext, sizeof(float), &submaterialFormat->shininess);

		//shininess power
		K15_WriteData(p_DataAcessContext, sizeof(float), &submaterialFormat->sininessStrength);

		//diffuse texture count
		K15_WriteData(p_DataAcessContext, sizeof(uint8), &submaterialFormat->diffuseTextureCount);

		for(uint16 diffuseTextureIndex = 0;
			diffuseTextureIndex < submaterialFormat->diffuseTextureCount;
			++diffuseTextureIndex)
		{
			K15_SubMaterialTextureFormat* submaterialTextureFormat = &submaterialFormat->diffuseTextureFormat[diffuseTextureIndex];

			//texture name length
			K15_WriteData(p_DataAcessContext, sizeof(uint32), &submaterialTextureFormat->textureNameLength);

			//texture name
			K15_WriteData(p_DataAcessContext, submaterialTextureFormat->textureNameLength, submaterialTextureFormat->textureName);

			//blend strength
			K15_WriteData(p_DataAcessContext, sizeof(float), &submaterialTextureFormat->blendStrength);

			//blend operation
			K15_WriteData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->blendOperation);

			//texture mapping (U)
			K15_WriteData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->textureMappingU);

			//texture mapping (V)
			K15_WriteData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->textureMappingV);

			//alpha usage
			K15_WriteData(p_DataAcessContext, sizeof(uint8), &submaterialTextureFormat->hasAlpha);
		}
	}

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeMaterialFormat(*p_MaterialFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_SetMaterialName(K15_MaterialFormat* p_MaterialFormat, const char* p_Name)
{
	assert(p_MaterialFormat && p_Name);

	uint32 nameLength = (uint32)strlen(p_Name);
	char* nameBuffer = (char*)K15_RF_MALLOC(nameLength);
	
	if (!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	memcpy(nameBuffer, p_Name, nameLength);

	p_MaterialFormat->materialNameLength = nameLength;
	p_MaterialFormat->materialName = nameBuffer;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubMaterialCount(K15_MaterialFormat* p_MaterialFormat, uint16 p_SubMaterialCount)
{
	assert(p_MaterialFormat && p_SubMaterialCount > 0);

	K15_SubMaterialFormat* subMaterialMemory = (K15_SubMaterialFormat*)K15_RF_MALLOC(sizeof(K15_SubMaterialFormat) * p_SubMaterialCount);

	if(!subMaterialMemory)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	p_MaterialFormat->subMaterials = subMaterialMemory;
	p_MaterialFormat->submaterialCount = p_SubMaterialCount;
	
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubMaterialTextureCount(K15_SubMaterialFormat* p_SubMaterialFormat, uint32 p_TextureCount, uint32 p_TextureIdentifier)
{
	assert(p_SubMaterialFormat && p_TextureCount > 0 && p_TextureIdentifier > 0);

	K15_SubMaterialTextureFormat* textureFormatMemory = (K15_SubMaterialTextureFormat*)K15_RF_MALLOC(p_TextureCount * sizeof(K15_SubMaterialTextureFormat));

	if(!textureFormatMemory)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	switch(p_TextureIdentifier)
	{
	case K15_MATERIAL_TEXTURE_DIFFUSE:
		{
			p_SubMaterialFormat->diffuseTextureFormat = textureFormatMemory;
			p_SubMaterialFormat->diffuseTextureCount = p_TextureCount;
			break;
		}
	default:
		{
			if(textureFormatMemory)
			{
				K15_RF_FREE(textureFormatMemory);
			}

			return K15_ERROR_WRONG_DATA_IDENTIFIER;
		}
	}
	
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubMaterialTextureFormatTextureName(K15_SubMaterialTextureFormat* p_SubMaterialTextureFormat, const char* p_TextureName)
{
	assert(p_SubMaterialTextureFormat && p_TextureName);

	uint32 lengthName = (uint32)strlen(p_TextureName);
	char* nameBuffer = (char*)K15_RF_MALLOC(lengthName);

	if(!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	memcpy(nameBuffer, p_TextureName, lengthName);

	p_SubMaterialTextureFormat->textureNameLength = lengthName;
	p_SubMaterialTextureFormat->textureName = nameBuffer;

	return K15_SUCCESS;
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
	for(uint16 submaterialFormatIndex = 0;
		submaterialFormatIndex < p_MaterialFormat.submaterialCount;
		++submaterialFormatIndex)
	{
		K15_SubMaterialFormat submaterialFormat = p_MaterialFormat.subMaterials[submaterialFormatIndex];
	
		for(uint16 submaterialDiffuseTextureIndex = 0;
			submaterialDiffuseTextureIndex < submaterialFormat.diffuseTextureCount;
			++submaterialDiffuseTextureIndex)
		{
			K15_SubMaterialTextureFormat submaterialDiffuseTextureFormat = submaterialFormat.diffuseTextureFormat[submaterialDiffuseTextureIndex];
			K15_RF_FREE(submaterialDiffuseTextureFormat.textureName);
			submaterialDiffuseTextureFormat.textureName = 0;
		}

		K15_RF_FREE(submaterialFormat.diffuseTextureFormat);
		submaterialFormat.diffuseTextureFormat = 0;
	}

	K15_RF_FREE(p_MaterialFormat.materialName);
	K15_RF_FREE(p_MaterialFormat.subMaterials);
	p_MaterialFormat.subMaterials = 0;
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