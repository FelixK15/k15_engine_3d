#ifndef _K15_Rendering_RenderMaterialDesc_h_
#define _K15_Rendering_RenderMaterialDesc_h_

#include "K15_RenderPrerequisites.h"
#include "generated/K15_RenderMaterialDataDescStretchBuffer.h"

struct K15_RenderMaterialDataDesc
{
	union
	{
		byte* rawData;
		K15_RenderResourceHandle* renderResourceHandle;
		float floatValue;
		int intValue;
	} data;
	//byte* data;
	//char* name;
	uint32 nameHash;
	//uint32 dataSizeInBytes;
	uint32 typeID;
	uint32 semanticID;
	uint32 semanticGroupID;
};

struct K15_RenderMaterialPassDesc
{
	K15_RenderResourceHandle* vertexShaderHandle;
	K15_RenderResourceHandle* fragmentShaderHandle;

	K15_RenderMaterialDataDescStretchBuffer materialData;
};

struct K15_RenderMaterialDesc
{
	uint32 numMaterialPasses;
	K15_RenderMaterialPassDesc* materialPasses;
};

bool8 K15_RenderMaterialDescHashCompare(K15_RenderMaterialDataDesc* p_Element, void* p_UserData);

void K15_SetRenderMaterialRenderResourceDataByName(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, K15_RenderResourceHandle* p_RenderResourceHandle);
void K15_SetRenderMaterialFloatValueDataByName(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, float p_Float);
void K15_SetRenderMaterialIntValueByName(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, int p_Int);

#endif //_K15_Rendering_RenderMaterialDesc_h_