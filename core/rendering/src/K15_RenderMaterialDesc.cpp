#include "K15_RenderMaterialDesc.h"

/*********************************************************************************/
bool8 K15_RenderMaterialDescHashCompare(K15_RenderMaterialDataDesc* p_Element, void* p_UserData)
{
	uint32* userHash = (uint32*)p_UserData;
	uint32 elementHash = p_Element->nameHash;

	return (*userHash) == elementHash ? 0 : 1;
}
/*********************************************************************************/
void K15_SetRenderMaterialDataAsString(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, const char* p_Value)
{
	K15_ASSERT_TEXT(p_MaterialPass, "Material Pass is NULL.");

	uint32 valueNameHash = K15_GenerateStringHash(p_DataValueName);

	K15_RenderMaterialDataDescStretchBuffer* materialDataBuffer = &p_MaterialPass->materialData;
	K15_RenderMaterialDataDesc* dataDesc = K15_GetRenderMaterialDataDescStretchBufferElementConditional
		(materialDataBuffer, K15_RenderMaterialDescHashCompare, &valueNameHash);

	K15_ASSERT_TEXT(dataDesc, "Material Pass data value with the name '%s' does not exist.", p_DataValueName);
	
	//MEMORY LEAK!
	dataDesc->data = (byte*)K15_CopyString(p_Value);
	dataDesc->dataSizeInBytes = (uint32)strlen(p_Value);
}
/*********************************************************************************/
void K15_SetRenderMaterialDataAsInt(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, int p_Value)
{
	K15_ASSERT_TEXT(p_MaterialPass, "Material Pass is NULL.");

	uint32 valueNameHash = K15_GenerateStringHash(p_DataValueName);

	K15_RenderMaterialDataDescStretchBuffer* materialDataBuffer = &p_MaterialPass->materialData;
	K15_RenderMaterialDataDesc* dataDesc = K15_GetRenderMaterialDataDescStretchBufferElementConditional
		(materialDataBuffer, K15_RenderMaterialDescHashCompare, &valueNameHash);

	K15_ASSERT_TEXT(dataDesc, "Material Pass data value with the name '%s' does not exist.", p_DataValueName);

	//MEMORY LEAK!
// 	dataDesc->data = (byte*)K15_CopyString(p_Value);
// 	dataDesc->dataSizeInBytes = (uint32)strlen(p_Value);
}
/*********************************************************************************/
void K15_SetRenderMaterialDataAsFloat(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, float p_Value)
{
	K15_ASSERT_TEXT(p_MaterialPass, "Material Pass is NULL.");

	uint32 valueNameHash = K15_GenerateStringHash(p_DataValueName);

	K15_RenderMaterialDataDescStretchBuffer* materialDataBuffer = &p_MaterialPass->materialData;
	K15_RenderMaterialDataDesc* dataDesc = K15_GetRenderMaterialDataDescStretchBufferElementConditional
		(materialDataBuffer, K15_RenderMaterialDescHashCompare, &valueNameHash);

	K15_ASSERT_TEXT(dataDesc, "Material Pass data value with the name '%s' does not exist.", p_DataValueName);

	//MEMORY LEAK!
// 	dataDesc->data = (byte*)K15_CopyString(p_Value);
// 	dataDesc->dataSizeInBytes = (uint32)strlen(p_Value);
}
/*********************************************************************************/